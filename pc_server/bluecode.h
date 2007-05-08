/**
 * @file   bluecode.h
 * @author Adenilson Cavalcanti
 * @date   Tue May  8 09:12:06 2007
 *
 * @brief  Bluetooth handling code will stay here.
 *
 * My purpose is support any X enabled O.S. (Linux, Solaris, FreeBSD, MacOSX),
 * I believe that each one has a different implementation for bluetooth stack
 * but hope that all them maintain the idea of sockets/fd to write and read
 * data from bluetooth.
 */
#ifndef __BLUECODE_PC__
#define __BLUECODE_PC__

#include "protocol.h"

/** TODO: how are I going to handle multiple platforms here? Maybe put
 * platform dependent code in .c files?
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

/** Read and parse commands from socket.
 *
 *
 * @param client The socket open to client cell phone.
 * @param data Pointer to buffer to write data.
 * @param length Buffer size.
 *
 * @return Number of bytes read or 0 in error case.
 */
int read_socket(int client, char *data, int length)
{
	int res = 0, tmp;
	memset(data, 0, length);

	while (!strchr(data, CMD_BREAK) && (res < length)) {
		tmp = read(client, data, length);
		res += tmp;
		length -= res;
	}

	return res;
}

/** Initialize a socket to read/write data from bluetooth channel.
 *
 *
 * @param channel Number of channel to be used to export the server service.
 *
 * @return Socket in sucess, -1 otherwise.
 */
int build_bluetooth_socket(unsigned int channel)
{
	struct sockaddr_rc loc_addr = { 0 };
	int s, res = -1;

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (s == -1)
		goto error;

	/* FIXME: use SDP in future */
	loc_addr.rc_family = AF_BLUETOOTH;
	/* TODO: how to test for available bluetooth dongles? */
	loc_addr.rc_bdaddr = *BDADDR_ANY;

	loc_addr.rc_channel = (uint8_t) channel;
	res = bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
	if (res == -1)
		goto close;

	goto exit;

close:
	fprintf(stderr, "closing socket...\n");
	close(s);
	s = -1;

error:
	fprintf(stderr, "build_bluetooth_socket error!\n");

exit:
	return s;
}

#endif
