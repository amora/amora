/**
 * @file   log.c
 * @author Vanderson Rodrigues
 * @date  Tue Jul 10 16:29:07 2007
 *
 * @brief Simple logging system.
 *
 * This logging system  can be used to log in: syslog, files, stdout and
 * stderr.
 *
 */

/*  Copyright (C) 2007  Vanderson Rodrigues <vanderson.gold@gmail.com>
 *
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
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <string.h>

#include "log.h"

#define BSIZE 128

static void get_timestamp(time_t curtime, char *timestamp)
{
	struct tm *loctime;
	char buf[BSIZE];

	loctime = localtime(&curtime);

	/* the date in a nice format. */
	strftime(buf, BSIZE, "%b %d %T", loctime);
	strncpy(timestamp, buf, BSIZE);
}

int log_message(unsigned int ldest, const char *fmt, ...)
{
	va_list ap;
	char *timestamp;
	time_t curtime;
	char buf[MAXLINE];
	char msg[MAXLINE];
	int fd;

	va_start(ap, fmt);
	vsnprintf(buf, MAXLINE, fmt, ap);

	/* Log to file, timestamp  included */
	if (ldest & FIL) {
		if ((fd = open(LOG_FILE, O_APPEND|O_WRONLY|O_CREAT,0644)) < 0) {
			fprintf(stderr, "Error opening log file: %s\n", LOG_FILE);
			return -1;
		}

		timestamp = malloc(BSIZE * sizeof(char));
		curtime = time(NULL);
		get_timestamp(curtime, timestamp);

		if (fd != -1) {
			snprintf(msg, MAXLINE, "[%s]: %s\n", timestamp, buf);
			write(fd, msg, strlen(msg));
		} else {
			return -1;
		}
	}

	/* log to stdout */
	if (ldest & OUT) {
		snprintf(msg, MAXLINE, "%s\n", buf);
		write(STDOUT_FILENO, msg, strlen(msg));
	}

	/* log to syslog */
	if (ldest & SYS) {
		syslog(LOG_INFO, buf);
	}

	/* log to stderr */
	if (ldest & ERR) {
		snprintf(msg, MAXLINE, "%s\n", buf);
		write(STDERR_FILENO, msg, strlen(msg));
	}

	va_end(ap);
	close(fd);

	return 0;
}

void dief(const char *fmt, ...)
{
	char msg[MAXLINE];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(msg, sizeof(msg), fmt, ap);
	log_message(ERR | FIL, msg);
	va_end(ap);
	exit(1);
}
