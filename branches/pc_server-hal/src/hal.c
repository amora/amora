/**
 * @file   hal.c
 * @author Thiago Marcos P. Santos 
 * @date   Fri May  4 14:25:09 2007
 *
 * @brief  HAL hardware detection system support
 *
 * HAL is being used to detect bluetooth dongle removal. This code was
 * inspired in Gnome Tracker battery detection.
 *
 */

/*  Copyright (C) 2007 Thiago Marcos P. Santos <tmpsantos@gmail.com>
 *                     Gnome Tracker Team <tracker-list@gnome.org>
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

#include <dbus/dbus.h>
#include <libhal.h>
#include <stdio.h>
#include <string.h>

#include "hal.h"


/** Bluetooh identifier */
static char *bluetooth_udi;


/** Print error messages to stderr
 *
 * @param message the error message
 * @param error DBUS error or NULL for simple messages.
 *
 */
static void print_error(const char *message, const DBusError *error)
{
	if (error && dbus_error_is_set(error))
		fprintf(stderr, "%s: %s.\n", message, error->message);
	else
		fprintf(stderr, "%s.\n", message);
}


/** Initiate HAL/DBUS subsystem
 *
 * @param ctx HAL context
 * @param udi HAL unique hardware identifier
 *
 */
static void hal_removed_cb(LibHalContext *ctx, const char *udi) {
	(void) ctx;

	if (strcmp(bluetooth_udi, udi) == 0) {
		printf("Bluetooth device removed.\n");
	}
}


int hal_init(void)
{
	LibHalContext *ctx;
	DBusConnection *dbus_conn;
	DBusError error;
	char **devices;
	int num, ret = -1;

	dbus_error_init(&error);

	dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if (!dbus_conn) {
		print_error("DBUS connection failed", &error);
		goto out;
	}

	ctx = libhal_ctx_new();
	if (!ctx) {
		print_error("Could not create HAL context", NULL);
		goto out;
	}

	libhal_ctx_set_dbus_connection(ctx, dbus_conn);
	if (!libhal_ctx_init(ctx, &error)) {
		print_error("HAL startup failed", &error);
		goto out;
	}

	devices = libhal_find_device_by_capability(ctx, "bluetooth_hci",
			&num, &error);

	if (dbus_error_is_set(&error)) {
		print_error("HAL error", &error);
		libhal_ctx_free(ctx);
		goto out;
	}

	if (devices && devices[0]) {
		bluetooth_udi = strdup(devices[0]);
		dbus_free_string_array(devices);
	}

	libhal_ctx_set_device_removed(ctx, hal_removed_cb);
	dbus_connection_get_unix_fd(dbus_conn, &ret);

out:
	if (dbus_error_is_set(&error))
		dbus_error_free(&error);

	return ret;
}

