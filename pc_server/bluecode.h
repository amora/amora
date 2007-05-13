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
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

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
struct service_description *build_sd(int channel)
{
	struct service_description *sd;
	sd = (struct service_description *) malloc(sizeof(struct service_description));
	if (!sd)
		goto mem_error;

	sd->channel = channel;
	sd->uuid = 0xAFFF;
	sd->service_name = strdup("Presenter 4 X ");
	sd->service_dsc = strdup("A X handler for cellphone ");
	sd->service_prov = strdup("Savago ");

	if (sd->service_name && sd->service_dsc && sd->service_prov)
		goto exit;

mem_error:
	printf("build_sd: failed service allocation structure!");
	if (sd)
		free(sd);
	sd = NULL;

exit:

	return sd;
}


/** This function frees resources allocated in service description structure.
 *
 * It works like an object destructor.
 *
 * @param sd Service description pointer.
 */
void destroy_sd(struct service_description *sd)
{

	free(sd->service_name);
	free(sd->service_dsc);
	free(sd->service_prov);
	sdp_close((sdp_session_t *)sd->session);
}

/** Read and parse commands from socket.
 *
 * It uses a new line as command terminating character, see \ref CMD_BREAK.
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
	/* Adding ending of string */
	data[res] = '\0';
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
int describe_service(struct service_description *sd) {


	uint32_t service_uuid_int[] = { 0, 0, 0, sd->uuid };
	uint8_t rfcomm_channel = sd->channel;
	const char *service_name = sd->service_name;//"Roto-Rooter Data Router";
	const char *service_dsc = sd->service_dsc;//"An experimental plumbing router";
	const char *service_prov = sd->service_prov;//"Roto-Rooter";

	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;
	sdp_list_t *l2cap_list = 0, *rfcomm_list = 0, *root_list = 0, *proto_list = 0,
	  *access_proto_list = 0,  *svclass_id = 0, *profiles = 0;

	sdp_data_t *channel = 0;
	sdp_record_t *record = sdp_record_alloc();

	int err = 0;
	sdp_session_t *session = 0;
	sdp_profile_desc_t profile;

	// set the general service ID
	sdp_uuid128_create(&svc_uuid, &service_uuid_int);
	sdp_set_service_id(record, svc_uuid);

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(record, root_list);

	//Set port attributes
	sdp_uuid16_create(&root_uuid, SERIAL_PORT_SVCLASS_ID);
	svclass_id = sdp_list_append(0, &root_uuid);
	sdp_set_service_classes(record, svclass_id);

	sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
	profile.version = 0x0100;
	profiles = sdp_list_append(0, &profile);
	sdp_set_profile_descs(record, profiles);


	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append(0, &l2cap_uuid);
	proto_list = sdp_list_append(0, l2cap_list);

	// set rfcomm information
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
	rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
	sdp_list_append(rfcomm_list, channel);
	sdp_list_append(proto_list, rfcomm_list);

	// attach protocol information to service record
	access_proto_list = sdp_list_append(0, proto_list);
	sdp_set_access_protos(record, access_proto_list);

	// set the name, provider, and description
	sdp_set_info_attr(record, service_name, service_prov, service_dsc);

	// connect to the local SDP server, register the service record, and
	// disconnect
	session = sdp_connect(BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
	err = sdp_record_register(session, record, 0);
	if (err == -1) {
		printf("describe_service: error registering service!");
	}

	// cleanup
	sdp_data_free(channel);
	sdp_list_free(l2cap_list, 0);
	sdp_list_free(rfcomm_list, 0);
	sdp_list_free(root_list, 0);
	sdp_list_free(access_proto_list, 0);
	sdp_list_free(svclass_id, 0);
	sdp_list_free(profiles, 0);
	sd->session = (void*) session;

	return err;
}



#endif
