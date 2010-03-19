#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/param.h>
#include <errno.h>
#include "config.h"
#include "amora.h"
#include "x11_event.h"
#include "bluecode.h"
#include "protocol.h"
#include "dbus.h"
#include "log.h"
#include "loop.h"
#include "imscreen.h"

int process_events(struct amora_s *amora, int fd, int clean_up)
{
	static char *buffer = NULL;
	const int BUF_SIZE = 300;
	int bytes_read, result = -1;
	char *start, *end;

	/* Call to just cleanup local allocated memory. */
	if ((clean_up == 1) && (fd == 0) && (amora->display == NULL)) {
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
		log_message(FIL|OUT, amora->log, "Error trying to read socket!");
		return result;
	}

	log_message(FIL, amora->log, "Read buffer = %s", buffer);

	start = buffer;
	while ((end = strchr(start, CMD_BREAK))) {
		result = treat_events(amora, start, (end - start), fd);
		if (result == -1) {
			memset(buffer, 0, BUF_SIZE);
			break;
		}
		start = ++end;
	}

	return result;
}


int treat_events(struct amora_s *amora, char *buffer, int length,
		 int client_socket)
{
	static unsigned char mouse_event = 0, times = 0,
		button_right = 0, button_left = 0, button_middle = 0;
	static int x_mouse, y_mouse;
	int result;

	/* TODO: move this whole code block to a distinct function */
	result = ecell_button_ewindow(buffer, length);
	log_message(FIL, amora->log, "ecell_button = %d", result);
	if (result == NONE) {

		result = ecell_mouse_ewindow(buffer, length);
		log_message(FIL, amora->log, "ecell_mouse = %d", result);
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
			mouse_click(result, 0, amora->display);
			break;
		case MOUSE_SCROLL_DOWN:
			mouse_click(result, 0, amora->display);
			break;
		case NONE:
			log_message(FIL, amora->log, "mouse_event = %d", mouse_event);
			if (mouse_event == 1) {
				if (times == 0) {
					x_mouse = atoi(buffer);
					++times;
				} else {
					y_mouse = atoi(buffer);

					log_message(FIL, amora->log,
						    "x = %d\ty=%d\t",
						    x_mouse, y_mouse);

					result = mouse_move(x_mouse, y_mouse,
							      amora->display);
					if (result == -1)
						log_message(FIL|OUT, amora->log,
							    "Can't"
							    "move mouse!");

					times = 0;
					mouse_event = 0;
				}
			} else {
				result = treat_command(amora, buffer, length,
						       client_socket);
				if (result == CONN_CLOSE) {
					mouse_event = 0;
					times = 0;
					goto exit;
				} else if (result == NONE)
					log_message(FIL|OUT, amora->log,
						    "Invalid event!");
			}
			break;
		default:
			if (button_right)
				mouse_click(MOUSE_BUTTON_RIGHT, result,
					    amora->display);
			else if (button_left)
				mouse_click(MOUSE_BUTTON_LEFT, result,
					    amora->display);
			else if (button_middle)
				mouse_click(MOUSE_BUTTON_MIDDLE, result,
					    amora->display);
			break;
		}
		goto exit;
	}

	if (result != NONE) {
		send_event(KeyPress, x_key_code[result], amora->display);
		send_event(KeyRelease, x_key_code[result], amora->display);
		goto exit;
	}

exit:
	return result;

}


