#--------------------------------------------

# getal = float(input("GEef getal "))

# for i in range(1,11):
#     print(getal*i)

#--------------------------------------------

# som = 0
# for x in range(1, 6):
#     som += x*x
# print(f'Sum of squares: {som}')

#--------------------------------------------

# import random
# luckyNubmer = random.randint(1,100)
# while True:
#     guess = int(input("What is your next guess: "))
#     if guess == luckyNubmer:
#         print(f'You guessed the correct number: {luckyNubmer}')
#         break
#     elif guess > luckyNubmer:
#         print('Too high! Try agian')
#     elif guess < luckyNubmer:
#         print('Too low! Try again')
    
#--------------------------------------------

def som(a:float, b:float):
    return a+b
def verschil(a: float, b:float):
    return a-b
def product(a: float, b:float):
    return a*b
def deling(a:float, b:float):
    return a/b

#--------------------------------------------

def calculateTVA(price):
    return price*1.21
