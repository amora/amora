/**
 * @file   bluetooth-bluez-dbus.c
 * @author Thiago Marcos P. Santos
 * @date   $LastChangedDate$
 *
 * @brief  BlueZ with D-Bus functions
 *
 */

/*  Copyright (C) 2008  Thiago Marcos P. Santos <tmpsantos@gmail.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <dbus/dbus.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bluetooth-bluez-dbus.h"
#include "dbus.h"
#include "loop.h"


/** D-Bus signal handler
 *
 * Function called when a signal that match the pre-defined
 * rules arrives.
 *
 * @param connection D-Bus connection
 * @param msg the arrived message
 * @param data userdata (hci device number, i.e. "hci0")
 *
 * @return DBUS_HANDLER_RESULT_{HANDLED,NOT_YET_HANDLED}
 *
 */
static DBusHandlerResult signal_handler(DBusConnection *conn,
		DBusMessage *msg, void *data)
{
	const char *path, *hci_id = (char *) data;
	char *basec, *bname;

	(void) conn;

	if (dbus_message_is_signal(msg, "org.bluez.Manager", "AdapterRemoved")) {
		dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING,
				&path, DBUS_TYPE_INVALID);

		/* basename() systax is fscking ugly */
		basec = strdup(path);
		bname = basename(basec);

		if (strcmp(bname, hci_id) == 0) {
			/* We shall disable bt frontend here */
			printf("dongle_removed = %s\n", hci_id);
		}

		free(basec);
	}

	return DBUS_HANDLER_RESULT_HANDLED;
}


int dbus_bt_dongle_removed_watch(DBusConnection *conn, char * hci_id)
{
	DBusError error;
	int ret = -1;

	if (!dbus_connection_add_filter(conn, signal_handler, (void *) hci_id, NULL))
		goto out;

	dbus_bus_add_match(conn, "type='signal',interface='org.bluez.Manager',"
			"member='AdapterRemoved'", &error);

	if (dbus_error_is_set(&error))
		dbus_error_free(&error);
	else
		ret = 0;

out:
	return ret;
}

