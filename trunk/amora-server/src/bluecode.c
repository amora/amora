/**
 * @file   bluecode.c
 * @author teste
 * @date   Thu Dec  6 18:14:25 2007
 *
 * @brief  POSIX/ANSIC code for communication.
 *
 * Source code for communication common between the *nix flavors goes
 * here.
 * \todo
 * - remove unused includes
 *
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


/** strnlen is GNU extension, this makes the compiler happy. */
#define _GNU_SOURCE

#include "bluecode.h"
#include "protocol.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp_lib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
/** Linux sendfile is failing (probably I'm not using it correctly). */
#define STRANGE_BUG

#ifndef STRANGE_BUG
#include <sys/sendfile.h>
#endif


struct service_description *build_sd(int channel)
{
	struct service_description *sd;
	sd = (struct service_description *)
		malloc(sizeof(struct service_description));

	if (!sd)
		goto mem_error;

	sd->channel = channel;
	sd->uuid = 0xAFFF;
	sd->service_name = strdup("Amora: assistant");
	sd->service_dsc = strdup("A X handler for cellphone");
	sd->service_prov = strdup("Savago");

	if (sd->service_name && sd->service_dsc && sd->service_prov)
		goto exit;

mem_error:
	perror("build_sd: failed service allocation structure!");
	if (sd)
		free(sd);
	sd = NULL;

exit:

	return sd;
}

int read_socket(int client, char *data, int length)
{
	int res = 0, tmp;
	memset(data, 0, length);
	--length;

	while (!strchr(data, CMD_BREAK) && (res < length)) {
		tmp = read(client, data, length);

		if (tmp == -1)
			return tmp;

		res += tmp;
		length -= res;
	}
	/* Adding ending of string */
	data[res] = '\0';
	return res;
}


int send_file(int client_socket, char *filename)
{
	struct stat src_stat;
	int source, result = -1;
#ifndef STRANGE_BUG
	off_t offset = 0;
#endif
	source = open(filename, O_RDONLY);
	if (source == -1) {
		perror("send_file: failed opening source file!\n");
		goto exit;
	}

	fstat(source, &src_stat);
#ifndef STRANGE_BUG
	result = sendfile(client_socket, source, &offset, src_stat.st_size);
#else
	result = hack_send_file(client_socket, source, src_stat);
#endif
	if (result != src_stat.st_size) {
		perror("send_file: failed sending file to client!\n");
		result = -1;
	} else
		result = 0;

	close(source);

exit:
	return result;
}

int hack_send_file(int client_socket, int file_descriptor, struct stat mstat)
{
	int bytes, res, readen, tmp1, tmp2;
	int length = 8192;
	char buffer[length];
	int byte_count;

	bytes = 0;
	snprintf(buffer, (length - 1), "%d", (int)mstat.st_size);
	byte_count = strnlen(buffer, length);
	res = write(client_socket, buffer, byte_count);
	if (res == -1) {
		printf("Error sending image file size!\n");
		return -1;
	}

	while (bytes < mstat.st_size) {

		readen = read(file_descriptor, buffer, length);
		res = write(client_socket, buffer, readen);

		if (res == -1)
			goto error;

		/* Contention required for non blocking socket */
		tmp1 = res;
		while (tmp1 < readen) {
			tmp2 = write(client_socket, (buffer + tmp1), readen - tmp1);
			if (tmp2 > 0)
				tmp1 += tmp2;
			else if (tmp2 == 0)
				usleep(80000);
			else if (tmp2 == -1)
				goto error;

		}
		res = tmp1;
		bytes += res;

	}

	return bytes;

error:

	printf("Error reading image file!\n");
	return -1;
}
