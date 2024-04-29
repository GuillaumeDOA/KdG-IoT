import requests
import random
import time

ADAFRUIT_KEY = "aio_ZEBj40c2V2xTekVRnuoaEHVlAuNa"

ADAFRUIT_HEADERS = {"X-AIO-Key": ADAFRUIT_KEY}
ADAFRUITIO_URL = (
    "https://io.adafruit.com/api/v2/Exotic/feeds/adafruit-io-demo.sensor/data"
)

respone = requests.post(ADAFRUITIO_URL, headers=ADAFRUIT_HEADERS, data={'value': random.randint(0, 4000)})