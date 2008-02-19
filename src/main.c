/**
 * @file   main.c
 * @author Adenilson Cavalcanti
 * @date   Fri May  4 14:25:09 2007
 *
 * @brief  Main server side presenter app.
 *
 * This app is reponsible to receive events from cellphone and send them to
 * X Server.
 * \todo
 * - use mkdtemp() when storing temporary screenshot
 * - catch SIGTERM or provide a way to clean exit (close sockets).
 * - add utests using 'check' library
 *
 */

/*  Copyright (C) 2007  Adenilson Cavalcanti <savagobr@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; by version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_DBUS
#include <dbus/dbus.h>
#endif

#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#include "x11_event.h"
#include "bluecode.h"
#include "bluetooth-bluez-dbus.h"
#include "protocol.h"
#include "dbus.h"
#include "log.h"
#include "loop.h"
#include "imscreen.h"


/** Amora global struct that holds the main resources */
struct amora_s {
	/** Log resource */
	struct log_resource *log;
	/** X11 display */
	Display *display;

#ifdef HAVE_DBUS
	/** D-Bus Connection */
	DBusConnection *dbus;
#endif

} amora;


/** Client file descriptor callback
 *
 * @param client_socket the file descriptor itself
 * @param data the userdata
 *
 * @return 0 on success, -1 otherwise
 *
 */
static int client_socket_cb(int client_socket, void *data);


/** Server file descriptor callback
 *
 * @param server_socket the file descriptor itself
 * @param data the userdata
 *
 * @return 0 on success, -1 otherwise
 *
 */
static int server_socket_cb(int server_socket, void *data);


/** Show program usage
 *
 * @param path the program binary path
 *
 */
static void show_usage(const char *path);


/** Check for protocol commands in buffer, used by \ref process_events
 *
 *
 * @param buffer A string buffer with commands (i.e. CONN_CLOSE), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param client_socket Client socket connection descriptor. This function will
 * take screenshots and write data back to client.
 * display connection to grab screenshots.
 *
 * @return Number of bytes read on success, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
static int treat_command(char *buffer, int length, int client_socket);


/** Check for protocol commands, handle input events (mouse
 * and keyboard).
 *
 *
 * @param buffer A string buffer with commands (i.e. UP, DOWN), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param client_socket Client socket connection descriptor (really used by
 * \ref treat_command).
 *
 * @return Number of bytes read on success, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
static int treat_events(char *buffer, int length, int client_socket);


/** Process event stream. Reads what new commands are being received
 * in socket and send them to X session.
 *
 * @param fd Socket file descriptor.
 * @param clean_up Free local allocated resources.
 *
 * @return Number of bytes read on success, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
static int process_events(int fd, int clean_up);


/** Main app function.
 *
 * @param argc argument counter
 * @param argv argument vector
 *
 * @return 0 on success, -1 on error.
 */
int main(int argc, char **argv)
{
	int server_socket, client_socket, channel = 10, res;
	int clean_up = 0;
	struct service_description *sd = NULL;
	char arg, hci_id[6];
	const char *logfile = NULL;

	if (argc > 4) {
		show_usage(argv[0]);
		return -1;
	}

	while ((arg = getopt(argc, argv, "l:hv")) != -1) {
		switch (arg) {
			case 'l':
				logfile = optarg;
				break;
			case 'h':
				show_usage(argv[0]);
				return 0;
			case 'v':
				printf("%s (build %s)\n", PACKAGE_STRING,
						BUILDVERSION);
				return 0;
			default:
				return -1;
		}
	}

	if (!(amora.log = log_build_resources(logfile)))
		perror("Failed log resource creation!");

	if (check_device() < 0) {
		log_message(FIL|OUT, amora.log, "No bluetooth device/dongle available."
				" Aborting...");
		return -1;
	}

	amora.display = construct_display(NULL);
	if (!amora.display) {
		log_message(FIL|OUT, amora.log, "Error creating display object!"
				"Aborting...");
		return -1;
	}

	/* Service description registering */
	sd = build_sd(channel);
	if (!sd) {
		log_message(FIL|OUT, amora.log, "Error creating service description"
				"object! Aborting...");
		return -1;
	}
	res = describe_service(sd);
	if (res == -1) {
		log_message(FIL|OUT, amora.log, "Error registering service!"
				"Aborting...");
		destroy_sd(sd);
		return -1;
	}

	/* Socket creation */
	server_socket = build_bluetooth_socket(channel, sd);
	if (server_socket == -1) {
		log_message(FIL|OUT, amora.log, "Failed creating bluetooth conn!"
				"Exiting...");
		return -1;
	}

	loop_add(server_socket, server_socket_cb, NULL);

	snprintf(hci_id, sizeof(hci_id), "hci%d", sd->hci_id);

#ifdef HAVE_DBUS
	if ((amora.dbus = dbus_init()) == NULL) {
		log_message(FIL|OUT, amora.log, "Error while initilizing "
				"D-Bus! Aborting...");
		return -1;
	}

	if (dbus_bt_dongle_removed_watch(amora.dbus, hci_id)) {
		log_message(FIL|OUT, amora.log, "Error while initilizing "
				"D-Bus with BlueZ! Aborting...");
		return -1;
	}
#endif

	log_message(FIL, amora.log, "Bluetooth device code hci = %d", sd->hci_id);
	log_message(FIL|OUT, amora.log, "Initialization done, waiting cellphone"
			" connection...");

	res = listen(server_socket, 10);
	if (res) {
		log_message(FIL|OUT, amora.log, "Failed listening...");
		return -1;
	}


	log_message(FIL|OUT, amora.log, "Entering main loop...");
	loop();

	res = destroy_display(amora.display);
	amora.display = NULL;
	log_message(FIL|OUT, amora.log, "Done, we are closing now.");
	close(server_socket);
	destroy_sd(sd);
	res = process_events(client_socket = 0, clean_up = 1);
	log_clean_resources(amora.log);

	return 0;
}


