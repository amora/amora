## Author: Adenilson Cavalcanti
## email: adenilson.silva@indt.org.br
##        Copyright 2007
## About: a keyboard class for key event handling, based on
## code written by Jurgen Scheible

import appuifw
import e32
from key_codes import *


class Keyboard(object):
    "This class handles keyboard events"
    #Default constructor, you can pass a callback to it for
    #each key press.
    def __init__(self, onevent = lambda:None):
        self.state = {}
        self.downs = {}
        self.event = onevent
    #Return if a given key was pressed (internal use)
    def __is_down(self, scancode):
        return self.state.get(scancode,0)
    #Function member, receives events (it is called back
    #by appuifw.Canvas) for both key downs + ups. It will store
    #key events for later checks, using 'self.downs'.
    def handle_event(self, event):
        if event['type'] == appuifw.EEventKeyDown:
            code = event['scancode']
            #Case not recorded, save the code for this key
            if not self.__is_down(code):
                self.downs[code] = self.downs.get(code, 0) + 1
            self.state[code] = 1
        elif event['type'] == appuifw.EEventKeyUp:
            self.state[event['scancode']] = 0
        #When using default constructor *without* parameters
        #(i.e. obj = Keyboard()), it does nothing.
        self.event()
    #Use this to check if a button was pressed.
    def pressed(self,scancode):
        if self.downs.get(scancode,0):
            self.downs[scancode]-=1
            return True
        return False


