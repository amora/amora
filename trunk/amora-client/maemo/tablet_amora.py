#!/usr/bin/env python

import ecore
import ecore.evas
import edje
import edje.decorators
import evas
import sys
import os

# constants
WIDTH_DFL = 800
HEIGHT_DFL = 480
FPS_DFL = 50

APP_TITLE = "Amora Tablet Client"
APP_WM_INFO = ("Amora Tablet Client", "amora-tablet-client")

class AmoraView():
	def __init__(self, ecanvas):
		self.ee = ecanvas.ee

		try:
			edje_file = os.path.splitext(sys.argv[0])[0] + ".edj"
			self.main_group = edje.Edje(self.ee.evas, file=edje_file, group="main")
		except edje.EdjeLoadError, e:
			raise SystemExit("Error loading %s (%s)" % (edje_file, e))

		self.main_group.size = self.ee.evas.size
		self.ee.data["main"] = self.main_group

		self.main_group.show()
		self.main_group.focus = True		


class AmoraCanvas():
	def  __init__(self, size):

		# checks if 16bits support on X11 is enabled
		if ecore.evas.engine_type_supported_get("software_x11_16") and \
			'-x11' not in sys.argv:
 			engine = ecore.evas.SoftwareX11_16
		else:
			engine = ecore.evas.SoftwareX11

		self.ee = engine(w=size[0], h=size[1])
		self.ee.callback_delete_request = self.on_delete_request
		self.ee.callback_resize = self.on_resize
		self.ee.fullscreen = False
		self.ee.size = size;
		self.ee.title = APP_TITLE
		self.ee.name_class = APP_WM_INFO

		self.ee.show()			

	def on_resize(self, ee):
		x, y, w, h = ee.evas.viewport
		size = (w, h)
		for key in ee.data.keys():
			ee.data[key].size = size

	def on_delete_request(self, ee):
		ecore.main_loop_quit()


if  __name__  == "__main__":
	from optparse import OptionParser

	def parse_size(option, opt, value, parser):
		try:
			w, h = value.split("x")
			w = int(w)
			h = int(h)
		except Exception, e:
			raise optparse.OptionValueError("Invalid option: %s" % option)
		parser.values.size = (w, h)

	opt = OptionParser(usage="usage: %prog [options]")
	opt.add_option("-s", "--size", type="string", metavar="WxH | fs",
					action="callback", callback=parse_size,
					default=(WIDTH_DFL, HEIGHT_DFL),
					help="Show application in window size WxH or fullscreen, default=%default")

	opt.add_option("-f", "--fps", type="int", default=FPS_DFL,
					help="Frames per second, default=%default") 


	options, args = opt.parse_args()
	
	edje.frametime_set(1.0/options.fps)

	canvas = AmoraCanvas(size=options.size)

	view = AmoraView(canvas)

	ecore.main_loop_begin();	

