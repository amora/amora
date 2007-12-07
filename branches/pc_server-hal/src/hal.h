/**
 * @file   hal.h
 * @author Thiago Marcos P. Santos
 * @date  Tue Jul 10 16:09:20 2007
 *
 * @brief Amora HAL subsystem API
 *
 */

/*  Copyright (C) 2007 <tmpsantos@gmail.com>
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

#ifndef __HAL_H__
#define __HAL_H__

#ifdef HAVE_HAL


/** Initiate HAL/DBUS subsystem
 *
 * HAL is used to detect the bluetooth dongle removal
 *
 * @return the DBUS file descriptor, -1 on error.
 */
static int hal_init(void);


#endif /* HAVE_HAL */

#endif /* __HAL_H__ */
