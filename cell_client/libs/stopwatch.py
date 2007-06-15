# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
# About: A stopwatch, used to control timer for presentation.
#  I took this code from http://snippets.dzone.com/posts/show/776
#  and I'm not sure who is the writter of it.

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
    def __init__(self):
        appuifw.app.screen = 'full'
        self.canvas = appuifw.Canvas(self.update)
        appuifw.app.body = self.canvas
        self.canvas.bind(key_codes.EKeySelect, self.toggle)
        self.update()
    def update(self, rect = None, x = 0, y = 20):
        if self.running:
            self.elap = time.clock() - self.time_start
            e32.ao_sleep(0.05, self.update)
        t = self.elap
        min = int(t / 60)
        sec =  int(t - min*60)
        hsec = int((t - min*60 - sec)*100)
        self.canvas.clear()
        self.canvas.text((x, y), u"%02d:%02d:%02d" % (min,sec,hsec), font='title',
                         fill = 0x0000FF)
    def toggle(self):
        if self.running:
            self.running = 0
            self.elap = time.clock() - self.time_start
        else:
            self.running = 1
            self.time_start = time.clock() - self.elap
            self.update()
    def reset(self):
        self.running = 0
        self.elap = 0.0
        self.update()


sw = stopwatch()
lock = e32.Ao_lock()
appuifw.app.menu = [(u'Reset', sw.reset), (u'Close', lock.signal)]
appuifw.app.exit_key_handler = lock.signal
lock.wait()