int treat_command(struct amora_s *amora, char *buffer, int length, int client_socket)
{

	static int do_capture = 0, screen_rotate = 0,
		width = 176, height = 208, flag = 0, times = 0;
	int result, tmp;
	char *tmpname, *tmpdir;
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
		tmp = screen_capture(amora->display, &image);
		if (tmp) {
			perror("failed screen capture: cap!\n");
			result = NONE;
			break;
		}

		tmp = rescale_image(&image, width, height, &rescaled);
		imlib_free_image();
		if (tmp) {
			perror("failed screen capture: rescale!\n");
			result = NONE;
			break;
		}

		if (screen_rotate) {
			tmp = rotate_image(&rescaled);
			if (tmp) {
				imlib_free_image();
				perror("failed screen capture: rotate!\n");
				result = NONE;
				break;
			}
		}

		tmpdir = malloc(MAXPATHLEN);
		strncpy(tmpdir, "/tmp/amora-XXXXXX", MAXPATHLEN);
		tmp = mkdtemp(tmpdir);
		if (tmp == NULL) {
			perror(sprintf("Error: Cannot create directory %s: %s\n",
				       tmpdir, strerror(errno)));
			result = NONE;
			break;
		} else {
			tmpname = malloc(MAXPATHLEN);
			strncpy(tmpname, tmpdir, MAXPATHLEN);
			strncat(tmpname, "/screenshot.png", MAXPATHLEN);

			tmp = save_image(&rescaled, tmpname);
			imlib_free_image();

			if (tmp) {
				perror("Error: Failed screen capture: freeing!\n");
				result = NONE;
				unlink(tmpname);
				rmdir(tmpdir);
				break;
			}

			tmp = send_file(client_socket, tmpname);


			if (unlink(tmpname)) {
				perror(sprintf("Error: Cannot remove file %s: %s\n",
					       tmpname, strerror(errno)));
			}
			if (rmdir(tmpdir)) {
				perror(sprintf("Error: Cannot remove directory %s: %s\n",
					       tmpdir, strerror(errno)));
			}

			if (tmp) {
				perror("failed screen transfer!\n");
				result = CONN_CLOSE;
				break;
			}
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

int client_socket_cb(void *context, int client_socket)
{
	int res;
	struct amora_s *amora;

	amora = (struct amora_s *)context;
	res = process_events(amora, client_socket, 0);

	if (res == CONN_CLOSE) {
		log_message(FIL, amora->log,"Client asked to close connection.");
		goto exit_close;
	}

	if (res == -1) {
		log_message(FIL, amora->log, "Client died or closed connection.");
		goto exit_close;
	}

	return 0;

exit_close:
	loop_remove(client_socket);
	close(client_socket);
	if (amora->disconn_callback)
		amora->disconn_callback(amora->client_name);
	return 0;

}

int server_socket_cb(void *context, int server_socket)
{
	char buffer[20];
	struct amora_s *amora;
	struct sockaddr rem_addr;
	unsigned int opt = sizeof(struct sockaddr);
	int client_socket;

	amora = (struct amora_s *)context;
	memset(&rem_addr, 0, sizeof(struct sockaddr));

	client_socket = accept(server_socket,
			(struct sockaddr *) &rem_addr, &opt);

	if (client_socket == -1)
		log_message(FIL|OUT, amora->log, "Failed opening connection,"
				" exiting...");
	else {
		client_bluetooth_id(&rem_addr, buffer);
		client_bluetooth_name(amora->device_socket, &rem_addr,
						      amora->client_name);
		log_message(FIL, amora->log, "Accepted connection. Client"
				" is %s [%s]", amora->client_name, buffer);

		if (amora->conn_callback)
			amora->conn_callback(amora->client_name);
		loop_add(client_socket, context, client_socket_cb);
	}

	return client_socket < 0 ? -1 : 0;
}

struct amora_s *amora_context_new(char *logfile, int channel, int hci_device)
{
	struct amora_s *result = NULL;
	int res;

	result = (struct amora_s *) malloc(sizeof(struct amora_s));
	if (!result)
		goto exit;

	memset(result, 0, sizeof(struct amora_s));

	if (logfile) {
		if (!(result->log = log_build_resources(logfile))) {
			perror("Failed log resource creation!");
			goto cleanup;
		}
	} else
		result->log = NULL;


	if (check_device() < 0) {
		log_message(FIL|OUT, result->log, "No bluetooth device/dongle "
			    "available. Aborting...");
		goto cleanup;
	}

	if ((0 < channel) && (channel < 20))
		result->channel = channel;
	else
		result->channel = 16;

	result->display = construct_display(NULL);
	if (!result->display) {
		log_message(FIL|OUT, result->log, "Error creating display "
			    "object! Aborting...");
		goto cleanup;
	}

	/* Service description registering */
	result->sd = build_sd(result->channel);
	if (!result->sd) {
		log_message(FIL|OUT, result->log, "Error creating service "
			    "description object! Aborting...");
		goto destroy;
	}

	res = describe_service(result->sd);
	if (res == -1) {
		log_message(FIL|OUT, result->log, "Error registering service!"
				"Aborting...");
		goto destroy;

	}

	/* Socket creation */
	result->sd->hci_id = hci_device;
	result->server_socket = build_bluetooth_socket(result->channel,
						       result->sd, &(result->device_socket));
	if (result->server_socket == -1) {
		log_message(FIL|OUT, result->log, "Failed creating bluetooth "
			    "conn! Exiting...");
		goto destroy;
	}

	snprintf(result->hci_str, sizeof(result->hci_str), "hci%d",
		 result->sd->hci_id);
	log_message(FIL, result->log, "Bluetooth device code hci = %d",
		    result->sd->hci_id);

#ifdef HAVE_DBUS
	if (dbus_init(result->hci_str)) {
		log_message(FIL|OUT, result->log, "Error while initilizing "
				"D-Bus! Aborting...");
		goto destroy;
	}
#endif

	loop_add(result->server_socket, result, server_socket_cb);
	log_message(FIL|OUT, result->log, "Initialization done.");
	goto exit;

destroy:

	if (result->display)
		destroy_display(result->display);
	if (result->sd)
		destroy_sd(result->sd);
	if (result->log)
		log_clean_resources(result->log);

cleanup:

	if (result) {
		free(result);
		result = NULL;
	}

exit:

	return result;
}


void amora_start(struct amora_s *context)
{
	int result;
	if (!context)
		return;

	log_message(FIL|OUT, context->log, "Waiting cellphone connection...");
	result = listen(context->server_socket, 10);
	if (result) {
		log_message(FIL|OUT, context->log, "Failed listening...");
		return;
	}

	log_message(FIL|OUT, context->log, "Entering main loop...");
	loop();
	log_message(FIL|OUT, context->log, "Done, we are exiting now.");
}


void amora_context_delete(struct amora_s *context)
{

	if (!context)
		return;

	/* FIXME: this call is required to cleanup static buffers */
	/* result = process_events(context, context->client_socket = 0, 1); */

	if (context->display)
		destroy_display(context->display);
	if (context->sd)
		destroy_sd(context->sd);

	if (context->server_socket != -1)
		close(context->server_socket);

	log_message(FIL|OUT, context->log, "Done, we are closing now.");
	if (context->log)
		log_clean_resources(context->log);

}


void amora_connection_callback(struct amora_s *context,
			       void (*conn_cb) (const char *device_name))
{
	if (!context || !conn_cb)
		return;

	context->conn_callback = conn_cb;
}

void amora_disconnection_callback(struct amora_s *context,
			       void (*conn_cb) (const char *device_name))
{
	if (!context || !conn_cb)
		return;

	context->disconn_callback = conn_cb;
}
