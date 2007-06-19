/**
 * @file   protocol.h
 * @author Adenilson Cavalcanti
 * @date   Tue May  8 09:08:59 2007
 *
 * @brief  Protocol handling functions/codes goes here.
 *
 * I'm still working on a protocol for handling events and data
 * exchange between the cellphone and PC.
 *
 */

/*  Copyright (C) 2007  Adenilson Cavalcanti <savagobr@yahoo.com>
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

#ifndef __PROTOCOL_PC__
#define __PROTOCOL_PC__

#include <string.h>


/** Command event codes, has codes for:
 * - Arrow keys
 * - Mouse keys
 * - Connection/protocol related
 *
 **/
enum { NONE = -1,
       UP = 1, DOWN, LEFT, RIGHT,
       MOUSE_MOVE, MOUSE_BUTTON_PRESS, MOUSE_BUTTON_RELEASE,
       MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE,
       MOUSE_SCROLL_UP, MOUSE_SCROLL_DOWN,
       CONN_CLOSE, SERVER_STOP, RESOLUTION, IMG_FORMAT,
       } codes;


/** Event command strings
 * ps: first element is "EMPTY", since enumerations should
 * not have value zero. See \ref codes.
 **/
static const char *all_codes[] = { "EMPTY",
				   "UP", "DOWN", "LEFT", "RIGHT",
				   "MOUSE_MOVE", "MOUSE_BUTTON_PRESS",
				   "MOUSE_BUTTON_RELEASE",
				   "MOUSE_BUTTON_RIGHT",
				   "MOUSE_BUTTON_LEFT", "MOUSE_BUTTON_MIDDLE",
				   "MOUSE_SCROLL_UP", "MOUSE_SCROLL_DOWN",
				   "CONN_CLOSE", "SERVER_STOP", "RESOLUTION",
				   "IMG_FORMAT" };


/** Special character to describe end of command */
static const char CMD_BREAK = '\n';


/** Convert a cell button event to correspondent X window event code.
 *
 *
 * @param event String with event/command.
 * @param length Length of string command.
 *
 * @return A code representing the event or NONE otherwise.
 * \todo: turn this code to a vector with loop... several if/elses are
 * not beauty.
 *
 */
static int ecell_button_ewindow(char *event, int length)
{
	int res = NONE;
	if (!strncasecmp(event, all_codes[UP], length))
		res = UP;
	else if (!strncasecmp(event, all_codes[DOWN], length))
		res = DOWN;
	else if (!strncasecmp(event, all_codes[LEFT], length))
		res = LEFT;
	else if (!strncasecmp(event, all_codes[RIGHT], length))
		res = RIGHT;

	return res;
};

/** Convert a cell mouse event to correspondent X window code.
 *
 *
 * @param event String with event/command.
 * @param length Length of string command.
 *
 * @return A code representing the event or NONE otherwise.
 */
static int ecell_mouse_ewindow(char *event, int length)
{
  	int res = NONE;

	if (!strncasecmp(event, all_codes[MOUSE_MOVE], length))
		res = MOUSE_MOVE;
	else if (!strncasecmp(event, all_codes[MOUSE_BUTTON_PRESS],
			      length))
		res = MOUSE_BUTTON_PRESS;
	else if (!strncasecmp(event, all_codes[MOUSE_BUTTON_RELEASE],
			      length))
		res = MOUSE_BUTTON_RELEASE;
	else if (!strncasecmp(event, all_codes[MOUSE_BUTTON_RIGHT],
			      length))
		res = MOUSE_BUTTON_RIGHT;
	else if (!strncasecmp(event, all_codes[MOUSE_BUTTON_LEFT],
			      length))
		res = MOUSE_BUTTON_LEFT;
	else if (!strncasecmp(event, all_codes[MOUSE_BUTTON_MIDDLE],
			      length))
		res = MOUSE_BUTTON_MIDDLE;
	else if (!strncasecmp(event, all_codes[MOUSE_SCROLL_UP],
			      length))
		res = MOUSE_SCROLL_UP;
	else if (!strncasecmp(event, all_codes[MOUSE_SCROLL_DOWN],
			      length))
		res = MOUSE_SCROLL_DOWN;

	return res;
}

/** Convert a cell mouse event to correspondent protocol code.
 *
 * @param event String with event/command.
 * @param length Length of string command.
 *
 * @return A code representing the event or NONE otherwise.
 *
 * \todo:
 * - implement logic processing for codes (SERVER_STOP,
 * RESOLUTION, IMG_FORMAT). This relates with \ref
 * main.c todos.
 */
static int protocol_command(char *cmd, int length)
{
 	int res = NONE;
 	if (!strncasecmp(cmd, all_codes[CONN_CLOSE], length))
		res = CONN_CLOSE;
	else if (strncasecmp(cmd, all_codes[SERVER_STOP], length))
		res = SERVER_STOP;
	else if (strncasecmp(cmd, all_codes[RESOLUTION], length))
		res = RESOLUTION;
	else if (strncasecmp(cmd, all_codes[IMG_FORMAT], length))
		res = IMG_FORMAT;

	return res;
}


#endif
