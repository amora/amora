#include <stdio.h>
#include <sys/timeb.h>
#include <X11/extensions/XTest.h>
#include "x11_event.h"
#include "protocol.h"
#include "log.h"

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

	//needed to flush events before any new command.
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
		log_message(FIL|ERR, "Cannot create mouse click events\n");

	return result;
}
