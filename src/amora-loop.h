/**
 * @file   amora-loop.h
 * @author Thiago Marcos P. Santos
 * @date  Sun Dec  9 20:08:53 AMT 2007
 *
 * @brief Main loop handler.
 *
 * \todo
 * - this implementation of loop can prove itself useful for
 * other projects and it can be a good idea to move it to an
 * independent library (uloop library).
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

#ifndef __AMORA_LOOP_H__
#define __AMORA_LOOP_H__


/** Remove a file descriptor from the loop set
 *
 * Note: the program is responsible to close the fd
 *
 * @param fd the fd that should be removed
 *
 * @return 0 on success, -1 otherwise
 *
 */
int loop_remove(const int fd);


/** Add a file descriptor to the loop fd_set
 *
 * Note: the callback must return 0 on success and -1 on fail.
 *
 * @param fd the file descriptor itself
 * @param callback the callback that will be called when fd is ready
 * @param data the userdata passed as parameter to the callback
 *
 * @return 0 on success, -1 otherwise
 *
 */
int loop_add(const int fd, int (*callback) (int fd, void *data), void *data);


/** Loop pooling the file descriptor set
 *
 * Preform a single iteration in the file descriptor pool.
 *
 * @return 0 on success, -1 otherwise
 *
 */
int loop_iteration(void);


/** Loop pooling the file descriptor set
 *
 * The loop will return when an error occurs with some file descritor
 * in the set or when the set became empty. Otherwise, the loop will
 * run forever.
 *
 * @return 0 on success, -1 otherwise
 *
 */
int loop(void);


#endif /* __AMORA_LOOP_H__ */

