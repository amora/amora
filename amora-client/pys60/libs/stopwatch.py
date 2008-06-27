# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: A stopwatch, used to control timer for presentation.
#  I took this code from http://snippets.dzone.com/posts/show/776
#  and I'm not sure who is the writter of it.
# TODO:
# - Font size can change according to cellphone Symbian version
# - The clean up rectangle should be a percentage of screen and *not*
# hard coded pixel dimensions

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
import key_codes
import graphics
import time
import e32

class stopwatch:
    running = 0
    time_start = None
    elap = 0.0
    acanvas = None
    x = 0
    y = 20
    blank_width = 85
    sleep_time = 0.2
    hsec_prec = True
    magic_delta = 25
    #Updates the clock in Canvas
    def update(self, canvas = None):
        if canvas != None:
            self.acanvas = canvas
        if self.running:
            self.elap = time.clock() - self.time_start
            e32.ao_sleep(self.sleep_time, self.update)
        t = self.elap
        min = int(t / 60)
        sec =  int(t - min*60)
        hsec = int((t - min*60 - sec)*100)
        if self.acanvas != None:
            if self.hsec_prec:
                self.acanvas.rectangle((self.x, 0,
                                        (self.x + self.blank_width),
                                        (self.y + 1)), fill = 0xFFFFFF)
                self.acanvas.text((self.x, self.y),
                                  u"%02d:%02d:%02d" % (min,sec,hsec),
                                  font='title',
                                  fill = 0x0000FF)
            else:
                self.acanvas.rectangle((self.x + self.magic_delta, 0,
                                        (self.x + self.blank_width),
                                        (self.y + 1)), fill = 0xFFFFFF)
                self.acanvas.text((self.x + self.magic_delta, self.y),
                                  u"%02d:%02d" % (min,sec),
                                  font='title',
                                  fill = 0x0000FF)
    #Turn off milisecond precision
    def set_msec(self, flag):
        if flag:
            self.sleep_time = 0.25
            self.hsec_prec = True
            self.blank_width = 85
        else:
            self.sleep_time = 1.0
            self.hsec_prec = False
            self.blank_width = 78
        if self.running:
            if self.acanvas != None:
                self.acanvas.clear()
    #Set/unset timer
    def toggle(self):
        if self.running:
            self.running = 0
            self.elap = time.clock() - self.time_start
        else:
            self.running = 1
            self.time_start = time.clock() - self.elap
            self.update()
    #Reset only the counter
    def reset_counter(self):
        self.elap = 0.0
        self.time_start = time.clock()
    #Resets all internal class data
    def reset(self):
        self.running = 0
        self.elap = 0.0
        self.time_start = None
        self.acanvas = None
        self.x = 0
        self.y = 20
    #Sets length of blanking retangle
    def set_blank_width(self, length):
        if length > 70:
            self.blank_width = length
    #Sets position where stopwatch is drawn
    def set_position(self, x, y):
        if (x > -1) and (y > 19):
            self.x = x
            self.y = y
    #Returns a menu subitem to control stopwatch
    def create_submenu_lst(self):
        sub_menu_lst = (u'Stopwatch',
                ((u'Reset', self.reset_counter),
                  (u'Toggle', self.toggle)
                  )
                )
        return sub_menu_lst


# def test(param):
#     global tc, clock
#     clock.update(tc)



# clock = stopwatch()
# clock.toggle()
# tc = appuifw.Canvas(test)
# appuifw.app.body = tc
# appuifw.app.screen = 'full'

# def _exit():
#     print 'exit'


# appuifw.app.menu = [ clock.create_submenu_lst(),
#                      (u'Exit', _exit)]


# #N95: Symbian 9.1, 240x320
# clock.set_position(140, 20)
# #E61: Symbian 9.0, 320x240
# clock.set_position(250, 20)
# #N93: Symbian 9.0, 240x320
# clock.set_position(160, 20)

