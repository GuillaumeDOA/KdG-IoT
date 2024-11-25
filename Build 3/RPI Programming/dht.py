import adafruit_dht
import board
from time import sleep

dht = adafruit_dht.DHT22(pin=board.D18)

while True:
    try:
        # Print the values to the serial port
        temperature_c = dht.temperature
        temperature_f = temperature_c * (9 / 5) + 32
        humidity = dht.humidity
        print(
            "Temp: {:.1f} F / {:.1f} C    Humidity: {}% ".format(
                temperature_f, temperature_c, humidity
            )
        )

    except RuntimeError as error:
        # Errors happen fairly often, DHT's are hard to read, just keep going
        print(error.args[0])
        sleep(2.0)
        continue
    except Exception as error:
        dht.exit()
        raise error

    sleep(2.0)