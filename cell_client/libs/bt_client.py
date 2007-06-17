# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: a bluetooth class it scans for local devices, connects
# with them and has methods to write data.
# TODO: fix reading data method

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
import socket
import appuifw
import e32

class bt_client:
    def connect(self):
        self.sock = socket.socket(socket.AF_BT, socket.SOCK_STREAM)
        addr,services = socket.bt_discover()
        print "Discovered: %s, %s"%(addr,services)
        if len(services) > 0:
            choices = services.keys()
            choices.sort()
            choice = appuifw.popup_menu([unicode(services[x]) + ": " + x
                                       for x in choices], u'Choose port:')
            port = services[choices[choice]]
        else:
            port = services[services.keys()[0]]
        address = (addr,port)
        print "Connecting to " + str(address) + "...",
        self.sock.connect(address)
        print "OK."
#     def readline(self):
#         line=[]
#         self.sock.recv(100)
#         line.append(ch)
#         while 1:
#             ch=self.sock.recv(100)
#             if(ch=='\n'):
#                 break
#             line.append(ch)
#        return ''.join(line)
    def write_line(self, command):
        self.sock.send(command + '\n');
    def close(self):
        self.sock.close()



