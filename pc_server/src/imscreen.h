/**
 * @file   imscreen.h
 * @author Adenilson Cavalcanti
 * @date   Tue Aug 28 12:08:21 2007
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

#ifndef __IMSCREEN_PC__
#define __IMSCREEN_PC__

#include <X11/Xlib.h>
#include <Imlib2.h>

/** Creates a screenshot from active window.
 *
 * It has some calls for Xlib to discover current active window (and its
 * dimensions).
 *
 * @param display Display pointer (see \ref x11_event.h contruct_display).
 * @param image A imlib2 image pointer.
 *
 * @return 0 for success, -1 otherwise.
 */
int screen_capture(Display *display, Imlib_Image *image);


/** Saves an image structure to filesystem.
 *
 *
 * @param image An imlib image pointer (hopefully with screen data taken by
 * \ref screen_capture).
 * @param name A file name (e.g. "screenshot.jpg")
 *
 * @return 0 for success, -1 otherwise.
 */
int save_image(Imlib_Image *image, char *name);


/** Rescale image, just a thin wrapper around Imlib2.
 *
 * @param image An imlib image pointer (hopefully with screen data taken by
 * \ref screen_capture).
 * @param new_width New width dimension (e.g. 320 pixels)
 * @param new_height New height dimension (e.g. 240 pixels)
 * @param rescaled Image pointer that will hold rescaled new image.
 *
 * @return 0 for success, -1 otherwise.
 */
int rescale_image(Imlib_Image *image, int new_width, int new_height,
		  Imlib_Image *rescaled);


#endif
