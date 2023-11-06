import random




def fill1():
    numbers = []
    while len(numbers) != 10:
        num = random.randint(0, 1000000)
        if num % 5 == 0:
            numbers.append(num)
    print(numbers)


def fill2():
    numbers = []
    for _ in range(10):
        num = random.randint(0, 1000000)
        numbers .append(num - num % 5)
    print(numbers)


fill1()
fill2()