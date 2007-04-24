

#include <X11/Xlib.h>
#include <sys/timeb.h>


enum { UP, DOWN, LEFT, RIGHT } codes;

char *cell_key_code[] = { "u",
			  "d",
			  "l",
			  "r"
};

int x_key_code[] = { 98, 104, 100, 102 };

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
