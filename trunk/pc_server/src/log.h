/**
 * @file   log.h
 * @author Vanderson Rodrigues
 * @date  Tue Jul 10 16:09:20 2007
 *
 * @brief Amora Log system definitions.
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

#ifndef _H_LOG
#define _H_LOG

/** Define log message destination, this one is for syslog. */
#define SYS   0x01 << 0
/** Message destination: log file */
#define FIL   0x01 << 1
/** Message destination:  stdout*/
#define OUT   0x01 << 2
/** Message destination:  stderr */
#define ERR   0x01 << 3

/** Log structure, contains resources used by logging function.
 */
struct log_resource {
	/** File descriptor */
	int fd;
	/** Time stamp buffer */
	char *timestamp;
	/** Message buffer, will be of size \ref MSG_BUFFER_LENGTH */
	char *message;
	/** Temporary buffer */
	char *buffer;
	/** Timestamp buffer length */
	int ts_length;
	/** Maximum line length, equal 3 times 80 columns */
	int length;
	/** log file path name */
	char *log_filename;

};


/** Builds a log resource structure.
 *
 * Use it to initialize log buffers and open file descriptors.
 *
 * @param filename Path and filename for log file.
 */
struct log_resource* log_build_resources(char *filename);


/** Deallocate log resource structure.
 *
 * Use it to clean up resouces used by log function.
 */
void log_clean_resources(struct log_resource *ptr);


/** Report log messages.
 *
 * @param ldest log destination(s) (syslog, file, stdout, etc)
 * @param resource A log resource structure, see \ref log_resource.
 * @param format message to log.
 *
 * @return 0 on success, -1 otherwise
 */
int log_message(unsigned int ldest, struct log_resource *resource,
		const char *format, ...);

#endif
