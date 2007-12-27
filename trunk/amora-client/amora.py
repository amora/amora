# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: Hold the application loop.

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

AMORA_PATH1 = "\\python\\"
AMORA_PATH2 = "\\Data\\python\\"

def get_path():
    drives_list = e32.drive_list()
    #Gives preference to load from drive 'E:'
    drives_list.reverse()
    for drive in [str(x) for x in drives_list]:
        if os.path.isfile(os.path.join(drive, AMORA_PATH1, "amora.py")):
            return os.path.join(drive, AMORA_PATH1)
        elif os.path.isfile(os.path.join(drive, AMORA_PATH2, "amora.py")):
            return os.path.join(drive, AMORA_PATH2)

full_path = get_path()
sys.path.append(os.path.join(full_path, "libs"))
from application import *

app = application(full_path)

flag = 1
while flag:
    if (app.running == 1):
        app.start()
    elif app.running == -1:
        flag = 0
    e32.ao_yield()
