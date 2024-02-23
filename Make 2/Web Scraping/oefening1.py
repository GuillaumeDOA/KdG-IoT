import requests
from bs4 import BeautifulSoup


# Oefening 1
r = requests.get(
    'https://books.toscrape.com/catalogue/a-light-in-the-attic_1000/index.html')
soup = BeautifulSoup(r.text, "html.parser")

price = soup.find_all("p", class_="price_color")[0].text
print(f"Boek: '{soup.body.h1.text}' kost {price[2:]} euro")

#--------------------------------------------------------------------------------------

# Oefening 2

urls = ["https://books.toscrape.com/catalogue/a-light-in-the-attic_1000/index.html",
         "https://books.toscrape.com/catalogue/tipping-the-velvet_999/index.html",
         "https://books.toscrape.com/catalogue/soumission_998/index.html",
         "https://books.toscrape.com/catalogue/sharp-objects_997/index.html",
         "https://books.toscrape.com/catalogue/sapiens-a-brief-history-of-humankind_996/index.html"]

books = []
ratings = {'One': 1, 'Two': 2, 'Three': 3, 'Four': 4, 'Five': 5}

for url in urls:
    r = requests.get(url)
    soup = BeautifulSoup(r.text, 'html.parser')
    title = soup.body.h1.text
    rating = soup.find("p", class_ = "star-rating").attrs['class'][1]
    price = soup.find("p", class_ = "price_color").text[2:]
    result = (title, ratings[rating], price)
    books.append(result)
    
def printlist(l):
    for item in l:
        print(item)

printlist(books)
print("Sorted")
printlist(sorted(books, key=lambda x: x[1], reverse=True))
