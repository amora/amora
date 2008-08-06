# Author: Adenilson Cavalcanti
# email: adenilson.silva@indt.org.br
#        savagobr@yahoo.com
#        Copyright 2007
# About: a bluetooth class, it scans for local devices, connects
# with them and has methods to write data.

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
    #Default constructor
    def __init__(self):
        self.port = None
        self.sock = None
        self.byte_read = 8192
    #This method scans for bluetooth devices and create a socket
    #with a user selected device.
    def connect(self):
        self.port = None
        #Close remaining connections
        if self.sock != None:
            self.sock.close()
            self.sock = None
        #Create new socket
        self.sock = socket.socket(socket.AF_BT, socket.SOCK_STREAM)
        addr,services = socket.bt_discover()
        print "Discovered: %s, %s"%(addr,services)
        if len(services) > 0:
            choices = services.keys()
            choices.sort()
            choice = appuifw.popup_menu([unicode(services[x]) + ": " + x
                                       for x in choices], u'Choose port:')
            self.port = services[choices[choice]]
        else:
            self.port = services[services.keys()[0]]
        address = (addr, self.port)
        print "Connecting to " + str(address) + "...",
        self.sock.connect(address)
        print "OK."
    #Reads a byte stream from socket and writes in a file (you should
    #provide the file handler)
    def readline(self, fout):
        if self.port == None:
            appuifw.note(u'Invalid port!')
            return
        bytes = 0
        size = 0
        ch = 0
        try:
            size = int(self.sock.recv(10))
            while bytes < size:
                ch = self.sock.recv(self.byte_read)
                fout.write(ch)
                bytes += len(ch)
            fout.close()
        except:
            print 'Got exception!!'
    #Writes a command line, adding a newline at end of string
    def write_line(self, command):
        if self.port != None:
            self.sock.send(command + '\n');
    #Closes bluetooth socket
    def close(self):
        if self.sock != None:
            self.sock.close()
            self.port = None
            self.sock = None



