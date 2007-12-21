# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: Help display window. This class will load a text file
# and display it inside a appuifw.Text widget.

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

class helpwindow:
    text = None
    #Default constructor
    def __init__(self):
        self.text = appuifw.Text()
        self.text.style = appuifw.STYLE_BOLD
    #Clean method, purges text contents
    def clear(self):
        self.text.clear()
    #Read method, will read a text file contents into widget
    def read(self, file_path):
        fin = open(file_path)
        for i in fin:
            self.text.add(u'' + i)
    #Access to widget object
    def widget(self):
        return self.text
    #Displays widget object in application body
    def display(self):
        appuifw.app.body = self.text

def exit_key_handler():
    app_lock.signal()


# obj = helpwindow()
# obj.read('E:\\Python\\data\\conn_help.txt')

# app_lock = e32.Ao_lock()
# appuifw.app.title = u'Test'
# #appuifw.body = obj.widget()
# obj.display()
# appuifw.app.exit_key_handler = exit_key_handler
# app_lock.wait()
