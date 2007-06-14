import graphics
import appuifw
import keyboard

class wallpaper:
    def __init__(self, width = 320, height = 240):
        self.keyboard = keyboard.Keyboard()
        #TODO: redraw works, but is a bit messy.
        self.width = width
        self.height = height
        self.image_obj = graphics.Image.open('E:\\python\\imgs\\wallpaper.jpg')
        self.canvas = appuifw.Canvas(event_callback = self.keyboard.handle_event,
                                     redraw_callback = self.event_redraw)
    def display(self):
        self.canvas.clear()
        tmp = self.calc_position()
        self.canvas.blit(self.image_obj, target = tmp)
    def event_redraw(self, other):
        self.display()
    #FIXME: still doesn't work for N93
    def calc_position(self):
        x = 0; y = 0;
        if (self.width > self.image_obj.size[0]):
            x = (self.width - self.image_obj.size[0])/2
        if (self.height > self.image_obj.size[1]):
            y = (self.height - self.image_obj.size[1])/2
        return [x, y]

# obj = wallpaper()
# appuifw.body = obj.canvas
# obj.display()

