
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
