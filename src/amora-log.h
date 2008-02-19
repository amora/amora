/**
 * @file   amora-log.h
 * @author Ademar de Souza Reis Jr.
 * @author Milton Soares Filho
 * @author Adenilson Cavalcanti
 * @date  Tue Jul 10 16:09:20 2007
 *
 * @brief Amora Log system definitions.
 *
 * This logging system  can be used to log in: syslog, files, stdout and
 * stderr. Based on code from sniffdet: http://sniffdet.sf.net
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

#ifndef __AMORA_LOG_H__
#define __AMORA_LOG_H__

/** Message destination: log file */
#define FIL   0x01 << 0
/** Message destination:  stdout*/
#define OUT   0x01 << 1
#define FOO 0x01 << 2
#define BAR 0x01 << 3

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
	/** Log file path name */
	char *log_filename;
};


/** Builds a log resource structure.
 *
 * Use it to initialize log buffers and open file descriptors.
 *
 * @param filename path and filename for log file (you can pass NULL if you
 * wish to log *only* to stdout).
 *
 * @return the initialized log resource or NULL on error.
 */
struct log_resource* log_build_resources(const char *filename);


/** Deallocate log resource structure.
 *
 * Use it to clean up resouces used by log function.
 *
 * @param resource a log resource structure, see \ref log_resource.
 *
 */
void log_clean_resources(struct log_resource *resource);


/** Report log messages.
 *
 * Note: Logging with NULL resource is considered "not logging" and
 * this function will just return flawless.
 *
 * @param ldest bitmask with log destination(s).
 * @param resource a log resource structure, see \ref log_resource.
 * @param format message to log.
 *
 * @return 0 on success, -1 otherwise.
 */
int log_message(unsigned int ldest, struct log_resource *resource,
		const char *format, ...);

#endif /* __AMORA_LOG_H__ */

