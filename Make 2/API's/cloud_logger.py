import requests

ADAFRUIT_KEY = "aio_ZEBj40c2V2xTekVRnuoaEHVlAuNa"

ADAFRUIT_HEADERS = {"X-AIO-Key": ADAFRUIT_KEY}
ADAFRUITIO_URL = (
    "https://io.adafruit.com/api/v2/Exotic/feeds/adafruit-io-demo.sensor/data?limit=1"
)

NOTION_KEY = "secret_5ppGUsxLGKfWMw5OfD2KcrZiZMD9rIqSOKXXmicFXhs"

NOTION_HEADERS = {
    "Authorization": f"Bearer {NOTION_KEY}",
    "Content-Type": "application/json",
    "Notion-Version": "2022-06-28",
}

TELEGRAM_KEY = "6804295966:AAFZaj_tGlLah73SqDUyb8i_2ymaHttVKMw"
TELEGRAM_URL = f"https://api.telegram.org/bot{TELEGRAM_KEY}"

response = requests.get(ADAFRUITIO_URL, headers=ADAFRUIT_HEADERS)
adafruit = {
    "Time": response.json()[0]["created_at"],
    "Value": response.json()[0]["value"],
}
print(f"Value: {adafruit['Value']} ({adafruit['Time']})")

search_params = {"filter": {"value": "database", "property": "object"}}
search_response = requests.post(
    f"https://api.notion.com/v1/search", json=search_params, headers=NOTION_HEADERS
)

search_results = search_response.json()["results"]
db_id = search_results[0]["id"]
db = requests.get(
    f"https://api.notion.com/v1/databases/{db_id}", headers=NOTION_HEADERS
)
db_keys = db.json()["properties"].keys()

new_page = {
    "object": "page",
    "created_by": {
        "object": "user",
        "id": "023db579-ce0a-4a52-802f-1df0ed250902"
    },
    "last_edited_by": {
        "object": "user",
        "id": "023db579-ce0a-4a52-802f-1df0ed250902"
    },
    "parent": {
        "type": "database_id",
        "database_id": "7d243326-9d5d-4a0c-bf1b-8566eeac0922"
    },
    "properties": {
        "Date": {
            "id": "%5DXRj",
            "type": "date",
            "date": {
                "start": adafruit['Time'],
                "end": None,
                "time_zone": None
            }
        },
        "Sensor_Value": {
            "id": "title",
            "type": "title",
            "title": [
                {
                    "type": "text",
                    "text": {
                        "content": adafruit['Value']
                    },
                    "plain_text": adafruit['Value']
                }
            ]
        }
    },
    "url": "https://www.notion.so/1500-3def25c4813748cebdfb72652c014686",
    "public_url": "None"
}

new_page_result = requests.post(
    "https://api.notion.com/v1/pages", headers=NOTION_HEADERS, json=new_page)

print(new_page_result.json())

if int(adafruit["Value"]) > 2500:
    telegram = requests.post(f'{TELEGRAM_URL}/sendMessage',
                         data={'chat_id': '7051565171', 'text': f'Data has exceeded limit ({adafruit["Value"]})'})
    print(telegram.json())

