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
#include <sys/timeb.h>
#include <X11/extensions/XTest.h>
#include "protocol.h"

/** X server key codes, you can get them from 'xev'
 * ps: first element is -1, since enumerations should
 * not have value zero. See \ref codes.
 */
int x_key_code[] = { -1, 98, 104, 100, 102 };


/** Display handler creator.
 *
 * Use this function to construct a Display handler.
 *
 * @param display_name Use default value NULL.
 *
 * @return A pointer to a display structure or NULL in error.
 */
Display* construct_display(char *display_name)
{

	Display *res;
	res = XOpenDisplay(display_name);
	if (!res)
		res = NULL;

	return res;

}

/** Display handler destructor.
 *
 *
 * @param active_display Pointer to display.
 *
 * @return Result of XCloseDisplay call.
 */
int destroy_display(Display *active_display)
{
	int res;
	res = XCloseDisplay(active_display);
	return res;
}


/** Function to send keyevents to active Display.
 *
 *
 * @param type Type of event (i.e. KeyPress).
 * @param keycode Keycode of event (i.e. 98 for up arrow).
 * @param active_display Pointer to display structure (see \ref construct_display).
 *
 * @return On sucess 0, -1 otherwise.
 */
int send_event(int type, int keycode, Display *active_display)
{
	int res = 0;
	XEvent event;
	Window win;
	int revert_to;
        struct timeb t;

        ftime(&t);

	XGetInputFocus(active_display, &win, &revert_to);

	event.xkey.type = type;
	event.xkey.serial = 0;
	event.xkey.send_event = True;
	event.xkey.display = active_display;
	event.xkey.window = win;
	event.xkey.root = XDefaultRootWindow(active_display);
	event.xkey.subwindow = None;
	event.xkey.time = t.time*1000+t.millitm;
	event.xkey.x = 0;
	event.xkey.y = 0;
	event.xkey.x_root = 0;
	event.xkey.state = 0;
	event.xkey.keycode = keycode;
	event.xkey.same_screen=True;

	res = XSendEvent(active_display, InputFocus, True, 3, &event);
        XFlush(active_display);

	if (res == BadValue || res == BadWindow)
		res = -1;

	return res;
}

/** Function to move the mouse around.
 *
 * @param x Coordinate to move the mouse (origin at upper left corner of window).
 * @param y Coordinate to move the mouse.
 * @param active_display Current Display, connection with X server (see
 * \ref construct_display)
 *
 * @return 0 on sucess, -1 otherwise.
 */
int mouse_move(int x, int y, Display *active_display)
{
	int res = 0;
	Window win;
	int revert_to;

	if (!active_display) {
		res = -1;
		goto exit;
	}

	XGetInputFocus(active_display, &win, &revert_to);
	res = XWarpPointer(active_display,
			   None, DefaultRootWindow(active_display),
			   0, 0, 0, 0,
			   x, y);

	if (res == BadValue || res == BadWindow)
		res = -1;


 exit:
	return res;

}

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
int mouse_click(int mouse_button, int button_status, Display *active_display)
{
	int result = -1;
	static int is_xtest_available = -1;
	int ev, er, ma, mi;
	int revert_to;
	Window win;

	if (is_xtest_available == -1)
		is_xtest_available = XTestQueryExtension(active_display,
							 &ev, &er, &ma, &mi);

	if (is_xtest_available) {
		XGetInputFocus(active_display, &win, &revert_to);

		if (mouse_button == MOUSE_BUTTON_LEFT &&
			button_status == MOUSE_BUTTON_PRESS)
			XTestFakeButtonEvent(active_display, 1, True,
					     CurrentTime);
		else if (mouse_button == MOUSE_BUTTON_LEFT &&
			button_status == MOUSE_BUTTON_RELEASE)
			XTestFakeButtonEvent(active_display, 1, False,
					     CurrentTime);
		else if (mouse_button == MOUSE_BUTTON_MIDDLE &&
			button_status == MOUSE_BUTTON_PRESS)
			XTestFakeButtonEvent(active_display, 2, True,
					     CurrentTime);
		else if (mouse_button == MOUSE_BUTTON_MIDDLE &&
			button_status == MOUSE_BUTTON_RELEASE)
			XTestFakeButtonEvent(active_display, 2, False,
					     CurrentTime);
		else if (mouse_button == MOUSE_BUTTON_RIGHT &&
			button_status == MOUSE_BUTTON_PRESS)
			XTestFakeButtonEvent(active_display, 3, True,
					     CurrentTime);
		else if (mouse_button == MOUSE_BUTTON_RIGHT &&
			button_status == MOUSE_BUTTON_RELEASE)
			XTestFakeButtonEvent(active_display, 3, False,
					     CurrentTime);
		else if (mouse_button == MOUSE_SCROLL_UP) {
			XTestFakeButtonEvent(active_display, 4, True,
					     CurrentTime);
			XTestFakeButtonEvent(active_display, 4, False,
					     CurrentTime);

		} else if (mouse_button == MOUSE_SCROLL_DOWN) {
			XTestFakeButtonEvent(active_display, 5, True,
					     CurrentTime);
			XTestFakeButtonEvent(active_display, 5, False,
					     CurrentTime);

		}

		XFlush(active_display);
		result = 0;
	} else
		printf("Cannot create mouse click events\n");

	return result;
}

#endif
