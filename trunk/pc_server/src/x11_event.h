/**
 * @file   x11_event.h
 * @author Adenilson Cavalcanti
 * @date   Fri May  4 14:10:49 2007
 *
 * @brief  XEvents module.
 *
 * This file will hold functions that interact with X server, sending
 * events to window session.
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

#ifndef __XEVENTS_PC__
#define __XEVENTS_PC__

#include <X11/Xlib.h>


/** X server key codes, you can get them from 'xev',
 * bellow are the key codes for following keys:
 * - Arrow up 98
 * - Arrow down 104
 * - Arrow left 100
 * - Arrow right 102
 * - Enter key 36
 * - ESC key 9
 * - Space key 65
 * - Del key 107
 * - Alt key 64
 * - Tab key 23
 * - F5 key (start slideshow in OpenOffice.org)
 * - F key (fullscreen for Mplayer and Totem)
 *
 * ps: first element is -1, since enumerations should
 * not have value zero. See \ref codes.
 */
static int x_key_code[] = { -1, 98, 104, 100, 102, 36, 9, 65, 107, 64, 23, 71,
			    41 };

/** Display handler creator.
 *
 * Use this function to construct a Display handler.
 *
 * @param display_name Use default value NULL.
 *
 * @return A pointer to a display structure or NULL in error.
 */
Display* construct_display(char *display_name);

/** Display handler destructor.
 *
 *
 * @param active_display Pointer to display.
 *
 * @return Result of XCloseDisplay call.
 */
int destroy_display(Display *active_display);


/** Function to send keyevents to active Display.
 *
 *
 * @param type Type of event (i.e. KeyPress).
 * @param keycode Keycode of event (i.e. 98 for up arrow).
 * @param active_display Pointer to display structure (see \ref construct_display).
 *
 * @return On sucess 0, -1 otherwise.
 */
int send_event(int type, int keycode, Display *active_display);


/** Function to move the mouse around.
 *
 * @param x Coordinate to move the mouse (origin at upper left corner of window).
 * @param y Coordinate to move the mouse.
 * @param active_display Current Display, connection with X server (see
 * \ref construct_display)
 *
 * @return 0 on sucess, -1 otherwise.
 */
int mouse_move(int x, int y, Display *active_display);

/** Function to send mouse button events.
 *
 * It requires XTest extension available on host, and also library
 * XTest. I don't like to increase dependecies on server app, but
 * don't know any other way to send mouse button events to X server.
 *
 * @param mouse_button Mouse button code, see \ref codes
 * @param button_status If button is pressed (MOUSE_BUTTON_PRESS) or released
 * (MOUSE_BUTTON_RELEASE)
 * @param active_display Current Display, connection with X server (see
 * \ref construct_display)
 *
 * @return 0 on sucess, -1 otherwise.
 */
int mouse_click(int mouse_button, int button_status, Display *active_display);

#endif
