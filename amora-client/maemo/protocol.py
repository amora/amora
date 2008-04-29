# Author: Adenilson Cavalcanti
# email: savagobr@yahoo.com
#        Copyright 2008
# About: the protocol class, it sends the commands to amora server,
# uses a 'bt' class as communication channel.

'''
/*  Copyright (C) 2008  Adenilson Cavalcanti <savagobr@yahoo.com>
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

import btio

class protocol:
    def __init__(self, width = 800, height = 480):
        self.socket_obj = None
        #TODO: provide a way to programatically get the screen resolution
        self.width = width
        self.height = height

    def comm_channel(self, socket_obj):
        if self.socket_obj != None:
            self.socket_obj.close()
        self.socket_obj = socket_obj


    def connect(self):
        if self.socket_obj == None:
            print u'protocol: It needs a socket object!'
            return

        self.socket_obj.write_line(u'SCREEN_MODE_ON')
        self.socket_obj.write_line(u'SCREEN_RESOLUTION')
        self.socket_obj.write_line(str(self.width))
        self.socket_obj.write_line(str(self.height))

