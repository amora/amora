/**
 * @file   hal.c
 * @author Thiago Marcos P. Santos
 * @date   Sun Dec  9 22:36:27 AMT 2007
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

#include <assert.h>
#include <dbus/dbus.h>
#include <libhal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hal.h"

/** HAL userdata */
static struct hal_s {
	/** Bluetooh identifier - FIXME: memory leak */
	char *bluetooth_udi;
	/** DBUS connection handler */
	DBusConnection *dbus;
} hal;


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


/** Callback called when some device is removed
 *
 * @param ctx HAL context
 * @param udi HAL unique hardware identifier
 *
 */
static void hal_removed_cb(LibHalContext *ctx, const char *udi) {
	(void) ctx;

	assert(udi);

	if (strcmp(hal.bluetooth_udi, udi) == 0) {
		printf("Bluetooth device removed.\n");
		free(hal.bluetooth_udi);
		hal.bluetooth_udi = NULL;
	}
}


/** Find the udi based on hci interface indentifier
 *
 * @param ctx HAL context
 * @param hci_id the inteface identifier (hciN, where N is the hci_id)
 *
 * @return the device udi or NULL if not found
 *
 */
static char *find_udi_by_ifid(LibHalContext *ctx, int hci_id)
{
	DBusError error;
	char **devices, *hal_ifid, *ret = NULL;
	char ifid[7];
	int i, num, match = 0;

	assert(ctx);

	snprintf(ifid, sizeof(ifid - 1), "hci%d", hci_id);

	dbus_error_init(&error);
	devices = libhal_find_device_by_capability(ctx, "bluetooth_hci",
		&num, &error);

	if (dbus_error_is_set(&error)) {
		print_error("HAL error", &error);
		goto out;
	}

	if (devices && devices[0]) {
		for (i = 0; i < num; i++) {
			hal_ifid = libhal_device_get_property_string(ctx, devices[i],
					"bluetooth_hci.interface_name", &error);

			if (dbus_error_is_set(&error)) {
				print_error("HAL error", &error);
				goto out;
			}

			if (hal_ifid) {
				if (strcmp(hal_ifid, ifid) == 0)
					match = 1;

				libhal_free_string(hal_ifid);
			}

			if (match) {
				printf("HAL: setting watch to %s.\n", ifid);
				ret = strdup(devices[i]);
				break;
			}
		}
		dbus_free_string_array(devices);
	}

out:
	if (dbus_error_is_set(&error))
		dbus_error_free(&error);

	return ret;
}


int hal_init(int hci_id)
{
	LibHalContext *ctx;
	DBusError error;
	int ret = -1;

	printf("HAL: starting.\n");

	dbus_error_init(&error);

	hal.dbus = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if (!hal.dbus) {
		print_error("DBUS connection failed", &error);
		goto out;
	}

	ctx = libhal_ctx_new();
	if (!ctx) {
		print_error("Could not create HAL context", NULL);
		goto out;
	}

	libhal_ctx_set_dbus_connection(ctx, hal.dbus);
	if (!libhal_ctx_init(ctx, &error)) {
		print_error("HAL startup failed", &error);
		goto out;
	}

	hal.bluetooth_udi = find_udi_by_ifid(ctx, hci_id);
	if (!hal.bluetooth_udi) {
		print_error("Bluetooth device not found", NULL);
		libhal_ctx_free(ctx);
		goto out;
	}

	libhal_ctx_set_device_removed(ctx, hal_removed_cb);
	dbus_connection_get_unix_fd(hal.dbus, &ret);

out:
	if (dbus_error_is_set(&error))
		dbus_error_free(&error);

	return ret;
}


int hal_dispatch(void)
{
	dbus_connection_read_write_dispatch(hal.dbus, 0);

	return 0;
}

int hal_has_dongle(void)
{
	return hal.bluetooth_udi ? 1 : 0;
}

