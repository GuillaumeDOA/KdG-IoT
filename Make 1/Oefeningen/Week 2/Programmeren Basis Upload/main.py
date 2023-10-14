# Create list with random words
# Create tuple with vowels
# Take next word in list
#   Take next letter of word
#       If letter is in vowallist
#           increment vowalcount
#       Else
#           add letter to output string
#       If letter is not last letter
#           Go back to line 4
#       Else
#           print output to screen
#   If word is not last word
#       Go back to line 3
#   Else 
#       Quit            


willekeurige_woorden = ["Tijger", "Appel", "Zonneschijn", "Piano", "Regenboog", "Koffie", "Berg", "Avontuur", "Zomer", "Lach"]

klinkers = ['a','e','i','o','u','y']

for woord in willekeurige_woorden:
    aantalKlinkers = 0
    geenKlinkers = ''
    for i in range(0,len(woord)):
        if woord[i].lower() in klinkers:
            aantalKlinkers+=1
        else:
            geenKlinkers += woord[i]
    print(f'Origineel woord: {woord}, woord zonder klinkers: {geenKlinkers}, aantal klinkers: {aantalKlinkers}')




