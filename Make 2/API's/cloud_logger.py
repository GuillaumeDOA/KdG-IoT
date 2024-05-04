import requests
from datetime import datetime, timedelta
import time

ADAFRUIT_KEY = "aio_ZEBj40c2V2xTekVRnuoaEHVlAuNa"

ADAFRUIT_HEADERS = {"X-AIO-Key": ADAFRUIT_KEY}
ADAFRUITIO_URL = (
    "https://io.adafruit.com/api/v2/Exotic/feeds/adafruit-io-demo.sensor/data"
)

NOTION_KEY = "secret_5ppGUsxLGKfWMw5OfD2KcrZiZMD9rIqSOKXXmicFXhs"

NOTION_HEADERS = {
    "Authorization": f"Bearer {NOTION_KEY}",
    "Content-Type": "application/json",
    "Notion-Version": "2022-06-28",
}

TELEGRAM_KEY = "6804295966:AAFZaj_tGlLah73SqDUyb8i_2ymaHttVKMw"
TELEGRAM_URL = f"https://api.telegram.org/bot{TELEGRAM_KEY}"


def checkResponse(response):
    if response.status_code != 200:
        print(f"Error: API Request failed '{response.url}")
        exit(1)


def getFeedData(time: datetime):
    oneMinuteAgo = time - timedelta(minutes=1)
    
    adafruit_response = requests.get(
        f"{ADAFRUITIO_URL}?start_time={oneMinuteAgo.strftime("%Y-%m-%dT%H:%MZ")}&end_time={time.strftime("%Y-%m-%dT%H:%MZ")}", headers=ADAFRUIT_HEADERS
    )
    print(adafruit_response.url)
    print(adafruit_response.json())
    return adafruit_response.json()


def getNotionDatabaseID():
    search_params = {"filter": {"value": "database", "property": "object"}}
    search_response = requests.post(
        f"https://api.notion.com/v1/search", json=search_params, headers=NOTION_HEADERS
    )
    checkResponse(search_response)
    search_results = search_response.json()["results"]
    id = search_results[0]["id"]
    return id


def createPage(id, data):
    new_page = {
        "object": "page",
        "created_by": {"object": "user", "id": "023db579-ce0a-4a52-802f-1df0ed250902"},
        "last_edited_by": {
            "object": "user",
            "id": "023db579-ce0a-4a52-802f-1df0ed250902",
        },
        "parent": {"type": "database_id", "database_id": id},
        "properties": {
            "Date": {
                "id": "%5DXRj",
                "type": "date",
                "date": {"start": data["Time"], "end": None, "time_zone": None},
            },
            "Sensor_Value": {
                "id": "title",
                "type": "title",
                "title": [
                    {
                        "type": "text",
                        "text": {"content": data["Value"]},
                        "plain_text": data["Value"],
                    }
                ],
            },
        },
        "url": "https://www.notion.so/1500-3def25c4813748cebdfb72652c014686",
        "public_url": "None",
    }
    new_page_result = requests.post(
        "https://api.notion.com/v1/pages", headers=NOTION_HEADERS, json=new_page
    )
    checkResponse(new_page_result)


def sendTelegramMessage(data):
    if int(data["Value"]) > 2000:
        telegram_response = requests.post(
            f"{TELEGRAM_URL}/sendMessage",
            data={
                "chat_id": "7051565171",
                "text": f'Data has exceeded limit ({data["Value"]})',
            },
        )
        checkResponse(telegram_response)


while True:
    database_id = getNotionDatabaseID()
    feed_data = getFeedData(datetime.now())

    if not feed_data:
        print("No new data found. Going to sleep...")
        time.sleep(60)
        continue
    
    for entry in feed_data:
        data = {"Time": entry["created_at"], "Value": entry["value"]}
        print("Uploading new data to Notion")
        createPage(database_id, data)
        sendTelegramMessage(data)
        firstTime = False
    time.sleep(60)
