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

page = {
    "parent": {"database_id": {db_id}},
    "properties": {
        "Value": {"title": [
            {
                "text": {
                    "content": adafruit["Value"]
                    }
                }
            ]
        },
        "Time": {
            "rich_text": [
                {
                    "text": {
                        "content": adafruit["Time"]
                        }
                    }
                ]
        },
    },
}

page_created = requests.post("https://api.notion.com/v1/pages", headers=NOTION_HEADERS, json=page)

print(page_created.json())
