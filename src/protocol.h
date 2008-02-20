/**
 * @file   protocol.h
 * @author Adenilson Cavalcanti
 * @date   $LastChangedDate$
 *
 * @brief  Protocol handling functions/codes goes here.
 *
 * I'm still working on a protocol for handling events and data
 * exchange between the cellphone and PC.
 *
 * \todo
 * - add utests using 'check' library
 */

/*  Copyright (C) 2007  Adenilson Cavalcanti <savagobr@yahoo.com>
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

/** Command event codes, has codes for:
 * - Arrow keys
 * - Mouse keys
 * - Connection/protocol related
 *
 */
enum { NONE = -2,
       UP = 1, DOWN, LEFT, RIGHT,
       ENTER, ESC, SPACE, DEL, ALT, TAB,
       SLIDESHOW, FULLSCREEN,
       MOUSE_MOVE, MOUSE_BUTTON_PRESS, MOUSE_BUTTON_RELEASE,
       MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE,
       MOUSE_SCROLL_UP, MOUSE_SCROLL_DOWN,
       CONN_CLOSE, SERVER_STOP, RESOLUTION, IMG_FORMAT,
       SCREEN_MODE_ON, SCREEN_MODE_OFF, SCREEN_ROTATE, SCREEN_NORMAL,
       SCREEN_RESOLUTION, SCREEN_WIDTH, SCREEN_HEIGHT,
       SCREEN_TAKE
       } codes;


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
int ecell_button_ewindow(char *event, int length);

/** Convert a cell mouse event to correspondent X window code.
 *
 *
 * @param event String with event/command.
 * @param length Length of string command.
 *
 * @return A code representing the event or NONE otherwise.
 */
int ecell_mouse_ewindow(char *event, int length);

/** Convert a cell mouse event to correspondent protocol code.
 *
 * @param cmd String with event/command.
 * @param length Length of string command.
 *
 * @return A code representing the event or NONE otherwise.
 */
int protocol_command(char *cmd, int length);

#endif
