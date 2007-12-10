/**
 * @file   loop.h
 * @author Thiago Marcos P. Santos
 * @date  Sun Dec  9 20:08:53 AMT 2007
 *
 * @brief Main loop handler
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

#include "config.h"

#ifndef __LOOP_H__
#define __LOOP_H__


int loop_remove(const int fd);

int loop_add(const int fd, int (*callback) (int fd));

int loop(void);


#endif /* __LOOP_H__ */
