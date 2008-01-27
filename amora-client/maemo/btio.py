# Author: Adenilson Cavalcanti
# email: savagobr@yahoo.com
#        Copyright 2008
# About: a bluetooth class, it scans for local devices, connects
# with them and has methods to write data.

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

import bluetooth

class bt:
    def __init__(self):
        self.socket = bluetooth.BluetoothSocket()

    def device_named_list(self):
        '''
        Return a list of tuples (device_name, BT 'mac' address).
        It does all in just 1 pass, but takes a while...
        '''
        result = []
        devices = bluetooth.discover_devices()
        for i in devices:
            result.append((bluetooth.lookup_name(i), i))
        self.all_devices = result
        return result

    def device_list(self):
        '''
        Returns nearby BT devices 'mac' addresses.
        For interactive interfaces, combine this with 'get_device_name'
        '''
        self.devices = bluetooth.discover_devices()
        return self.devices

    def get_device_name(self, device_mac, timeout = 20):
        '''
        Given the BT 'mac' address, returns device name.
        Combine this function with 'device_list' to interactively display
        device names
        '''
        name = bluetooth.lookup_name(device_mac)
        if name == None:
            print u'bt.get_device_name: Trying again...'
            name = bluetooth.lookup_name(device_mac, timeout)
            if name == None:
                print u'bt.get_device_name: Cannot get device name, giving up...'
        return name


    def connect(self, device_mac = '00:0C:41:E2:33:06', service_name = 'Amora: assistant'):
        '''
        Use this to connect to a given host.
        '''
        service_description = bluetooth.find_service(name = service_name,
                                                     address = device_mac)
        if service_description == []:
            print u'bt.connect: service not available'
            return -1

        port = service_description[0]['port']
        try:
            print u'bt.connect: trying to connect...'
            self.socket.connect((device_mac, port))
            print u'bt.connect: connected.'
        except:
            print u'bt.connect: Failed to connect!'

        return 0

    def close(self):
        try:
            print u'bt.close: trying to close socket...'
            self.socket.close()
            print u'bt.close: done.'
        except:
            print u'bt.close: Failed closing socket!'


#Usage of class
# obj = bt()
# obj.device_list()
# for i in obj.devices:
#     temp = obj.get_device_name(i)
#     print temp
#     if temp == 'zv64-0':
#         temp = obj.connect(i)
#         obj.close()

