/**
 * @file   imscreen.c
 * @author Adenilson Cavalcanti
 * @date   Tue Aug 28 13:27:13 2007
 *
 * @brief  Screenshot and image handling module.
 *
 * The code here is responsible to take screenshots from active window,
 * resize and save it to filesystem.
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

#include "imscreen.h"
#include "string.h"

/** We expect file extension names to have only 3 characters (e.g. jpg, bmp,
 * png, etc).
 */
const int const FILE_EXTENSION_LENGTH = 3;

/** The file extension separator must be the dot (e.g. '.png')
 */
const char const FILE_EXTENSION_MARK = '.';

/** If function \ref save_image fails to identify filename extension, it
 * defaults to 'png' format.
 */
char *const FILE_DEFAULT_FORMAT = ".png";

int screen_capture(Display *display, Imlib_Image *image)
{
	int res = -1, tmp, revert_to;
	Window window;
	XWindowAttributes window_attr;

	if ((!image) || (!display))
		goto exit;

	tmp = XGetInputFocus(display, &window, &revert_to);
	if ((tmp == BadValue) || (tmp == BadWindow))
		goto exit;

        tmp = XGetWindowAttributes(display, window, &window_attr);
	if ((tmp == BadDrawable) || (tmp == BadWindow))
		goto exit;


	imlib_context_set_anti_alias(1);
	imlib_context_set_blend(0);
        imlib_context_set_display(display);
        imlib_context_set_visual(DefaultVisual(display,
					       DefaultScreen(display)));
        imlib_context_set_colormap(DefaultColormap(display,
						   DefaultScreen(display)));
        imlib_context_set_drawable(window);
        *image = imlib_create_image_from_drawable((Pixmap)0, 0, 0,
						  window_attr.width,
						  window_attr.height, 1);

	if (*image)
		res = 0;

exit:
	return res;
}


int save_image(Imlib_Image *image, char *name)
{
	int res = 0;
	char *ptr;

	if (!image)
		goto exit;

	/* Checks for extension format, if it has more than 3 characters,
	 * defaults to ".png".
	 */
	ptr = (name - FILE_EXTENSION_LENGTH);
	if ((*(ptr - 1)) != FILE_EXTENSION_MARK)
		ptr = FILE_DEFAULT_FORMAT;

	imlib_context_set_image(*image);
	imlib_image_set_format(ptr);
	/* TODO: how to check for success in imlib? */
	imlib_save_image(name);

exit:
	return res;
}

int rescale_image(Imlib_Image *image, int new_width, int new_height,
		  Imlib_Image *rescaled)
{
	int res = -1;
	if ((!image) || (!rescaled))
		goto exit;

	imlib_context_set_image(image);
	*rescaled = imlib_create_cropped_scaled_image(0, 0,
						     imlib_image_get_width(),
						     imlib_image_get_height(),
						     new_width, new_height);

	res = 0;
exit:
	return res;
}


int rotate_image(Imlib_Image *image)
{
	int res = -1;
	if (!image)
		goto exit;

	imlib_context_set_image(image);
	imlib_image_orientate(1);

	res = 0;
exit:
	return res;
}
