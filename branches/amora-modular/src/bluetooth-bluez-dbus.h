/**
 * @file   bluetooth-bluez-dbus.h
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

#ifdef HAVE_DBUS

#ifndef __BLUETOOTH_BLUEZ_DBUS_H__
#define __BLUETOOTH_BLUEZ_DBUS_H__

#include <dbus/dbus.h>


/** Register a watch for dongle removal
 *
 * @param conn the D-Bus active connection handler
 * @param hci_id the hci device to watch for disconnection
 *
 * @return 0 on success, -1 otherwise
 *
 */
int dbus_bt_dongle_removed_watch(DBusConnection *conn, char *hci_id);


#endif /* __BLUETOOTH_BLUEZ_DBUS_H__ */

#endif /* HAVE_DBUS */
