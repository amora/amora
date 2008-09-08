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

/** Callback called by main loop, prints the client name
 * to terminal.
 *
 * @param msg It will have the mobile client address.
 */
static void client_conn_name(const char *msg);

/** Callback called by main loop when a client disconnects.
 *
 * @param msg It will have the mobile client address.
 */
static void client_disconn_name(const char *msg);


/** Main app function.
 *
 * @param argc argument counter
 * @param argv argument vector
 *
 * @return 0 on success, -1 on error.
 */
int main(int argc, char **argv)
{
	int hci = -1;
	struct amora_s *amora;
	char arg;
	char *logfile = NULL;
	int channel = 16;

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

	amora = amora_context_new(logfile, channel, hci);
	if (!amora)
		return -1;

	amora_connection_callback(amora, client_conn_name);
	amora_disconnection_callback(amora, client_disconn_name);
	amora_start(amora);
	amora_context_delete(amora);

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


static void client_conn_name(const char *msg)
{
	if (msg)
		fprintf(stderr, "Client is: %s\n", msg);

}

static void client_disconn_name(const char *msg)
{
	if (msg)
		fprintf(stderr, "Client is: %s\n", msg);
	else
		fprintf(stderr, "Client disconnection, but I don't have the"
			" device name!\n");
}
