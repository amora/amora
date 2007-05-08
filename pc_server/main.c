/**
 * @file   main.c
 * @author Adenilson Cavalcanti
 * @date   Fri May  4 14:25:09 2007
 *
 * @brief  Main server side presenter app.
 *
 * This app is reponsible to receive events from cellphone (in
 * near future...) and send them to X Server.
 * \todo
 * - screenshot from active window.
 * - autotools buildsystem.
 * - logging feature.
 * - catch SIGTERM or provide a way to clean exit (close sockets).
 */

#include <stdio.h>
#include <unistd.h>
#include "x11_event.h"
#include "protocol.h"
#include "bluecode.h"

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
	int bytes_read = 0, server_socket, client_socket, channel = 1, res;
	fd_set fd_set_socket;
	struct timeval time_socket;
	struct sockaddr_rc rem_addr = { 0 };
	unsigned int opt = sizeof(rem_addr);
	const int BUF_SIZE = 300;
	char *buf = NULL;

	/* Heap memory is easier to audit */
	buf = malloc(BUF_SIZE);
	if (!buf) {
		perror("Buffer memory allocation failed: ");
		return -1;
	}

	own_display = construct_display(NULL);
	if (!own_display) {
		printf("Error creating display object! Aborting...\n");
		return -1;
	}

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
		client_socket = accept(server_socket, (struct sockaddr *)&rem_addr, &opt);

		FD_ZERO(&fd_set_socket);
		FD_SET(client_socket, &fd_set_socket);
		time_socket.tv_sec = 5;
		time_socket.tv_usec = 0;
			while (select(1, &fd_set_socket, NULL, NULL, &time_socket) != -1) {
				ba2str(&rem_addr.rc_bdaddr, buf );
				fprintf(stderr, "accepted connection from %s\n", buf);

				bytes_read = read_socket(client_socket, buf, BUF_SIZE);
				printf("%s\t%d\n", buf, bytes_read);
				/* Remember that there is the CMD_BREAK character at end of
				 * command!
				 */
				--bytes_read;
				res = ecell_convert_ewindow(buf, bytes_read);
				if (res == NONE)
					printf("Invalid event!\n");
				else
					send_event(KeyPress, x_key_code[res], own_display);
			}

		close(client_socket);

	}

	res = destroy_display(own_display);
	printf("Done, we are closing now.\n");
	close(server_socket);

	return 0;
}
