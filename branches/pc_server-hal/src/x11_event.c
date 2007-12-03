/*  Copyright (C) 2007  Adenilson Cavalcanti <savagobr@yahoo.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; by version 2 of the License or (at your
 *  choice) any later version.
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

#include <stdio.h>
#include <sys/time.h>
#include <X11/extensions/XTest.h>
#include "x11_event.h"
#include "protocol.h"

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
int x_key_code[] = { -1, 98, 104, 100, 102, 36, 9, 65, 107, 64, 23, 71, 41 };

Display* construct_display(char *display_name)
{

	Display *res;
	res = XOpenDisplay(display_name);
	if (!res)
		res = NULL;

	return res;

}

int destroy_display(Display *active_display)
{
	int res;
	res = XCloseDisplay(active_display);
	return res;
}

/* Derived from sendevent.c
 * Copyright (C) 2002 Erich Kitzmüller  erich.kitzmueller@itek.at
 */
int send_event(int type, int keycode, Display *active_display)
{
	int res = 0;
	XEvent event;
	Window win;
	int revert_to;
	struct timeval t;

	gettimeofday(&t, NULL);

	XGetInputFocus(active_display, &win, &revert_to);

	event.xkey.type = type;
	event.xkey.serial = 0;
	event.xkey.send_event = True;
	event.xkey.display = active_display;
	event.xkey.window = win;
	event.xkey.root = XDefaultRootWindow(active_display);
	event.xkey.subwindow = None;
	event.xkey.time = t.tv_usec;
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

	/* needed to flush events before any new command. */
	XFlush(active_display);
	if (res == BadValue || res == BadWindow)
		res = -1;


exit:
	return res;

}


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
		perror("Cannot create mouse click events\n");

	return result;
}

Window find_real_window_down(Display *display, Window window)
{
	Atom wm_state, type;
	int format;
	unsigned long nitems, after;
	unsigned char* prop;

	Window root, parent, old_parent;
	Window* children;
	unsigned int nchildren;
	Window result;

	wm_state = XInternAtom(display, "WM_STATE", False);

	do {
		old_parent = window;

		/* TODO: check return value of Xlib functions */
		XQueryTree(display, window, &root, &parent, &children,
			   &nchildren);

		XGetWindowProperty(display, window, wm_state, 0, 0, False,
				   AnyPropertyType, &type, &format,
				   &nitems, &after, &prop);

		if (prop != NULL)
			XFree(prop);
		prop = NULL;

		if (children != NULL) {
			XFree(children);
			children = NULL;
		}

		window = parent;

	} while (parent != root);

	/* Has no attribute */
	if ((type == None) && (format == 0) && (after == 0))
		result = old_parent;
	else
		result = window;

	return result;
}
