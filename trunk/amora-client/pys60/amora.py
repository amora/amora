# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: Hold the application loop.
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
import sys
import os

AMORA_UID = u"ef0b4099"

AMORA_PATH1 = "\\python\\"
AMORA_PATH2 = "\\Data\\python\\"
AMORA_PATH3 = "\\Private\\ef0b4099\\"

# the initial timer is triggered, follow up the keys in the pressablekeys
# contribution by Makoto Sugano
def pressablekeys_followup():
    escancode_isdown = app.pressablekeys_isdown()
    if (escancode_isdown):
        timer.cancel()
        app.pressablekeys_process(escancode_isdown)
        timer.after(0.1, pressablekeys_followup)
    else:
        timer.cancel()



def get_path(app_name):
    drives_list = e32.drive_list()
    #Gives preference to load from drive 'E:'
    drives_list.reverse()
    for drive in [str(x) for x in drives_list]:
        if os.path.isfile(os.path.join(drive, AMORA_PATH1, app_name)):
            return os.path.join(drive, AMORA_PATH1)
        elif os.path.isfile(os.path.join(drive, AMORA_PATH2, app_name)):
            return os.path.join(drive, AMORA_PATH2)
        elif os.path.isfile(os.path.join(drive, AMORA_PATH3, app_name)):
            return os.path.join(drive, AMORA_PATH3)
    return None


full_path = get_path('default.py')
if full_path == None:
    full_path = get_path('amora.py')
    if full_path == None:
        appuifw.note(u'Using fallback path')
        full_path = u'C:\\' + u'Private' + u'\\' + AMORA_UID + u'\\'


sys.path.append(os.path.join(full_path, "libs"))
from application import *

app = application(full_path)

timer = e32.Ao_timer()

flag = 1
while flag:
    if (app.running == 1):
        escancode_pressed = app.pressablekeys_pressed()
        if (escancode_pressed):
            timer.cancel()
            app.pressablekeys_process(escancode_pressed)
            timer.after(1.5, pressablekeys_followup)
        else:
            app.otherkeys_process()
        #dont let screensaver activate
        if e32.inactivity() > 4.0:
            e32.reset_inactivity()
    elif app.running == -1:
        flag = 0
    e32.ao_yield()

