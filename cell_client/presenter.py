
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

sys.path.append('E:\\python\\libs\\')
import socket
import appuifw
import e32
from keyboard import *
from bt_client import *


def quit():
    global running
    running=0
    appuifw.app.set_exit()



bt = bt_client()
keyboard = Keyboard()

running = 1
canvas = appuifw.Canvas(event_callback = keyboard.handle_event, redraw_callback=None)
appuifw.app.body = canvas
appuifw.app.exit_key_handler = quit

bt.connect()
while running:
    if keyboard.pressed(EScancodeRightArrow):
        print u'RIGHT'
        bt.write_line(u'RIGHT')
    elif keyboard.pressed(EScancodeLeftArrow):
        print u'LEFT'
        bt.write_line(u'LEFT')
    elif keyboard.pressed(EScancodeUpArrow):
        print u'UP'
        bt.write_line(u'UP')
    elif keyboard.pressed(EScancodeDownArrow):
        print u'DOWN'
        bt.write_line(u'DOWN')
    e32.ao_yield()



bt.close()
