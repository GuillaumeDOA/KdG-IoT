from gpiozero import LED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep

factory = PiGPIOFactory(host='10.6.121.220')
green = LED(19, pin_factory=factory)
red = LED(20, pin_factory=factory)
yellow = LED(21, pin_factory=factory)

red.off()
green.off()
yellow.off()

while True:
    red.on()
    sleep(5)
    red.off()
    green.on()
    sleep(5)
    green.off()
    yellow.on()
    sleep(2)
    yellow.off()