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
 * - \ref check_socket_validity is just a stub
 * - refactor main.c, its starting to get smelly
 * - use mkdtemp() when storing temporary screenshot
 * - catch SIGTERM or provide a way to clean exit (close sockets).
 * - forking the process when new connection is received can be a good idea.
 * - what about a heartbeat between cellphone and server?
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

#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include "x11_event.h"
#include "bluecode.h"
#include "protocol.h"
#include "log.h"
#include "imscreen.h"

/** Show program usage
 *
 * @param path the program binary path
 *
 */
static void show_usage(const char *path);

/** Checks for client socket status, if its still valid.
 *
 * Currently, only a dummy function, its not able to detect if the client
 * is dead.
 *
 * @param client_socket The client socket descriptor.
 *
 * @return 0 when connection is ok, -1 for error.
 * \todo Write a function that really works.
 */
static int check_socket_validity(int client_socket);

/** Check for protocol commands in buffer, used by \ref process_events
 *
 *
 * @param buffer A string buffer with commands (i.e. CONN_CLOSE), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param client_socket Client socket connection descriptor. This function will
 * take screenshots and write data back to client.
 * @param active_display Pointer to active display. This function need access to
 * display connection to grab screenshots.
 *
 * @return Number of bytes read on sucess, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
static int treat_command(char *buffer, int length, int client_socket, Display *active_display);


/** Check for protocol commands, handle input events (mouse
 * and keyboard).
 *
 *
 * @param buffer A string buffer with commands (i.e. UP, DOWN), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param active_display Pointer to active display.
 * @param log A structure of log resources, see \ref log_resource.
 * @param client_socket Client socket connection descriptor (really used by
 * \ref treat_command).
 *
 * @return Number of bytes read on sucess, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
static int treat_events(char *buffer, int length, Display *active_display,
		 struct log_resource *log, int client_socket);


/** Process event stream. Reads what new commands are being received
 * in socket and send them to X session.
 *
 * @param fd Socket file descriptor.
 * @param active_display Pointer to active display.
 * @param clean_up Free local allocated resources.
 * @param log A structure of log resources, see \ref log_resource.
 *
 * @return Number of bytes read on sucess, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
static int process_events(int fd, Display *active_display, int clean_up,
		   struct log_resource *log);



/** Main app function.
 *
 * @param argc argument counter
 * @param argv argument vector
 *
 * @return 0 on sucess, -1 on error.
 */
int main(int argc, char **argv)
{
	Display *own_display = NULL;
	int server_socket, client_socket, channel = 10, res;
	int clean_up = 0;
	fd_set fd_set_socket;
	struct timeval time_socket;
	struct sockaddr rem_addr;
	unsigned int opt = sizeof(rem_addr);
	struct service_description *sd = NULL;
	struct log_resource *log = NULL;
	int length = 20;
	char buffer[length], arg, *logfile = NULL;
	time_t last_test;

	if (argc > 4) {
		show_usage(argv[0]);
		return -1;
	}

	while ((arg = getopt(argc, argv, "l:h")) != -1) {
		switch (arg) {
			case 'l':
				logfile = optarg;
				break;
			case 'h':
				show_usage(argv[0]);
				return 0;
			default:
				return -1;
		}
	}

	if (!logfile)
		logfile = "amora.log";

	memset(&rem_addr, 0, sizeof(struct sockaddr));

	log = log_build_resources(logfile);

	if (check_device() < 0) {
		log_message(FIL|OUT, log, "No bluetooth device/dongle available."
				" Aborting...\n");
		return -1;
	}

	own_display = construct_display(NULL);
	if (!own_display) {
		log_message(FIL|OUT, log, "Error creating display object!"
				"Aborting...\n");
		return -1;
	}

	/* Service description registering */
	sd = build_sd(channel);
	if (!sd) {
		log_message(FIL|OUT, log, "Error creating service description"
			    "object!"
			    "Aborting...\n");
		return -1;
	}
	res = describe_service(sd);
	if (res == -1) {
		log_message(FIL|OUT, log, "Error registering service!"
			    "Aborting...\n");
		destroy_sd(sd);
		return -1;
	}


	/* Socket creation */
	server_socket = build_bluetooth_socket(channel, sd);
	if (server_socket == -1) {
		log_message(FIL|OUT, log, "Failed creating bluetooth conn!"
			    "Exiting...\n");
		return -1;
	}

	log_message(FIL, log, "Bluetooth device code hci = %d\n", sd->hci_id);
	log_message(FIL|OUT, log, "\nInitialization done, waiting cellphone"
		    " connection...\n");

	res = listen(server_socket, 10);
	if (res) {
		log_message(FIL|OUT, log, "Failed listening...\n");
		return -1;
	}

	while (1) {
		log_message(FIL|OUT, log, "Entering main loop...\n");
		client_socket = accept(server_socket,
				       (struct sockaddr *)&rem_addr, &opt);
		if (client_socket == -1) {
			log_message(FIL|OUT, log, "Failed opening connection,"
				    " exiting...\n");
			goto exit;
		}

		client_bluetooth_id(&rem_addr, buffer);
		log_message(FIL|OUT, log, "Accepted connection. Client is "
			    "%s\n", buffer);

		res = process_events(client_socket, own_display, clean_up,
				     log);

		FD_ZERO(&fd_set_socket);
		FD_SET(client_socket, &fd_set_socket);
		time_socket.tv_sec = 5;
		time_socket.tv_usec = 0;

		last_test = time(NULL);
		while ((res = select(client_socket + 1, &fd_set_socket, NULL,
				     NULL, &time_socket)) != -1) {

			/* Linux resets timeout */
			FD_ZERO(&fd_set_socket);
			FD_SET(client_socket, &fd_set_socket);
			time_socket.tv_sec = 5;
			time_socket.tv_usec = 0;

			if (res == 1) {
				res = check_socket_validity(client_socket);

				if (res < 0) {

					/* FIXME: 3x the same block code
					 * is awful.
					 */
					log_message(FIL|OUT, log,
						    "Connection no longer"
						    "valid\n");

					close(client_socket);
					client_socket = -1;
					break;
				}

				res = process_events(client_socket,
						     own_display, clean_up,
						     log);

				if (res == CONN_CLOSE) {
					log_message(FIL|OUT, log,
						    "Client asked to close "
						    "connection\n\n");
					close(client_socket);
					client_socket = -1;
					break;
				}

				if (res == -1) {
					log_message(FIL|OUT, log,
						    "Client died or closed "
						    "connection\n\n");
					close(client_socket);
					client_socket = -1;
					break;
				}

				last_test = time(NULL);
			}

			res = time(NULL);
			if ((res - last_test) > 20) {
				log_message(FIL, log, "Timeout, check for "
					    "socket status");
				last_test = res;
				res = check_socket_validity(client_socket);
				if (res < 0) {
					log_message(FIL|OUT, log,
						    "Connection no longer"
						    "valid\n");

					close(client_socket);
					client_socket = -1;
					break;
				}

			}

		}

	}

exit:
	res = destroy_display(own_display);
	log_message(FIL|OUT, log, "Done, we are closing now.\n");
	close(server_socket);
	destroy_sd(sd);
	res = process_events(client_socket = 0, own_display = NULL,
			     clean_up = 1, log);
	log_clean_resources(log);

	return 0;
}


