# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: The main application class, will hold event processing
# code as also objects to represent windows.
# TODO:
# - slideshow class with icon animation
# - refactor continuous keypress code (and move it to 'application' and
# 'keyboard' class)



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
import time
from keyboard import *
from bt_client import *
from wallpaper import *
from helpwindow import *
from configure import *
from stopwatch import *

class application:
    mouse_x = 400
    mouse_y = 400
    delta = 2
    wallpaper = None
    presentation = None
    help_screen = None
    running = 0
    bt = None
    keyboard = None
    configuration = None
    img = None
    click_and_screen = False
    clock = None
    #Sets default window title
    def __window_title(self):
        appuifw.app.title = u'Amora'

    # Pressable keys allow continuous key-press
    pressablekeys = {EScancode6: 'RIGHT', EScancode4: 'LEFT'}

    #TODO: Add another object for 'options' window
    def __init__(self, app_path):
        self.path = app_path
        appuifw.app.title = u'Amora'
                            #(u'options', lambda:None)]
        appuifw.app.exit_key_handler = self.quit
        self.configuration = configure()
        self.clock = stopwatch()
        self.clock.set_msec(False)
        #Amora puts the stopwatch at top left of the screen
        self.clock.magic_delta = 0
        self.reset()
    #Reset application to its initial state
    def reset(self):
        self.__window_title()
        appuifw.app.screen = 'full'
        appuifw.app.menu = [(u'Search devices', self.__connect),
                            (u'Configuration', self.__configuration),
                            (u'Help', self.__help),
                            (u'About', self.__about),
                            (u'Exit', self.quit)]
        if self.keyboard != None:
            self.keyboard = None
        if self.presentation != None:
            self.presentation = None
        self.running = 0
        #Cleanup the screen
        if is_widescreen():
            self.wallpaper = wallpaper(
					self.path+'imgs\\wallpaper_wide_off.jpg')
        else:
            self.wallpaper = wallpaper(self.path+'imgs\\wallpaper_full_off.jpg')
        #TODO: move this code to wallpaper.display() method (like
        #helpwindow.display() method)
        appuifw.app.body = self.wallpaper.canvas
        self.wallpaper.display()
        self.img = None
        self.click_and_screen = False
        #Resets timer clock
        self.clock.reset()
    #Exit function
    def quit(self):
        self.running = -1
        if self.bt != None:
            print u'EXIT'
            self.bt.write_line(u'CONN_CLOSE')
            self.bt.close()
        appuifw.app.set_exit()
    #Call configuration dialog
    #TODO: implement method.
    def __configuration(self):
        self.configuration.display()
        self.delta = self.configuration.mouse_delta
        self.clock.set_msec(self.configuration.stop_msec)
        self.__window_title()
    #Load and display keymap
    def __display_keymap(self):
        try:
            if is_widescreen():
                self.img = graphics.Image.open(self.path + 'imgs\\keymap_wide.jpg')
            else:
                self.img = graphics.Image.open(self.path + 'imgs\\keymap_full.jpg')
            self.presentationdisplay()
        except:
            appuifw.note(u'Error displaying help!')
    #Call Help dialog
    #TODO: implement help for other application states: connected, started.
    def __help(self):
        if self.help_screen == None:
            self.help_screen = helpwindow()
        if self.running == 0:
            appuifw.app.screen = 'large'
            self.help_screen.clear()
            self.help_screen.read(self.path+'data\\conn_help.txt')
            self.help_screen.display()
        elif self.running == 2:
            appuifw.app.screen = 'large'
            self.help_screen.clear()
            self.help_screen.read(self.path+'data\\start_help.txt')
            self.help_screen.display()
        elif self.running == 1:
            appuifw.note(u'Joystick moves the mouse cursor')
            self.__display_keymap()
    #About box displaying
    def __about(self):
        try:
            #Restore window state if it has changed
            if appuifw.app.screen == 'large':
                appuifw.app.screen = 'full'
            #First about screen
            if is_widescreen():
                self.wallpaper = wallpaper(self.path + 'imgs\\about_wide01.png')
            else:
                self.wallpaper = wallpaper(self.path + 'imgs\\about_full01.png')
            appuifw.app.body = self.wallpaper.canvas
            self.wallpaper.display()
            #Second about screen
            if is_widescreen():
                self.wallpaper = wallpaper(self.path + 'imgs\\about_wide02.png')
            else:
                self.wallpaper = wallpaper(self.path + 'imgs\\about_full02.png')
            time.sleep(3.0)
            appuifw.app.body = self.wallpaper.canvas
            self.wallpaper.display()
            #Displays initial wallpaer
            if is_widescreen():
                self.wallpaper = wallpaper(self.path+'imgs\\wallpaper_wide_off.jpg')
            else:
                self.wallpaper = wallpaper(self.path+'imgs\\wallpaper_full_off.jpg')
            time.sleep(3.0)
            appuifw.app.body = self.wallpaper.canvas
            self.wallpaper.display()
        except:
                appuifw.note(u'Cannot find about box image!')
    #Time based callback, offer the option to start controlling the desktop
    #without clicking in submenu option
    def __start_control(self):
        conn_options = [u'now!', u'later...']
        index = appuifw.popup_menu(conn_options, u'Start control')
        if index == 0:
            self.start()
    #Private function, will try to connect with a server PC
    #using bluetooth
    def __connect(self):
        self.bt = bt_client()
        self.bt.connect()
        #Prepare server for screenshot mode
        if self.bt.sock != None:
            try:
                if self.configuration.screenshot:
                    self.bt.write_line(u'SCREEN_MODE_ON')
                    self.bt.write_line(u'SCREEN_RESOLUTION')
                    if self.configuration.rotate == False:
                        self.bt.write_line(str(self.wallpaper.screen_size()[0]))
                        self.bt.write_line(str(self.wallpaper.screen_size()[1]))
                    else:
                        self.bt.write_line(str(self.wallpaper.screen_size()[1]))
                        self.bt.write_line(str(self.wallpaper.screen_size()[0]))
                else:
                    self.bt.write_line(u'SCREEN_MODE_OFF')
                #Restore window state if it has changed
                if appuifw.app.screen == 'large':
                    appuifw.app.screen = 'full'
            except:
                appuifw.note(u'Connection is over, server down!')
                self.bt.close()
                self.bt = None
                self.reset()
                return
        #Proceed with normal flow
        appuifw.app.menu = [(u'Start', self.start),
                            (u'Disconnect', self.__reset),
                            (u'Help', self.__help),
                            (u'Exit', self.quit)]
        #Cleanup the screen
        if is_widescreen():
            self.wallpaper = wallpaper(self.path+'imgs\\wallpaper_wide.jpg')
        else:
            self.wallpaper = wallpaper(self.path+'imgs\\wallpaper_full.jpg')
        #TODO: move this code to wallpaper.display() method (like
        #helpwindow.display() method)
        appuifw.app.body = self.wallpaper.canvas
        self.wallpaper.display()
        self.running = 2
        #Wait 2s and ask if user want to start sending commands
        e32.ao_sleep(2, self.__start_control)
    #Reset connection, restore initial GUI menu elements
    def __reset(self):
        if self.bt != None:
            self.bt.write_line(u'CONN_CLOSE')
            self.bt.close()
        self.reset()
    #Ugly hack to print screenshot
    def presentationdisplay(self, a_img = None):
        if self.presentation != None:
            if self.configuration.stopwatch:
                self.clock.update(self.presentation)
            if a_img != None:
                self.img = a_img

            if self.img != None:
                self.presentation.clear()
                self.presentation.blit(image = self.img, target = (0,0), scale = 1)
        else:
            appuifw.note(u'Error, cannot display presentation.')
        #Updates stopwatch
    #Sets click and screen mode
    def __click_screen(self):
        if self.click_and_screen:
            self.click_and_screen = False
            appuifw.note(u'Click and screen off.')
        else:
            self.click_and_screen = True
            appuifw.note(u'Click and screen on.')
    #This private function send request, read, blit screenshot
    #TODO:
    # - make it run in a distinct thread (now it hangs for 2s)
    # - discover a way to transfer image without using files
    def __take_screenshot(self):
        try:
            if self.configuration.rotate:
                self.bt.write_line(u'SCREEN_ROTATE')
            else:
                self.bt.write_line(u'SCREEN_NORMAL')
            fout = open(self.path+'test.png', 'w')
            self.bt.write_line(u'SCREEN_TAKE')
            try:
                res = self.bt.readline(fout)
            except:
                appuifw.note(u'Failed reading!')
            self.img = graphics.Image.open(self.path+'test.png')
            self.presentationdisplay()
        except:
            appuifw.note(u'Cannot transfer thumbnail!')
            raise 'I\'m done here!'

    # return the pressed keys escancode from the pressablekeys
    # contribution by Makoto Sugano
    def pressablekeys_pressed(self):
        result = 0
        for i in self.pressablekeys.keys():
            if self.keyboard.pressed(i):
                result = i
                break
        return result

    # return the is_down keys escancode from the pressablekeys.
    # contribution by Makoto Sugano
    def pressablekeys_isdown(self):
        result = 0
        for i in self.pressablekeys.keys():
            if self.keyboard.is_down(i):
                result = i
                break
        return result

    # process the keys in the pressablekeys
    # contribution by Makoto Sugano
    def pressablekeys_process(self, escancode):
        try:
            print self.pressablekeys[escancode]
            self.bt.write_line(self.pressablekeys[escancode])
        except:
            appuifw.note(u'Connection is over, server down!')
            self.bt.close()
            self.bt = None
            self.reset()

    # process the keys, not in the pressablekeys
    def otherkeys_process(self):
        #XXX: fix to make slide control work, I should write a code
        # to process continously pressing given a time out value.
        try:
            #Mouse move event processing
            if self.keyboard.is_down(EScancodeUpArrow):
                print u'MOUSE_UP'
                self.mouse_y = -self.delta
                self.bt.write_line(u'MOUSE_MOVE')
                self.bt.write_line(str(0))
                self.bt.write_line(str(self.mouse_y))
            elif self.keyboard.is_down(EScancodeDownArrow):
                print u'MOUSE_DOWN'
                self.mouse_y = self.delta
                self.bt.write_line(u'MOUSE_MOVE')
                self.bt.write_line(str(0))
                self.bt.write_line(str(self.mouse_y))
            elif self.keyboard.is_down(EScancodeLeftArrow):
                print u'MOUSE_LEFT'
                self.mouse_x = -self.delta
                self.bt.write_line(u'MOUSE_MOVE')
                self.bt.write_line(str(self.mouse_x))
                self.bt.write_line(str(0))
            elif self.keyboard.is_down(EScancodeRightArrow):
                print u'MOUSE_RIGHT'
                self.mouse_x = self.delta
                self.bt.write_line(u'MOUSE_MOVE')
                self.bt.write_line(str(self.mouse_x))
                self.bt.write_line(str(0))
            #Mouse click event processing
            elif self.keyboard.pressed(EScancodeSelect):
                print u'MOUSE CLICK'
                self.bt.write_line(u'MOUSE_BUTTON_LEFT')
                self.bt.write_line(u'MOUSE_BUTTON_PRESS')
                self.bt.write_line(u'MOUSE_BUTTON_RELEASE')
                if self.click_and_screen:
                    #FIXME: synchronization between click and new slide
                    #display will require a change in protocol or IPC
                    #with OpenOffice/Acrobat/etc
                    print u'Sleeping for ' + str(self.configuration.screenshot_wait)
                    time.sleep(self.configuration.screenshot_wait)
                    self.__take_screenshot()
            elif self.keyboard.pressed(EScancode1):
                print u'MOUSE CLICK_HOLD'
                if self.press_flag == 0:
                    self.bt.write_line(u'MOUSE_BUTTON_LEFT')
                    self.bt.write_line(u'MOUSE_BUTTON_PRESS')
                    self.press_flag = 1
                else:
                    self.press_flag = 0
                    self.bt.write_line(u'MOUSE_BUTTON_LEFT')
                    self.bt.write_line(u'MOUSE_BUTTON_RELEASE')
