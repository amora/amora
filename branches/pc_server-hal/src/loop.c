/**
 * @file   loop.c
 * @author Thiago Marcos P. Santos
 * @date   Sun Dec  9 20:08:53 AMT 2007
 *
 * @brief  Main loop handler
 *
 */

/*  Copyright (C) 2007  Thiago Marcos P. Santos <tmpsantos@gmail.com>
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

#include <assert.h>
#include <stdlib.h>
#include <sys/select.h>

#include "loop.h"


/** The loop resource holder */
static struct loop_set_s {
	/** The callback for each fd using the fd as index */
	int (*callback[FD_SETSIZE]) (int fd);
	/** The fd with max number as required by select(). Don't ask me why. */
	int nfds;
	/** The fd set, only ready for reading is supported by now */
	fd_set readfds;
} loop_set;


/** Dispatch the event
 *
 * @param fd the file descriptor that has trigged the event
 *
 * @return 0 on sucess, -1 otherwise
 *
 */
static int dispatch(int fd)
{
	assert(fd >= 0 && fd < FD_SETSIZE);

	return loop_set.callback[fd](fd);
}


/** Check if the given fd_set is empty
 *
 * @param fds the fd_set conforming select.h
 *
 * @return 1 if the set is empty, 0 otherwise
 *
 */
static int is_empty(fd_set *fds)
{
	int i, ret = 1;

	assert(fds);

	for (i = 0; i < FD_SETSIZE; i++) {
		if (FD_ISSET(i, fds)) {
			ret = 0;
			goto out;
		}
	}

out:
	return ret;
}


int loop_add(const int fd, int (*callback) (int fd))
{
	int ret = -1;

	assert(fd >= 0 && fd < FD_SETSIZE && callback);

	if (FD_ISSET(fd, &loop_set.readfds))
		goto out;

	FD_SET(fd, &loop_set.readfds);
	loop_set.callback[fd] = callback;

	if (fd > loop_set.nfds)
		loop_set.nfds = fd;

	ret = 0;

out:
	return ret;
}


int loop_remove(const int fd)
{
	int i, ret = -1;

	assert(fd >= 0 && fd < FD_SETSIZE);

	if (FD_ISSET(fd, &loop_set.readfds)) {
		FD_CLR(fd, &loop_set.readfds);
		loop_set.callback[fd] = NULL;
		ret = 0;
	}

	loop_set.nfds = 0;

	for (i = 0; i < FD_SETSIZE; i++)
		if (FD_ISSET(i, &loop_set.readfds))
			loop_set.nfds = i;

	return ret;
}


int loop(void)
{
	fd_set readfds;
	int i , ret = 0;

	readfds = loop_set.readfds;

	while (select(loop_set.nfds + 1, &readfds, NULL, NULL, NULL) > 0) {
		for (i = 0; i < FD_SETSIZE; i++) {
			if (!FD_ISSET(i, &readfds))
				continue;

			if ((ret = dispatch(i)) < 0)
				goto out;
		}

		if (is_empty(&loop_set.readfds))
			goto out;

		readfds = loop_set.readfds;
	}

out:
	return ret;
}

