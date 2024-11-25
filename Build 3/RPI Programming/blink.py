from gpiozero import LED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep

factory = PiGPIOFactory(host='10.6.121.220')
led = LED(4, pin_factory=factory)

while True:
    led.on()
    sleep(1)
    led.off()
    sleep(1)