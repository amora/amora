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

#include <string.h>
#include "protocol.h"

/** Event command strings
 * ps: first element is "EMPTY", since enumerations should
 * not have value zero. See \ref codes.
 **/
const char *all_codes[] = { "EMPTY",
			    "UP", "DOWN", "LEFT", "RIGHT",
			    "ENTER", "ESC", "SPACE",
			    "DEL", "ALT", "TAB",
			    "SLIDESHOW", "FULLSCREEN",
			    "MOUSE_MOVE", "MOUSE_BUTTON_PRESS",
			    "MOUSE_BUTTON_RELEASE",
			    "MOUSE_BUTTON_RIGHT",
			    "MOUSE_BUTTON_LEFT", "MOUSE_BUTTON_MIDDLE",
			    "MOUSE_SCROLL_UP", "MOUSE_SCROLL_DOWN",
			    "CONN_CLOSE", "SERVER_STOP", "RESOLUTION",
			    "IMG_FORMAT"
			    };


int ecell_button_ewindow(char *event, int length)
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
	else if (!strncasecmp(event, all_codes[ENTER], length))
		res = ENTER;
	else if (!strncasecmp(event, all_codes[ESC], length))
		res = ESC;
	else if (!strncasecmp(event, all_codes[SPACE], length))
		res = SPACE;
	else if (!strncasecmp(event, all_codes[DEL], length))
		res = DEL;
	else if (!strncasecmp(event, all_codes[ALT], length))
		res = ALT;
	else if (!strncasecmp(event, all_codes[TAB], length))
		res = TAB;
	else if (!strncasecmp(event, all_codes[SLIDESHOW], length))
		res = SLIDESHOW;
	else if (!strncasecmp(event, all_codes[FULLSCREEN], length))
		res = FULLSCREEN;

	return res;
}


int ecell_mouse_ewindow(char *event, int length)
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


int protocol_command(char *cmd, int length)
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
