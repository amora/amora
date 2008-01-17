#!/usr/bin/env python

import ecore
import ecore.evas
import edje
import sys
import os

class main_app:
    #Default constructor
    def  __init__(self, width = 800, height = 480, fullscreen = True,
                 framerate = 20):
        f = ecore.evas.SoftwareX11
        ee = f(w = width, h = height)
        ee.fullscreen = fullscreen
        edje.frametime_set(1.0 / framerate)
        #Setup UI
        ee.title = "Amora tablet client"
        ee.name_class = ("amora_client", "amora_client")
        self.canvas = ee.evas
        self.f = f
        self.ee = ee
    #Loads an edje file
    def load_edje(self, path):
        try:
            print path
            self.edje_obj = edje.Edje(self.canvas, file=path, group="main")
        except Exception, e:
            raise SystemExit("Failed to load Edje file: %s" % path)



obj = main_app()

#edje_file_path = os.path.join(os.path.dirname(sys.argv[0]), "main_win.edj")
#obj.load_edje(edje_file_path)

