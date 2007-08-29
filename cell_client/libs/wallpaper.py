# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: a class to load, rescale and display a nice wallpaper in main
# application.

'''
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
'''
import graphics
import appuifw
import keyboard
import sysinfo

class wallpaper:
    def __init__(self, filename):
        self.keyboard = keyboard.Keyboard()
        self.image_obj = self.load_image(filename)
        #TODO: redraw works, but is a bit messy. Do I really need a keyboard
        # object?
        self.canvas = appuifw.Canvas(event_callback = self.keyboard.handle_event,
                                     redraw_callback = self.event_redraw)
    #Call this method to display the wallpaper
    def display(self):
        self.canvas.clear()
        tmp = self.calc_position()
        self.canvas.blit(self.image_obj, target = tmp)
    #Redraw function, used as a callback for canvas events.
    def event_redraw(self, other):
        self.display()
    #Calculates position where to blit the image in Canvas
    def calc_position(self):
        #this magic value works for N93, E61, N70
        target = [0, -10]
        return target
    #Calculates aspect ratio and resize original image
    def load_image(self, filename):
        canvas = appuifw.Canvas(None, None)
        screen_size = canvas.size
        canvas = None
        border_perc = None
        img = graphics.Image.open(filename)
        ratio_x = screen_size[0]/float(img.size[0])
        ratio_y = screen_size[1]/float(img.size[1])
        print [ratio_x, ratio_y]
        scaled = img.resize([(img.size[0] * ratio_x),
                                 (img.size[1] * ratio_y)], keepaspect = 1)
        return scaled
    #Auxiliar function, returns screensize
    def screen_size(self):
        return sysinfo.display_pixels()

#Example of use
# obj = wallpaper()
# appuifw.body = obj.canvas
# obj.display()

