/**
 * @file   log.c
 * @author Ademar de Souza Reis Jr.
 * @author Milton Soares Filho
 * @author Adenilson Cavalcanti
 * @date  Tue Jul 10 16:29:07 2007
 *
 * @brief Simple logging system.
 *
 * This logging system can be used to log in: files and stderr.
 *
 */

/*  Copyright (C) 2002, 2003 Ademar de Souza Reis Jr. <myself@ademar.org>
 *  Copyright (C) 2002, 2003 Milton Soares Filho <eu_mil@yahoo.com>
 *  Copyright (C) 2007  Adenilson Cavalcanti <savagobr@yahoo.com>
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

/** Length of timestamp buffer */
#define TIMESTAMP_LENGTH 20

/** Length of message buffer */
#define MSG_BUFFER_LENGTH 240


/** Set a timestamp on a given buffer
 *
 * @param timestamp pre-allocated buffer that will hold the timestamp.
 * @param length buffer size.
 *
 * @return 0 on success, -1 otherwise
 */
static int get_timestamp(char *timestamp, int length)
{
	struct tm *loctime;
	time_t curtime;

	if (!timestamp || length < 16)
		return -1;

	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime(timestamp, length - 1, "%b %d %T", loctime);

	return 0;
}


void log_clean_resources(struct log_resource *resource)
{
	if (resource) {
		close(resource->fd);
		free(resource->message);
		free(resource->timestamp);
		free(resource->log_filename);
		free(resource);
		resource = NULL;
	}
}


struct log_resource* log_build_resources(const char *filename)
{
	struct log_resource *resource;
	resource = (struct log_resource *) malloc(sizeof(struct log_resource));
	if (!resource) {
		perror("Failed allocation of log resource structure!\n");
		goto exit;
	}

	memset(resource, 0, sizeof(struct log_resource));
	resource->ts_length = TIMESTAMP_LENGTH;
	resource->length = MSG_BUFFER_LENGTH;

	resource->timestamp = (char*) malloc(resource->ts_length);
	resource->message = (char*) malloc(resource->length);
	resource->buffer = (char*) malloc(resource->length);

	if ((!resource->timestamp) || (!resource->message)
			|| (!resource->buffer)) {
		perror("Failed allocation of log internal buffers!\n");
		goto failed;
	}

	if (!filename) {
		perror("Invalid log filename\n");
		goto failed;
	}

	resource->log_filename = strdup(filename);
	if (!resource->log_filename) {
		perror("Failed log filename copying!\n");
		goto failed;
	}

	resource->fd = open(resource->log_filename,
			O_APPEND|O_WRONLY|O_CREAT, 0644);
	if (resource->fd < 0) {
		perror("Error opening log file");
		goto failed;
	}

	goto exit;

failed:
	if (resource->timestamp)
		free(resource->timestamp);
	if (resource->message)
		free(resource->message);
	if (resource->buffer)
		free(resource->buffer);

	free(resource);
	resource = NULL;

exit:
	return resource;
}



int log_message(unsigned int ldest, struct log_resource *resource,
		const char *format, ...)
{
	va_list ap;
	int ret = -1;

	if (!resource) {
		ret = 0;
		goto out;
	}

	if (ldest != FIL && ldest != OUT)
		goto out;

	if (!format)
		goto out;

	va_start(ap, format);
	vsnprintf(resource->buffer, resource->length, format, ap);

	/* Log to file, timestamp  included */
	if (ldest & FIL) {
		if (get_timestamp(resource->timestamp, resource->ts_length) == -1)
			goto out_va;

		snprintf(resource->message, resource->length - 1, "[%s]: %s\n",
			 resource->timestamp, resource->buffer);
		write(resource->fd, resource->message,
		      strlen(resource->message));
	}

	/* log to stdout */
	if (ldest & OUT) {
		snprintf(resource->message, resource->length - 1, "%s\n",
			 resource->buffer);
		write(STDOUT_FILENO, resource->message,
		      strlen(resource->message));
	}

	ret = 0;

out_va:
	va_end(ap);
out:
	return ret;
}

