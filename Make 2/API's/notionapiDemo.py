import requests
import json

NOTION_KEY = "secret_5ppGUsxLGKfWMw5OfD2KcrZiZMD9rIqSOKXXmicFXhs"

headers = {'Authorization': f"Bearer {NOTION_KEY}",
           'Content-Type': 'application/json',
           'Notion-Version': '2022-06-28'}

search_params = {'filter': {"value": "page", "property": "object"}}
search_response = requests.post(f"https://api.notion.com/v1/search", json=search_params, headers=headers)

search_results = search_response.json()['results']
page_id = search_results[0]['id']
create_page_body = {
    "parent": { "page_id": page_id },
    "properties": {
        "title": {
      "title": [{ 
          "type": "text", 
          "text": { "content": "Hello World!" } }]
        }
    },
    "children": [
    {
      "object": "block",
      "type": "paragraph",
      "paragraph": {
        "rich_text": [{ 
            "type": "text", 
            "text": { 
                "content": "This page was made using an Api call!" 
            } 
        }]
      }
    }
  ]}

create_response = requests.post('https://api.notion.com/v1/pages',json=create_page_body, headers=headers)

created_id = create_response.json()['id']
blocks_response = requests.get(f'https://api.notion.com/v1/blocks/{created_id}/children', headers=headers)
print(blocks_response.json()['results'][0]['paragraph']['rich_text'][0]['text']['content'])