from gpiozero import LED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep

factory = PiGPIOFactory(host='10.6.121.181')
red = LED(17, pin_factory=factory)

while True:
    red.on()
    print("led aan")
    sleep(1)
    red.off()
    print("led uit")
    sleep(1)