
num1 = float(input("Geef het eerste getal in "))
num2 = float(input("Geef het tweede getal in "))
keuze = int(input("Kies een bewerking "))

bewerkingen = {
    1: num1 + num2,
    2: num1 - num2,
    3: num1 * num2,
    4: num1 / num2,
    5: num1 % num2
}

print(bewerkingen[keuze])
