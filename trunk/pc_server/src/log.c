/**
 * @file   log.c
 * @author Vanderson Rodrigues
 * @author Adenilson Cavalcanti
 * @date  Tue Jul 10 16:29:07 2007
 *
 * @brief Simple logging system.
 *
 * This logging system  can be used to log in: syslog, files, stdout and
 * stderr.
 *
 */

/*  Copyright (C) 2007  Vanderson Rodrigues <vanderson.gold@gmail.com>
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

void log_clean_resources(struct log_resource *ptr)
{
	if (ptr) {
		close(ptr->fd);
		free(ptr->message);
		free(ptr->timestamp);
		free(ptr->log_filename);
		free(ptr);
		ptr = NULL;
	}

}

struct log_resource* log_build_resources(char *filename)
{
	struct log_resource *result = NULL;
	result = (struct log_resource *) malloc(sizeof(struct log_resource));
	if (!result) {
		perror("Failed allocation of log resource structure!\n");
		goto exit;

	}

	memset(result, 0, sizeof(struct log_resource));
	result->ts_length = TIMESTAMP_LENGTH;
	result->length = MSG_BUFFER_LENGTH;

	result->timestamp = (char*) malloc(result->ts_length);
	result->message = (char*) malloc(result->length);
	result->buffer = (char*) malloc(result->length);

	if ((!result->timestamp) || (!result->message) || (!result->buffer)) {
		perror("Failed allocation of log internal buffers!\n");
		goto failed;
	}

	if (filename)
		result->log_filename = strdup(filename);
	else
		result->log_filename = strdup("/tmp/amora.log");

	if (!result->log_filename) {
		perror("Failed log filename copying!\n");
		goto failed;
	}

	result->fd = open(result->log_filename, O_APPEND|O_WRONLY|O_CREAT,
			  0644);
	if (result->fd < 0) {
		perror("Error opening log file");
		goto failed;
	}

	goto exit;

failed:
	if (result->timestamp)
		free(result->timestamp);
	if (result->message)
		free(result->message);
	if (result->buffer)
		free(result->buffer);

	free(result);
	result = NULL;

exit:
	return result;
}

static void get_timestamp(char *timestamp, int length)
{
	struct tm *loctime;
	time_t curtime;

	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime(timestamp, length - 1, "%b %d %T", loctime);
}

int log_message(unsigned int ldest, struct log_resource *resource,
		const char *format, ...)
{
	va_list ap;
	int fd = -1;
	int result = 0;

	if (!resource)
		return result;

	va_start(ap, format);
	vsnprintf(resource->buffer, resource->length, format, ap);

	/* Log to file, timestamp  included */
	if (ldest & FIL) {

		get_timestamp(resource->timestamp, resource->ts_length);
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

	/* log to syslog */
	if (ldest & SYS) {
		syslog(LOG_INFO, resource->buffer);
	}

	/* log to stderr */
	if (ldest & ERR) {
		snprintf(resource->message, resource->length -1, "%s\n",
			 resource->buffer);
		write(STDERR_FILENO, resource->message,
		      strlen(resource->message));
	}

	va_end(ap);
	if (fd > 0)
		close(fd);

	return result;
}

