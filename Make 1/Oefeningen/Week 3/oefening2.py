def add_item(inv, item, quantity):
    inv[item] = quantity

def update_item(inv, item, quantity):
    if item in inv:
        inv[item] = quantity
    else:
        print(f"\nItem '{item}' niet in voorraad\n")

def display_inventory(inv):
    print("Item    | Stock")
    print("---------------")
    for key in inv:
        print("{:8}| {}".format(key, inv[key]))
    print("")

invetory= {}

while True:
    print("1. Voeg een item toe \n2. Update een item \n3. Geef de inventaris weer \n4. Stoppen")
    choice = int(input("\nWat wil je doen? "))
    
    match choice:
        case 1:
            add_item(invetory, input("Geef de naam van het item "), input("Geef de hoeveelheid "))
        case 2:
            update_item(invetory, input("Geef de naam van het item "), input("Geef de hoeveelheid "))
        case 3:
            display_inventory(invetory)
        case 4:
            break
            