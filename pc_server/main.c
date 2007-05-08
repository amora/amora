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
 * - communicate with cellphone using BlueZ.
 * - screenshot from active window.
 * - autotools buildsystem.
 * - logging feature.
 */

#include <stdio.h>
#include <unistd.h>
#include "x11_event.h"
#include "protocol.h"
#include "bluecode.h"

/** Tests for forward key event.
 *
 *
 * @param active_display Pointer to active display.
 * @param times How many times to advance (send event right arrow).
 */
void foward_test(Display *active_display, int times);

/** Tests for backward key event.
 *
 *
 * @param active_display Pointer to active display.
 * @param times How many times to advance (send event left arrow).
 */
void backward_test(Display *active_display, int times);


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
	while (listen(server_socket, 1) == 0) {

		client_socket = accept(server_socket, (struct sockaddr *)&rem_addr, &opt);
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

	res = destroy_display(own_display);
	printf("Done, we are closing now.\n");
	return 0;
}
