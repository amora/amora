
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

import appuifw
import e32
from keyboard import *
from bt_client import *
from wallpaper import *

class application:
    mouse_x = 400
    mouse_y = 400
    delta = 10
    wallpaper = None
    presentation = None
    running = 0
    bt = None
    keyboard = None
    #Add another object for 'options' window
    def __init__(self):
        appuifw.app.title = u'P4X'
        appuifw.app.menu = [(u'connect', self.__connect), (u'start', self.start)]
                            #(u'options', lambda:None)]
        appuifw.app.exit_key_handler = self.quit
        #Cleanup the screen
        self.wallpaper = wallpaper()
        appuifw.app.body = self.wallpaper.canvas
        self.wallpaper.display()
    #Exit function
    def quit(self):
        self.running = 0
        self.bt.close()
        appuifw.app.set_exit()
    #Private function, will try to connect with a server PC
    #using bluetooth
    def __connect(self):
        self.bt = bt_client()
        self.bt.connect()
    #Start presentation mode
    def start(self):
        #Presentation display
        if self.keyboard == None:
            self.keyboard = Keyboard()
        if self.presentation == None:
            self.presentation = appuifw.Canvas(event_callback =
                                               self.keyboard.handle_event,
                                               redraw_callback = None)
        self.running = 1
        appuifw.app.body = self.presentation
        appuifw.app.exit_key_handler = self.quit
        if self.keyboard.is_down(EScancode6):
            print u'RIGHT'
            self.bt.write_line(u'RIGHT')
        elif self.keyboard.is_down(EScancode4):
            print u'LEFT'
            self.bt.write_line(u'LEFT')
        elif self.keyboard.is_down(EScancode2):
            print u'UP'
            self.bt.write_line(u'UP')
        elif self.keyboard.is_down(EScancode8):
            print u'DOWN'
            self.bt.write_line(u'DOWN')
        elif self.keyboard.is_down(EScancodeUpArrow):
            print u'MOUSE_UP'
            self.mouse_y = self.mouse_y - self.delta
            self.bt.write_line(u'MOUSE_MOVE')
            self.bt.write_line(str(self.mouse_x))
            self.bt.write_line(str(self.mouse_y))
        elif self.keyboard.is_down(EScancodeDownArrow):
            print u'MOUSE_DOWN'
            self.mouse_y = self.mouse_y + self.delta
            self.bt.write_line(u'MOUSE_MOVE')
            self.bt.write_line(str(self.mouse_x))
            self.bt.write_line(str(self.mouse_y))
        elif self.keyboard.is_down(EScancodeLeftArrow):
            print u'MOUSE_LEFT'
            self.mouse_x = self.mouse_x - self.delta
            self.bt.write_line(u'MOUSE_MOVE')
            self.bt.write_line(str(self.mouse_x))
            self.bt.write_line(str(self.mouse_y))
        elif self.keyboard.is_down(EScancodeRightArrow):
            print u'MOUSE_RIGHT'
            self.mouse_x = self.mouse_x + self.delta
            self.bt.write_line(u'MOUSE_MOVE')
            self.bt.write_line(str(self.mouse_x))
            self.bt.write_line(str(self.mouse_y))





