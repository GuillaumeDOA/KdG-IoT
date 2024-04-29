import requests

ADAFRUIT_KEY = "aio_ZEBj40c2V2xTekVRnuoaEHVlAuNa"

ADAFRUIT_HEADERS = {"X-AIO-Key": ADAFRUIT_KEY}
ADAFRUITIO_URL = (
    "https://io.adafruit.com/api/v2/Exotic/feeds/adafruit-io-demo.sensor/data"
)

respone = requests.get(f'{ADAFRUITIO_URL}/last', headers=ADAFRUIT_HEADERS)
respone = requests.get(f'{ADAFRUITIO_URL}/next', headers=ADAFRUIT_HEADERS)
print(respone.json())