static int process_events(int fd, int clean_up)
{
	static char *buffer = NULL;
	const int BUF_SIZE = 300;
	int bytes_read, result = -1;
	char *start, *end;

	/* Call to just cleanup local allocated memory. */
	if ((clean_up == 1) && (fd == 0) && (amora.display == NULL)) {
		if (buffer != NULL)
			free(buffer);

		buffer = NULL;
		return 0;
	}

	/* Heap memory is easier to audit */
	if (!buffer) {
		buffer = malloc(BUF_SIZE);
		if (!buffer) {
			perror("Buffer memory allocation failed: ");
			return result;
		}
	}

	bytes_read = read_socket(fd, buffer, BUF_SIZE);
	if (bytes_read == -1 || bytes_read == 0) {
		log_message(FIL|OUT, amora.log, "Error trying to read socket!");
		return result;
	}

	log_message(FIL, amora.log, "Read buffer = %s", buffer);

	start = buffer;
	while ((end = strchr(start, CMD_BREAK))) {
		result = treat_events(start, (end - start), fd);
		start = ++end;
	}

	return result;
}


static int treat_events(char *buffer, int length, int client_socket)
{
	static unsigned char mouse_event = 0, times = 0,
		button_right = 0, button_left = 0, button_middle = 0;
	static int x_mouse, y_mouse;
	int result;

	/* TODO: move this whole code block to a distinct function */
	result = ecell_button_ewindow(buffer, length);
	log_message(FIL, amora.log, "ecell_button = %d", result);
	if (result == NONE) {

		result = ecell_mouse_ewindow(buffer, length);
		log_message(FIL, amora.log, "ecell_mouse = %d", result);
		switch (result) {
		case MOUSE_MOVE:
			mouse_event = 1;
			break;
		case MOUSE_BUTTON_RIGHT:
			button_right = 1;
			button_left = button_middle = 0;
			break;
		case MOUSE_BUTTON_LEFT:
			button_left = 1;
			button_right = button_middle = 0;
			break;
		case MOUSE_BUTTON_MIDDLE:
			button_middle = 1;
			button_right = button_left = 0;
			break;
		case MOUSE_SCROLL_UP:
			mouse_click(result, 0, amora.display);
			break;
		case MOUSE_SCROLL_DOWN:
			mouse_click(result, 0, amora.display);
			break;
		case NONE:
			log_message(FIL, amora.log, "mouse_event = %d", mouse_event);
			if (mouse_event == 1) {
				if (times == 0) {
					x_mouse = atoi(buffer);
					++times;
				} else {
					y_mouse = atoi(buffer);

					log_message(FIL, amora.log,
						    "x = %d\ty=%d\t",
						    x_mouse, y_mouse);

					result = mouse_move(x_mouse, y_mouse,
							      amora.display);
					if (result == -1)
						log_message(FIL|OUT, amora.log,
							    "Can't"
							    "move mouse!");

					times = 0;
					mouse_event = 0;
				}
			} else {
				result = treat_command(buffer, length,
						       client_socket);
				if (result == CONN_CLOSE) {
					mouse_event = 0;
					times = 0;
					goto exit;
				} else if (result == NONE)
					log_message(FIL|OUT, amora.log,
						    "Invalid event!");
			}
			break;
		default:
			if (button_right)
				mouse_click(MOUSE_BUTTON_RIGHT, result,
					    amora.display);
			else if (button_left)
				mouse_click(MOUSE_BUTTON_LEFT, result,
					    amora.display);
			else if (button_middle)
				mouse_click(MOUSE_BUTTON_MIDDLE, result,
					    amora.display);
			break;
		}
		goto exit;
	}

	if (result != NONE) {
		send_event(KeyPress, x_key_code[result], amora.display);
		goto exit;
	}

exit:
	return result;

}


