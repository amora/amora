/**
 * @file   dbus.h
 * @author Thiago Marcos P. Santos
 * @date   Tue Feb 12 00:07:16 AMT 2008
 *
 * @brief  D-Bus related functions.
 *
 */

/*  Copyright (C) 2008 <tmpsantos@gmail.com>
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

#ifdef HAVE_DBUS

#ifndef __DBUS_H__
#define __DBUS_H__


/** Initialize D-Bus subsystem
 *
 * Systems with hcid and D-Bus (most of Linux distros) we can use
 * D-Bus to query and listen for bluetooth related events such as
 * dongle removal and insertion.
 *
 * @param hci_id the hci device to watch for disconnection
 *
 * @return 0 on success, -1 otherwise
 *
 */
int dbus_init(char *hci_id);


#endif /* __DBUS_H__ */

#endif /* HAVE_DBUS */

