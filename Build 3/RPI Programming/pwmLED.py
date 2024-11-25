from gpiozero import PWMLED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep

factory = PiGPIOFactory(host='10.6.121.220')
led = PWMLED(19, pin_factory=factory)

for i in range(11):
    led.value = i/10
    print('i: {}'.format(i/10))
    sleep(0.5)
    

for i in range(10,-1, -1):
    led.value = i/10
    print('i: {}'.format(i/10))
    sleep(0.5)

