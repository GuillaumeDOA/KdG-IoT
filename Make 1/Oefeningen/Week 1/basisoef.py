# OEFENING 1
# num1 = int(input("Getal 1 "))
# num2 = int(input("Getal 2 "))
# print(f'Sum is {num1+num2}, minus is {num1-num2}, product is {num1*num2}, quotient is {num1/num2}  ')

# OEFENING 2
# celsius = int(input("Geef temp in celsius "))
# print(f'Temp in Fahrenheit: {(celsius*9/5)+32}')

# OEFENING 3
# l = int(input("Geef de lengte "))
# b = int(input("Geef de breedte "))
# print(f'Oppervlakte is {l*b}')

# OEFENING 4
# lengte = float(input("Geef je lengte in meter "))
# gewicht = int(input('Geef je gewicht in kg '))
# print(f'Je BMI: {round(gewicht/(lengte**2),2)}')

# OEFENING 5
# a = int(input("Geef waarde a "))
# b = int(input("Geef waarde b "))
# print(f'Waarde a: {a} \nWaarde b: {b}')
# c = a
# a = b
# b = c
# print(f'Waarde a: {a} \nWaarde b: {b}')



# OEFENING 1
#print(('maandag', 'dinsdag', 'woensdag', 'donderdag', 'vrijdag', 'zaterdag', 'zondag'))

# OEFENING 1
# import random
# mijn_lijst = []
# for i in range(4):
#     mijn_lijst.append(random.randint(0,150))
# print(f'mijn_lijst: {mijn_lijst}')

# OEFENING 2
# l1 = []
# l2 = []
# for x in range(1,11):
#     if x < 6:
#         l1.append(x)
#     else :
#         l2.append(x)
# print(f'Lijst 1: {l1} \nLijst 2: {l2}')
# l3 = l1+l2
# print(f'Lijst 3: {l3}')

# OEFENING 3
# myList = ["Testing", "Sheesh", "Tuples"]
# print(f"Eerste: {myList[0]} \nLaatste: {myList[-1]}")

# OEFENING 8
# l = [45, 12, 67, 3, 89, 22]
# l.sort()
# print(l)

# DICTIONARIES
# OEFENING 1
persoon = {'naam': 'John', 'leeftijd': 25, 'stad': 'Amsterdam'}
print(persoon)

# OEFENING 2
import random
student = {'naam': 'Alice', 'leeftijd': 20, 'cijfers': [85, 90, 78, 92]}
student['cijfers'].append(random.randint(0,101))
print(student)

# OEFENING 3
laguages = {"Python": 1, "Java": 2, "C++": 3}
for x in laguages:
    print(f'Taal: {x} \nScore: {laguages[x]}')


