from src.pin import *

from aw._gpio import h616_gpio

class Pin(PIN_father):

    # self.setup(self.id, self.IN, pull_up_down=self.PUD_UP)
    def setup(self, id, dir, pull_up_down=None ):
        print("id=", id)
        print("dir=", dir)
        print("pull_up_down=", pull_up_down)
        pass

    # self.output(self.id, val)
    def output(self, id, val):
        print("id=", id)
        print("val=", val)

    # return GPIO.input(self.id)
    def input(self, id):
        print("id=", id)