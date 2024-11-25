from gpiozero import Button, LED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep

factory = PiGPIOFactory(host='10.6.121.220')
led = LED(19, pin_factory=factory)
button = Button(15, pin_factory=factory)

powerpin = led.on()
motion= False

while True:
    if not button.is_pressed:
        led.on()
        print("MOTION DETECTED")
        sleep(2)
    else:
        led.off()
        print("NO MOTION DETECTED")
    