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

    def get_device_name(self, device_mac):
        '''
        Given the BT 'mac' address, returns device name.
        Combine this function with 'device_list' to interactively display
        device names
        '''
        name = bluetooth.lookup_name(device_mac)
        return name




#Usage of class
# obj = bt()
# obj.device_list()
# for i in obj.devices:
#     print obj.get_device_name(i)

