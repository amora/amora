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
 * - a 'stop/close' command in protocol.
 * - screenshot from active window.
 * - autotools buildsystem.
 * - logging feature.
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
#include "x11_event.h"
#include "bluecode.h"
#include "protocol.h"


/** Process event stream. Reads what new commands are being received
 * in socket and send them to X session.
 *
 * @param fd Socket file descriptor.
 * @param active_display Pointer to active display.
 * @param clean_up Free local allocated resources.
 *
 * @return Number of bytes read on sucess, -1 on error.
 */
int process_events(int fd, Display *active_display, int clean_up);

/** Main app function.
 *
 *
 * @param argc Number of arguments.
 * @param argv Arguments values.
 *
 * @return 0 on sucess, -1 on error.
 */
int main(int argc, char* argv[])
{
	Display *own_display = NULL;
	int server_socket, client_socket, channel = 10, res;
	int clean_up = 0;
	fd_set fd_set_socket;
	struct timeval time_socket;
	struct sockaddr rem_addr = { 0 };
	unsigned int opt = sizeof(rem_addr);
	struct service_description *sd = NULL;

	own_display = construct_display(NULL);
	if (!own_display) {
		printf("Error creating display object! Aborting...\n");
		return -1;
	}

	/* Service description registering */
	sd = build_sd(channel);
	if (!sd) {
		printf("Error creating service description object!"
		       "Aborting...\n");
		return -1;
	}
	res = describe_service(sd);
	if (res == -1) {
		printf("Error registering service! Aborting...\n");
		destroy_sd(sd);
		return -1;
	}


	/* Socket creation */
	server_socket = build_bluetooth_socket(channel);
	if (server_socket == -1) {
		printf("Failed creating bluetooth conn! Exiting...\n");
		return -1;
	}

	printf("\nInitialization done, waiting cellphone connection...\n");
	res = listen(server_socket, 1);
	if (res) {
		printf("Failed listening...\n");
		return -1;
	}

	while (1) {

		printf("Entering main loop...\n");
		client_socket = accept(server_socket,
				       (struct sockaddr *)&rem_addr, &opt);
		printf("Accepted connection.\n");
		FD_ZERO(&fd_set_socket);
		FD_SET(client_socket, &fd_set_socket);
		time_socket.tv_sec = 5;
		time_socket.tv_usec = 0;
			while (select(1, &fd_set_socket, NULL, NULL,
				      &time_socket) != -1) {
				//TODO: move this code to bluetooth
// 				ba2str(&rem_addr.rc_bdaddr, buf);
// 				fprintf(stderr, "accepted connection from %s\n", buf);
				res = process_events(client_socket,
						     own_display, clean_up);
#ifdef VERBOSE
				printf("Read bytes: %d\n", res);
#endif
			}

		close(client_socket);

	}

	res = destroy_display(own_display);
	printf("Done, we are closing now.\n");
	close(server_socket);
	destroy_sd(sd);
	res = process_events(client_socket = 0, own_display = NULL,
			     clean_up = 1);

	return 0;
}


int process_events(int fd, Display *active_display, int clean_up)
{
	static char *buffer = NULL;
	const int BUF_SIZE = 300;
	static unsigned char mouse_event = 0, times = 0;
	static int x_mouse, y_mouse;
	int bytes_read, result;

	/* Call to just cleanup local allocated memory. */
	if ((clean_up == 1) && (fd == 0) && (active_display == NULL)) {
		free(buffer);
		buffer = NULL;
		return 0;
	}

	/* Heap memory is easier to audit */
	if (!buffer) {
		buffer = malloc(BUF_SIZE);
		if (!buffer) {
			perror("Buffer memory allocation failed: ");
			return -1;
		}
	}

	bytes_read = read_socket(fd, buffer, BUF_SIZE);

	/* Remember that there is the CMD_BREAK character at end of
	 * command!
	 */
	--bytes_read;
	result = ecell_button_ewindow(buffer, bytes_read);

	/* TODO: move this code to a distinct function */
	if (result == NONE) {
		result = ecell_mouse_ewindow(buffer, bytes_read);

		switch (result) {
		case MOUSE_MOVE:
			mouse_event = 1;
			break;
		case MOUSE_BUTTON_RIGHT:
			/* TODO: add mouse event X code */
			break;
		case MOUSE_BUTTON_LEFT:
			/* TODO: add mouse event X code */
			break;
		case NONE:
			if (mouse_event == 1) {
				if (times == 0) {
					x_mouse = atoi(buffer);
					++times;
				} else {
					y_mouse = atoi(buffer);
#ifdef VERBOSE
					printf("\nx = %d\ty=%d\t\n\n",
					       x_mouse, y_mouse);
#endif
					result = mouse_move(x_mouse, y_mouse,
							      active_display);
					if (result == -1)
						printf("Can't move mouse!");

					times = 0;
					mouse_event = 0;
				}
			} else
				printf("Invalid event!\n");
		}
	} else
		send_event(KeyPress, x_key_code[result], active_display);


	return bytes_read;
}
