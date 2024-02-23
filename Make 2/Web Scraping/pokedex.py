import requests
from bs4 import BeautifulSoup

url = requests.get("https://pokemondb.net/pokedex/all")

if url.status_code < 200 or url.status_code > 300:
    print("Failed HTTP GET request. Response code: %d" % url.status_code)

soup = BeautifulSoup(url.text, "html.parser")
table = soup.find('tbody')

previousNumber = 0
for row in table.find_all('tr'):
    cells = row.find_all('td')
    if previousNumber == cells[0].text:
        continue
    # Number - Name - Hp - Types
    print(f'{cells[0].text.strip().lstrip("0")} - {cells[1].text} - {cells[4].text} - {cells[2].text}')
    previousNumber = cells[0].text