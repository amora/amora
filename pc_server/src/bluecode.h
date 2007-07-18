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

#ifndef __BLUECODE_PC__
#define __BLUECODE_PC__


//#include <sys/types.h>
#include <sys/socket.h>

/** Service description structure.
 *
 * Use it to hold bluetooth service description, tell what the service does
 * and which port/channel it runs.
 */
struct service_description {
	/** Channel/port number. */
	unsigned char channel;
	/** ID */
	unsigned int uuid;
	/** Name of service, it appears for devices that connects with the
	 * service
	 */
	char *service_name;
	/** Description of service. */
	char *service_dsc;
	/** The service provider, who is the one responsible for the app. */
	char *service_prov;
	/** Session's pointer, I used pointer to void to abstract the type,
	 * since each O.S. probably will have a different struct/object
	 * to represent daemon bluetooth session.
	 */
	void *session;
};

/** This function builds a service description structure.
 *
 * It works like a constructor for service description structure, see
 * \ref service_description. For while I only exported the channel interface
 * when creating the structure, maybe in future I will let the user set
 * other details while creating the structure.
 *
 * @param channel Number of channel/port where service will run.
 *
 * @return Pointer to bluetooth service description.
 */
struct service_description *build_sd(int channel);


/** This function frees resources allocated in service description structure.
 *
 * It works like an object destructor.
 *
 * @param sd Service description pointer.
 */
void destroy_sd(struct service_description *sd);


/** Read and parse commands from socket.
 *
 * It uses a new line as command terminating character, see CMD_BREAK in
 * source file \ref protocol.h.
 *
 * @param client The socket open to client cell phone.
 * @param data Pointer to buffer to write data.
 * @param length Buffer size.
 *
 * @return Number of bytes read or 0 in error case.
 */
int read_socket(int client, char *data, int length);


/** Initialize a socket to read/write data from bluetooth channel.
 *
 *
 * @param channel Number of channel to be used to export the server service.
 *
 * @return Socket in sucess, -1 otherwise.
 */
int build_bluetooth_socket(unsigned int channel);


/** This functions talks with bluetooth daemon and registers the service
 * with it.
 *
 * Beware... here goes the gory details of service registration. I'snt there
 * an easier way to do this stuff?
 *
 * @param sd Service description pointer structure.
 *
 * @return 0 on sucess, -1 otherwise.
 */
int describe_service(struct service_description *sd);


/** Translated remote client address to a more human readable
 * format. It gives the MAC address of bluetooth client device.
 *
 * @param client_address Socket address peer socket pointer.
 * @param buffer A string buffer to hold translated address.
 */
void client_bluetooth_id(struct sockaddr *client_address, char *buffer);

#endif
