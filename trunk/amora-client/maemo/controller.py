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

import ecore
import ecore.evas
import edje
import edje.decorators
import evas
import sys

APP_DATADIR = "/usr/share/amora/"

class Controller(object):
    def __init__(self, ecanvas):
        self.ee = ecanvas.ee

        if len(sys.argv) > 1:
            self.edje_file = 'controller.edj'
        else:
            # do not hardcode edje file path
            self.edje_file = APP_DATADIR + "controller.edj"

        try:
            self.main_group = edje.Edje(self.ee.evas,
                                        file=self.edje_file,
                                        group="main")
        except edje.EdjeLoadError, e:
            raise SystemExit("Error loading %s (%s)" %
                             (self.edje_file, e))

        self.main_group.size = self.ee.evas.size
        self.ee.data["main"] = self.main_group

        self.main_group.show()
        self.main_group.focus = True

