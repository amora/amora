#!/usr/bin/env python

import ecore
import ecore.evas
import edje
import edje.decorators
import evas
import sys
import os

WIDTH_DFL = 800
HEIGHT_DFL = 480
FRAMERATE_DFL = 20
FULLSCREEN_DFL = False
APP_TITLE = "Amora Tablet Client"
APP_WM_INFO = ("Amora Tablet Client", "amora-tablet-client")

class AmoraTabletClient(edje.Edje):
    #Default constructor
    def  __init__(self, canvas, width = WIDTH_DFL, height = HEIGHT_DFL,
                 fullscreen = FULLSCREEN_DFL, framerate = FRAMERATE_DFL):
        edje.Edje.__init__(self, canvas)
        self.text = []
        edje_file = os.path.splitext(sys.argv[0])[0] + ".edj"
        self.file_set(edje_file, "main")



if ecore.evas.engine_type_supported_get("software_x11_16") and \
   '-x11' not in sys.argv:
    ee = ecore.evas.SoftwareX11_16(w=WIDTH_DFL, h=HEIGHT_DFL)
else:
    ee = ecore.evas.SoftwareX11(w=WIDTH_DFL, h=HEIGHT_DFL)

canvas = ee.evas

obj = AmoraTabletClient(canvas)
obj.size = canvas.size
obj.focus = True
obj.show()

ee.data["main"] = obj
ee.title = APP_TITLE
ee.name_class = APP_WM_INFO
ee.fullscreen = FULLSCREEN_DFL
ee.show()

ecore.main_loop_begin();
