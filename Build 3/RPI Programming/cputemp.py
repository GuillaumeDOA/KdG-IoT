from gpiozero import LEDBarGraph, CPUTemperature
from time import sleep

cpu = CPUTemperature()

print(f'Temperature: {cpu.temperature}°C')

graph = LEDBarGraph(19, 21, 20, 16, 13)
graph.source = cpu

while True:
    sleep(1)