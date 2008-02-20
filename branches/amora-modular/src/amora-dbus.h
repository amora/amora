/**
 * @file   amora-dbus.h
 * @author Thiago Marcos P. Santos
 * @date   $LastChangedDate$
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

#ifdef HAVE_DBUS

#ifndef __AMORA_DBUS_H__
#define __AMORA_DBUS_H__

#include <dbus/dbus.h>


/** Initialize D-Bus subsystem
 *
 * D-Bus connection will be integrated with amora main loop.
 * All messages will be automatically dispatched.
 *
 * @return D-Bus handler on success or NULL otherwise
 *
 */
DBusConnection *dbus_init(void);


#endif /* __AMORA_DBUS_H__ */

#endif /* HAVE_DBUS */

