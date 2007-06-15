import graphics
import appuifw
import keyboard

class wallpaper:
    def __init__(self):
        self.keyboard = keyboard.Keyboard()
        self.image_obj = self.load_image()
        #TODO: redraw works, but is a bit messy.
        self.canvas = appuifw.Canvas(event_callback = self.keyboard.handle_event,
                                     redraw_callback = self.event_redraw)
    def display(self):
        self.canvas.clear()
        tmp = self.calc_position()
        self.canvas.blit(self.image_obj, target = tmp)
    def event_redraw(self, other):
        self.display()
    #Calculates position where to blit the image in Canvas
    def calc_position(self):
        #this magic value works for N93, E61, N70
        target = [0, -10]
        return target
    #Calculates aspect ratio and resize original image
    def load_image(self):
        canvas = appuifw.Canvas(None, None)
        screen_size = canvas.size
        canvas = None
        border_perc = None
        #FIXME: location dependent code!
        img = graphics.Image.open('E:\\python\\imgs\\wallpaper_wide.jpg')
        ratio_x = screen_size[0]/float(img.size[0])
        ratio_y = screen_size[1]/float(img.size[1])
        print [ratio_x, ratio_y]
        scaled = img.resize([(img.size[0] * ratio_x),
                                 (img.size[1] * ratio_y)], keepaspect = 1)
        return scaled


# obj = wallpaper()
# appuifw.body = obj.canvas
# obj.display()