static int process_events(int fd, Display *active_display, int clean_up,
		   struct log_resource *log)
{
	static char *buffer = NULL;
	const int BUF_SIZE = 300;
	int bytes_read, result = -1;
	char *start, *end;

	/* Call to just cleanup local allocated memory. */
	if ((clean_up == 1) && (fd == 0) && (active_display == NULL)) {
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
	if (bytes_read == -1) {
		log_message(FIL|OUT, log, "Error trying to read socket!");
		return result;
	}

	log_message(FIL, log, "Read buffer = %s\n", buffer);

	start = buffer;
	while ((end = strchr(start, CMD_BREAK))) {
		result = treat_events(start, (end - start), active_display,
				      log, fd);
		start = ++end;
	}

	return result;
}


static int treat_events(char *buffer, int length, Display *active_display,
		 struct log_resource *log, int client_socket)
{
	static unsigned char mouse_event = 0, times = 0,
		button_right = 0, button_left = 0, button_middle = 0;
	static int x_mouse, y_mouse;
	int result;

	/* TODO: move this whole code block to a distinct function */
	result = ecell_button_ewindow(buffer, length);
	log_message(FIL, log, "ecell_button = %d\n", result);
	if (result == NONE) {

		result = ecell_mouse_ewindow(buffer, length);
		log_message(FIL, log, "ecell_mouse = %d\n", result);
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
			mouse_click(result, 0, active_display);
			break;
		case MOUSE_SCROLL_DOWN:
			mouse_click(result, 0, active_display);
			break;
		case NONE:
			log_message(FIL, log, "mouse_event = %d", mouse_event);
			if (mouse_event == 1) {
				if (times == 0) {
					x_mouse = atoi(buffer);
					++times;
				} else {
					y_mouse = atoi(buffer);

					log_message(FIL, log,
						    "x = %d\ty=%d\t",
						    x_mouse, y_mouse);

					result = mouse_move(x_mouse, y_mouse,
							      active_display);
					if (result == -1)
						log_message(FIL|OUT, log,
							    "Can't"
							    "move mouse!");

					times = 0;
					mouse_event = 0;
				}
			} else {
				result = treat_command(buffer, length,
						       client_socket,
						       active_display);
				if (result == CONN_CLOSE) {
					mouse_event = 0;
					times = 0;
					goto exit;
				} else if (result == NONE)
					log_message(FIL|OUT, log,
						    "Invalid event!\n");
			}
			break;
		default:
			if (button_right)
				mouse_click(MOUSE_BUTTON_RIGHT, result,
					    active_display);
			else if (button_left)
				mouse_click(MOUSE_BUTTON_LEFT, result,
					    active_display);
			else if (button_middle)
				mouse_click(MOUSE_BUTTON_MIDDLE, result,
					    active_display);
			break;
		}
		goto exit;
	}

	if (result != NONE) {
		send_event(KeyPress, x_key_code[result], active_display);
		goto exit;
	}

exit:
	return result;

}


static int treat_command(char *buffer, int length, int client_socket,
		  Display *active_display)
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
		tmp = screen_capture(active_display, &image);
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

	printf("Usage: %s [-l logfile] [-h]\n"
	       "\n"
	       "  -h                         show this help message.\n"
	       "  -l logfile                 set the log file path.\n"
	       "\n", name);

	free(p);
}

static int check_socket_validity(int client_socket)
{
	int res;
	char buffer[1];

	/* Try to read twice */
	res = read(client_socket, buffer, 0);
	if (!res)
		res = read(client_socket, buffer, 0);

	return res;
}

