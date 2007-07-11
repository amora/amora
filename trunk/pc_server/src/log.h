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

#include <stdio.h>
#include <stdlib.h>

#define SYS   0x01 << 0
#define FIL   0x01 << 1
#define OUT   0x01 << 2
#define ERR   0x01 << 3

#define MAXLINE 1024
#define LOG_FILE "/tmp/amora.log"

/* General debug macro */
#ifdef DEBUG
#define DPRINT(fmt, ...) printf("DEBUG: "fmt"\n", ##__VA_ARGS__)
#else
#define DPRINT(fmt, ...)
#endif

/** Report log messages.
 *
 * @param ldest log destination(s) (syslog, file, stdout, etc)
 * @param format message to log.
 *
 * @return 0 on success, -1 otherwise
 */
int log_message(unsigned int ldest, const char *format, ...);

/** Report a fatal error and terminate.
 *
 * @param fmt message to log.
 */
void dief(const char *fmt, ...);

#endif
