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
 * - new background/icon for connected status
 * - refactor main.c, its starting to get smelly
 * - screenshot from active window.
 * - catch SIGTERM or provide a way to clean exit (close sockets).
 * - forking the process when new connection is received can be a good idea.
 * - what about a heart beat between cellphone and server?
 */

/*  Copyright (C) 2007  Adenilson Cavalcanti <savagobr@yahoo.com>
 *
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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include "x11_event.h"
#include "bluecode.h"
#include "protocol.h"
#include "log.h"

/** Check for protocol commands in buffer, used by \ref process_events
 *
 *
 * @param buffer A string buffer with commands (i.e. CONN_CLOSE), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 *
 * @return 0 for closing the connection.  \todo Think how
 * to process other events.
 */
int treat_exit(char *buffer, int length);


/** Check for protocol commands, handle input events (mouse
 * and keyboard).
 *
 *
 * @param buffer A string buffer with commands (i.e. UP, DOWN), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param active_display Pointer to active display.
 * @param log A structure of log resources, see \ref log_resource.
 *
 * @return 0 for closing the connection.
 */
int treat_events(char *buffer, int length, Display *active_display,
		 struct log_resource *log);



/** Process event stream. Reads what new commands are being received
 * in socket and send them to X session.
 *
 * @param fd Socket file descriptor.
 * @param active_display Pointer to active display.
 * @param clean_up Free local allocated resources.
 * @param log A structure of log resources, see \ref log_resource.
 *
 * @return Number of bytes read on sucess, -1 on error, 0 on exit.
 */
int process_events(int fd, Display *active_display, int clean_up,
		   struct log_resource *log);



/** Main app function.
 *
 * @return 0 on sucess, -1 on error.
 */
int main(void)
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
	char buffer[length];


	memset(&rem_addr, 0, sizeof(struct sockaddr));

	/* TODO: check for environment variable */
	log = log_build_resources(NULL);

	if (check_device() < 0) {
		log_message(FIL|ERR, log, "No bluetooth device/dongle available."
				" Aborting...\n");
		return -1;
	}

	own_display = construct_display(NULL);
	if (!own_display) {
		log_message(FIL|ERR, log, "Error creating display object!"
				"Aborting...\n");
		return -1;
	}

	/* Service description registering */
	sd = build_sd(channel);
	if (!sd) {
		log_message(FIL|ERR, log, "Error creating service description"
			    "object!"
			    "Aborting...\n");
		return -1;
	}
	res = describe_service(sd);
	if (res == -1) {
		log_message(FIL|ERR, log, "Error registering service!"
			    "Aborting...\n");
		destroy_sd(sd);
		return -1;
	}


	/* Socket creation */
	server_socket = build_bluetooth_socket(channel);
	if (server_socket == -1) {
		log_message(FIL|ERR, log, "Failed creating bluetooth conn!"
			    "Exiting...\n");
		return -1;
	}

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
			log_message(FIL|ERR, log, "Failed opening connection,"
				    " exiting...\n");
			goto exit;
		}

		client_bluetooth_id(&rem_addr, buffer);
		log_message(FIL|OUT, log, "Accepted connection. Client is "
			    "%s\n", buffer);

		FD_ZERO(&fd_set_socket);
		FD_SET(client_socket, &fd_set_socket);
		time_socket.tv_sec = 5;
		time_socket.tv_usec = 0;

		while (select(1, &fd_set_socket, NULL, NULL,
			      &time_socket) != -1) {

			res = process_events(client_socket,
					     own_display, clean_up, log);
			if (!res) {
				log_message(FIL|OUT, log,
					    "Client asked to close "
					    "connection\n\n");
				close(client_socket);
				client_socket = -1;
				break;
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


int process_events(int fd, Display *active_display, int clean_up,
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
	if (bytes_read == -1)
		return result;

	start = buffer;
	while ((end = strchr(start, CMD_BREAK))) {
		result = treat_events(start, (end - start), active_display,
				      log);
		start = ++end;
	}

	return result;
}

int treat_events(char *buffer, int length, Display *active_display,
		 struct log_resource *log)
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
						log_message(FIL|ERR, log,
							    "Can't"
							    "move mouse!");

					times = 0;
					mouse_event = 0;
				}
			} else {

				result = treat_exit(buffer, length);
				if (result == 0) {
					mouse_event = 0;
					times = 0;
					goto exit;
				}

				log_message(FIL|ERR, log, "Invalid event!\n");
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


int treat_exit(char *buffer, int length) {

	int result = protocol_command(buffer, length);
	if (result != NONE) {

		switch (result) {

		case CONN_CLOSE:
			result = 0;
			break;
			/* TODO: add server stop code */
		case SERVER_STOP:
			break;
			/* TODO: add image handling/screenshot code */
		case RESOLUTION:
			break;
			/* TODO: add image handling/screenshot code */
		case IMG_FORMAT:
			break;

		}

	}

	return result;

}

