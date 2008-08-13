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
 * - use mkdtemp() when storing temporary screenshot
 * - catch SIGTERM or provide a way to clean exit (close sockets).
 * - add utests using 'check' library
 *
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

#include "config.h"

#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#include "amora.h"
#include "x11_event.h"
#include "bluecode.h"
#include "protocol.h"
#include "dbus.h"
#include "log.h"
#include "loop.h"
#include "imscreen.h"

/** Show program usage
 *
 * @param path the program binary path
 *
 */
static void show_usage(const char *path);


/** Main app function.
 *
 * @param argc argument counter
 * @param argv argument vector
 *
 * @return 0 on success, -1 on error.
 */
int main(int argc, char **argv)
{
	int res, hci = -1;
	struct amora_s amora;
	char arg, hci_str[6];
	const char *logfile = NULL;

	if (argc > 5) {
		show_usage(argv[0]);
		return -1;
	}

	while ((arg = getopt(argc, argv, "l:i:hv")) != -1) {
		switch (arg) {
			case 'l':
				logfile = optarg;
				break;
			case 'h':
				show_usage(argv[0]);
				return 0;
			case 'v':
				printf("%s (build %s)\n", PACKAGE_STRING,
						BUILDVERSION);
				return 0;
			case 'i':
			        hci = atoi(optarg);
				break;
			default:
				return -1;
		}
	}

	if (!(amora.log = log_build_resources(logfile)))
		perror("Failed log resource creation!");

	if (check_device() < 0) {
		log_message(FIL|OUT, amora.log, "No bluetooth device/dongle available."
				" Aborting...");
		return -1;
	}

	amora.channel = 16;
	amora.display = construct_display(NULL);
	if (!amora.display) {
		log_message(FIL|OUT, amora.log, "Error creating display object!"
				"Aborting...");
		return -1;
	}

	/* Service description registering */
	amora.sd = build_sd(amora.channel);
	if (!amora.sd) {
		log_message(FIL|OUT, amora.log, "Error creating service description"
				"object! Aborting...");
		return -1;
	}
	res = describe_service(amora.sd);
	if (res == -1) {
		log_message(FIL|OUT, amora.log, "Error registering service!"
				"Aborting...");
		destroy_sd(amora.sd);
		return -1;
	}

	/* Socket creation */
	amora.sd->hci_id = hci;
	amora.server_socket = build_bluetooth_socket(amora.channel, amora.sd);
	if (amora.server_socket == -1) {
		log_message(FIL|OUT, amora.log, "Failed creating bluetooth conn!"
				"Exiting...");
		return -1;
	}

#ifdef HAVE_DBUS
	if (dbus_init(hci_str)) {
		log_message(FIL|OUT, amora.log, "Error while initilizing "
				"D-Bus! Aborting...");
		return -1;
	}
#endif

	loop_add(amora.server_socket, &amora, server_socket_cb);

	snprintf(hci_str, sizeof(hci_str), "hci%d", amora.sd->hci_id);

	log_message(FIL, amora.log, "Bluetooth device code hci = %d", amora.sd->hci_id);
	log_message(FIL|OUT, amora.log, "Initialization done, waiting cellphone"
			" connection...");

	res = listen(amora.server_socket, 10);
	if (res) {
		log_message(FIL|OUT, amora.log, "Failed listening...");
		return -1;
	}


	log_message(FIL|OUT, amora.log, "Entering main loop...");
	loop();

	res = destroy_display(amora.display);
	amora.display = NULL;
	log_message(FIL|OUT, amora.log, "Done, we are closing now.");
	close(amora.server_socket);
	destroy_sd(amora.sd);
	res = process_events(&amora, amora.client_socket = 0, 1);
	log_clean_resources(amora.log);

	return 0;
}

static void show_usage(const char *path)
{
	char *name, *p = strdup(path);

	name = basename(p);

	printf("Usage: %s [-l logfile] [-h] [-v] [-i hci_number]\n"
	       "\n"
	       "  -h             show this help message and exit;\n"
	       "  -l logfile     set the log file path (default is disabled);\n"
	       "  -v             show version and exit.\n"
	       "  -i hci_number  set the bluetooth dongle device to use\n"
	       "\n", name);

	free(p);
}

