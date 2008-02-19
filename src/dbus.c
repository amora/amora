/**
 * @file   dbus.c
 * @author Thiago Marcos P. Santos
 * @date   Tue Feb 12 00:07:16 AMT 2008
 *
 * @brief  D-Bus related functions.
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
#include <stdlib.h>
#include <string.h>

#include "dbus.h"
#include "loop.h"

#ifdef DBUS_COMPAT_MODE
#define dbus_watch_get_unix_fd dbus_watch_get_fd
#endif


/** Connection handler */
static DBusConnection *conn;

/* FIXME: find a better way to do this */
static int dongle_removed;


/** D-Bus file descriptor callback
 *
 * @param fd the file descriptor itself
 * @param data the userdata
 *
 * @return 0 on success, -1 otherwise
 *
 */
static int dispatch(int fd, void *data)
{
	(void) fd;
	(void) data;

	/* Shall never block, I hope */
	dbus_connection_read_write(conn, -1);

	while (dbus_connection_dispatch(conn) == DBUS_DISPATCH_DATA_REMAINS);

	if (dongle_removed)
		return -1;
	else
		return 0;
}


/** D-Bus add watch callback
 *
 * Function needed when not using glib main loop.
 *
 * @param watch D-Bus watch context
 * @param data userdata (unused by now)
 *
 * @return TRUE on success, FALSE otherwise
 *
 */
static dbus_bool_t add_watch(DBusWatch *watch, void *data)
{
	int flags;

	(void) data;

	if (!dbus_watch_get_enabled(watch))
		goto out;

	flags = dbus_watch_get_flags(watch);
	if (flags & DBUS_WATCH_READABLE)
		loop_add(dbus_watch_get_unix_fd(watch), dispatch, data);

out:
	return TRUE;
}


/** D-Bus remove watch callback
 *
 * Function needed when not using glib main loop.
 *
 * @param watch D-Bus watch context
 * @param data userdata (unused by now)
 *
 */
static void remove_watch(DBusWatch *watch, void *data)
{
	(void) data;

	loop_remove(dbus_watch_get_unix_fd(watch));
}


/** D-Bus toogle watch callback
 *
 * Functions needed when not using glib main loop.
 *
 * @param watch D-Bus watch context
 * @param data userdata (unused by now)
 *
 */
static void toggle_watch(DBusWatch *watch, void *data)
{
	/* Doesn't really matter in our scenario */
	if (dbus_watch_get_enabled(watch))
		remove_watch(watch, data);
	else
		add_watch(watch, data);
}


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
static DBusHandlerResult signal_handler(DBusConnection *connection,
		DBusMessage *msg, void *data)
{
	const char *path, *hci_id = (char *) data;
	char *basec, *bname;

	(void) connection;

	if (dbus_message_is_signal(msg, "org.bluez.Manager", "AdapterRemoved")) {
		dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING,
				&path, DBUS_TYPE_INVALID);

		/* basename() systax is fscking ugly */
		basec = strdup(path);
		bname = basename(basec);

		if (strcmp(bname, hci_id) == 0)
			dongle_removed = 1;

		free(basec);
	}

	return DBUS_HANDLER_RESULT_HANDLED;
}


int dbus_init(char *hci_id)
{
	DBusError error;
	int ret = 0;

	dbus_error_init(&error);
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if (conn == NULL)
		goto out;

	if (!dbus_connection_add_filter(conn, signal_handler, (void *) hci_id, NULL))
		goto out;

	dbus_bus_add_match(conn, "type='signal',interface='org.bluez.Manager',"
			"member='AdapterRemoved'", &error);

	dbus_connection_set_watch_functions(conn, add_watch,
				remove_watch, toggle_watch, NULL, NULL);

	/* Flush outgoing queue */
	dbus_connection_flush(conn);

out:
	if (dbus_error_is_set(&error)) {
		dbus_error_free(&error);
		ret = -1;
	}

	return ret;
}

