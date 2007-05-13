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


/** X server key codes, you can get them from 'xev'
 */
int x_key_code[] = { 98, 104, 100, 102 };


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

#endif
