from aw.gpio.H616 import * 
import time

led = Pwm(PC13.id)
led.frequency = 100
while 1:
    for i in range(1,1000,10):
        led.duty_cycle = i
        time.sleep(0.01)