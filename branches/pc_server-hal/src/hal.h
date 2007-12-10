/**
 * @file   hal.h
 * @author Thiago Marcos P. Santos
 * @date   Sun Dec  9 22:36:27 AMT 2007
 *
 * @brief Amora HAL subsystem API
 *
 * \todo
 * - add utests using 'check' library
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

#include "config.h"

#ifndef __HAL_H__
#define __HAL_H__

#ifdef HAVE_HAL


/** Initiate HAL/DBUS subsystem
 *
 * HAL is used to detect the bluetooth dongle removal
 *
 * @param hci_id the inteface identifier (hciN, where N is the hci_id)
 *
 * @return the DBUS file descriptor or -1 on error.
 */
int hal_init(int hci_id);


/** Dispatch HAL queued events
 *
 * Must be called when DBUS file descriptor is ready for read()
 *
 * @return 1 on sucess or -1 on error.
 */
int hal_dispatch(void);


/** Check if there is a bluetooth dongle connected
 *
 * @return 1 if dongle is connected or 0 otherwise.
 */
int hal_has_dongle(void);


#endif /* HAVE_HAL */

#endif /* __HAL_H__ */
