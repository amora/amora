/**
 * @file   dbus.c
 * @author Thiago Marcos P. Santos
 * @date   $LastChangedDate$
 *
 * @brief  D-Bus related functions
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

#include "dbus.h"
#include "loop.h"

#ifdef DBUS_COMPAT_MODE
#define dbus_watch_get_unix_fd dbus_watch_get_fd
#endif


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
	DBusConnection *conn = (DBusConnection *) data;

	(void) fd;

	/* Shall never block, I hope */
	dbus_connection_read_write(conn, -1);

	while (dbus_connection_dispatch(conn) == DBUS_DISPATCH_DATA_REMAINS);

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


DBusConnection *dbus_init(void)
{
	DBusError error;
	DBusConnection *conn;

	dbus_error_init(&error);
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if (conn == NULL)
		goto out;

	dbus_connection_set_watch_functions(conn, add_watch,
			remove_watch, toggle_watch, (void *) conn, NULL);

	/* Flush outgoing queue */
	dbus_connection_flush(conn);

out:
	if (dbus_error_is_set(&error)) {
		dbus_error_free(&error);

		/* FIXME: This is a leak? */
		conn = NULL;
	}

	return conn;
}

