# Author: Adenilson Cavalcanti
# email: savagobr@yahoo.com
#        Copyright 14-02-2008
# About: Bluetooth scanner using D-Bus. I got to write this one because
# it seems that BlueZ bluetooth.lookup_name doesn't work within the
# Nokia N810 internet tablet.
# The code was based on http://wiki.bluez.org/wiki/HOWTO/DiscoveringDevices
# and on E17 CVS sample named 'test-bt-search-devices.py'

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

import dbus
import e_dbus
import ecore

def disc_completed_signal():
    print 'Signal: DiscoveryCompleted()'
    ecore.idler_add(ecore.main_loop_quit)

class dbus_bluetooth():
    def __init__(self):
        self.main_loop = e_dbus.DBusEcoreMainLoop()
        self.bus = dbus.SystemBus(mainloop=self.main_loop)
    def set_callback(self, f_callback):
        self.bus.add_signal_receiver(f_callback, 'RemoteNameUpdated',
                                     'org.bluez.Adapter', 'org.bluez',
                                     '/org/bluez/hci0')
        self.bus.add_signal_receiver(disc_completed_signal,
                                     'DiscoveryCompleted', 'org.bluez.Adapter',
                                     'org.bluez', '/org/bluez/hci0')
        self.obj = self.bus.get_object('org.bluez', '/org/bluez/hci0')
        self.adapter = dbus.Interface(self.obj, 'org.bluez.Adapter')
    def discover(self):
        try:
            self.adapter.DiscoverDevices()
            ecore.main_loop_begin()
        except:
            print u'Cannot find devices: check bluetooth dongle!'
            return

# def test(address, name):
#         print 'Got a device: (%s, %s)' % (address, name)

# obj = dbus_bluetooth()
# obj.set_callback(test)
# obj.discover()
