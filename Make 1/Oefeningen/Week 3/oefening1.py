def get_grades():
    subjects = int(input("Hoeveel vakken heb je? "))
    list=[]
    for vak in range(0,subjects):
        list.append(int(input(f"Cijfer vak {vak+1}: ")))
    return list

def calculate_average(grades):
    return round(sum(grades)/len(grades))

def calculate_final_grade(average):
    if(average >= 90):
        return 'A'
    elif average >= 80:
        return 'B'
    elif average >= 70:
        return 'C'
    elif average >= 60:
        return 'D'
    else:
        return 'F'

while True:
    subjects = get_grades()
    average = calculate_average(subjects)
    final_grade = calculate_final_grade(average)

    print(f"Cijfers: {subjects}; Gemiddlede: {average}; Grade: {final_grade}")
    if input("Wil je voor een andere student herhalen? ").lower() != "ja":
        break