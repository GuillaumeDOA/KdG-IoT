from gpiozero import Button, LED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep


factory = PiGPIOFactory(host='10.6.121.220')
led = LED(4, pin_factory=factory)
button = Button(17, pin_factory=factory)

led.source = button

while True:
    sleep(1)