#            elif self.keyboard.pressed(EScancode2):
# I'm going to use mouse middle button (number 2) to trigger for screenshot
#                 print u'MOUSE CLICK MIDDLE'
#                 self.bt.write_line(u'MOUSE_BUTTON_MIDDLE')
#                 self.bt.write_line(u'MOUSE_BUTTON_PRESS')
#                 self.bt.write_line(u'MOUSE_BUTTON_RELEASE')
            elif self.keyboard.pressed(EScancode3):
                print u'MOUSE CLICK RIGHT'
                self.bt.write_line(u'MOUSE_BUTTON_RIGHT')
                self.bt.write_line(u'MOUSE_BUTTON_PRESS')
                self.bt.write_line(u'MOUSE_BUTTON_RELEASE')
            elif self.keyboard.pressed(EScancode7):
                print u'SCROLL_UP'
                self.bt.write_line(u'MOUSE_SCROLL_UP')
            elif self.keyboard.pressed(EScancodeStar):
                print u'SCROLL_DOWN'
                self.bt.write_line(u'MOUSE_SCROLL_DOWN')
            #Special keys event processing
            elif self.keyboard.pressed(EScancode9):
                print u'ESC'
                self.bt.write_line(u'ESC')
            elif self.keyboard.pressed(EScancode0):
                print u'SPACE'
                self.bt.write_line(u'SPACE')
            elif self.keyboard.pressed(EScancode5):
                print u'ENTER'
                self.bt.write_line(u'ENTER')
            elif self.keyboard.pressed(EScancodeBackspace):
                print u'DEL'
                self.bt.write_line(u'DEL')
            elif self.keyboard.pressed(EScancode8):
                print u'SLIDESHOW - F5'
                self.bt.write_line(u'SLIDESHOW')
            elif self.keyboard.pressed(EScancodeHash):
                print u'Fullscreen - F'
                self.bt.write_line(u'FULLSCREEN')
            if self.configuration.screenshot and self.keyboard.pressed(EScancode2):
                self.__take_screenshot()
        except:
            appuifw.note(u'Connection is over, server down!')
            self.bt.close()
            self.bt = None
            self.reset()

    #Start presentation mode
    def start(self):
        #Creates presentation display if it already doesn't exist.
        if self.keyboard == None:
            self.keyboard = Keyboard()
        if self.presentation == None:
            self.presentation = appuifw.Canvas(event_callback = self.keyboard.handle_event, redraw_callback = self.presentationdisplay)
                                               #redraw_callback = None)

            appuifw.app.screen = 'full'
            appuifw.app.body = self.presentation
        #First time the function is called, change menu and display keymap
        if self.running == 0 or self.running == 2:
            if self.configuration.stopwatch:
                appuifw.app.menu = [ self.clock.create_submenu_lst(),
                                     (u'Disconnect', self.__reset),
                                     (u'Auto screen', self.__click_screen),
                                     (u'Help', self.__help),
                                     (u'Exit', self.quit)]
                self.clock.toggle()
            else:
                appuifw.app.menu = [ (u'Disconnect', self.__reset),
                                     (u'Auto screen', self.__click_screen),
                                     (u'Help', self.__help),
                                     (u'Exit', self.quit)]

            self.press_flag = 0
            self.__display_keymap()
        self.running = 1
        appuifw.app.exit_key_handler = self.quit