static int treat_command(char *buffer, int length, int client_socket)
{

	static int do_capture = 0, screen_rotate = 0,
		width = 176, height = 208, flag = 0, times = 0;
	int result, tmp;
	Imlib_Image image, rescaled;

	result = protocol_command(buffer, length);

	switch (result) {

	case CONN_CLOSE:
		do_capture = screen_rotate = 0;
		width = 320;
		height = 240;
		flag = 0;
		break;
	case SERVER_STOP: /* TODO: add server stop code */
		break;
	case RESOLUTION:
		break;
	case IMG_FORMAT:
		break;
	case SCREEN_MODE_ON:
		do_capture = 1;
		break;
	case SCREEN_MODE_OFF:
		do_capture = 0;
		break;
	case SCREEN_ROTATE:
		screen_rotate = 1;
		break;
	case SCREEN_NORMAL:
		screen_rotate = 0;
		break;
	case SCREEN_RESOLUTION:
		flag = 1;
		break;
	case SCREEN_WIDTH:
		break;
	case SCREEN_HEIGHT:
		break;
	case SCREEN_TAKE:
		tmp = screen_capture(amora.display, &image);
		if (tmp) {
			perror("failed screen capture!\n");
			result = NONE;
			break;
		}

		tmp = rescale_image(&image, width, height, &rescaled);
		imlib_free_image();
		if (tmp) {
			perror("failed screen capture!\n");
			result = NONE;
			break;
		}

		if (screen_rotate) {
			tmp = rotate_image(&rescaled);
			if (tmp) {
				imlib_free_image();
				perror("failed screen capture!\n");
				result = NONE;
				break;
			}
		}

		/* XXX, TODO: we should have a tmp or working dir for
		 * amora-server created with mkdtemp(3) */
		tmp = save_image(&rescaled, "amora-screenshot.png");
		imlib_free_image();
		if (tmp) {
			perror("failed screen capture!\n");
			result = NONE;
			break;
		}

		tmp = send_file(client_socket, "amora-screenshot.png");
		if (tmp) {
			perror("failed screen transfer!\n");
			result = NONE;
			break;
		}
		break;
	case NONE:
		tmp = atoi(buffer);
		if (flag)
			result = SCREEN_RESOLUTION;

		if (flag && (!times)) {
			++times;
			if (tmp > 0)
				width = tmp;
		} else if (flag && times) {
			flag = times = 0;
			if (tmp > 0)
				height = tmp;
		}

	}

	return result;
}

static void show_usage(const char *path)
{
	char *name, *p = strdup(path);

	name = basename(p);

	printf("Usage: %s [-l logfile] [-h] [-v]\n"
	       "\n"
	       "  -h             show this help message and exit;\n"
	       "  -l logfile     set the log file path (default is disabled);\n"
	       "  -v             show version and exit.\n"
	       "\n", name);

	free(p);
}

static int client_socket_cb(int client_socket, void *data)
{
	(void) data;

	int res = process_events(client_socket, 0);

	if (res == CONN_CLOSE) {
		log_message(FIL|OUT, amora.log,"Client asked to close connection.");
		loop_remove(client_socket);
		close(client_socket);
		client_socket = -1;
	}

	if (res == -1) {
		log_message(FIL|OUT, amora.log, "Client died or closed connection.");
		loop_remove(client_socket);
		close(client_socket);
		client_socket = -1;
	}

	return 0;
}

static int server_socket_cb(int server_socket, void *data)
{
	char buffer[20];
	struct sockaddr rem_addr;
	unsigned int opt = sizeof(struct sockaddr);
	int client_socket, flags;

	(void) data;

	memset(&rem_addr, 0, sizeof(struct sockaddr));

	client_socket = accept(server_socket,
			(struct sockaddr *) &rem_addr, &opt);

	if (client_socket == -1)
		log_message(FIL|OUT, amora.log, "Failed opening connection,"
				" exiting...");
	else {
		flags = fcntl(client_socket, F_GETFL);
		flags |= O_NONBLOCK;
		fcntl(client_socket, F_SETFL, flags);

		client_bluetooth_id(&rem_addr, buffer);
		log_message(FIL|OUT, amora.log, "Accepted connection. Client"
				" is %s", buffer);
		loop_add(client_socket, client_socket_cb, NULL);
	}

	return client_socket < 0 ? -1 : 0;
}
