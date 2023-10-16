import random

print(r"""  _______       __    _____  ___            __            _______     ______    __   __  ___ 
 /" __   )     |" \  (\"   \|"  \          /""\          /"      \   /    " \  |"  |/  \|  "|
(__/ _) ./     ||  | |.\\   \    |        /    \        |:        | // ____  \ |'  /    \:  |
    /  //      |:  | |: \.   \\  |       /' /\  \       |_____/   )/  /    ) :)|: /'        |
 __ \_ \\      |.  | |.  \    \. |      //  __'  \       //      /(: (____/ //  \//  /\'    |
(: \__) :\     /\  |\|    \    \ |     /   /  \\  \     |:  __   \ \        /   /   /  \\   |
 \_______)    (__\_|_)\___|\____\)    (___/    \___)    |__|  \___) \"_____/   |___/    \___|
      """)


def PrintBoard():
    count = 0
    rowCount = 1
    print("")
    for key in GameBoard:
        print(GameBoard[key], end="")
        count+=1
        if count == 3:
            if rowCount == 3:
                print("\n")
                return
            print("")
            print("-+-+-")
            count = 0
            rowCount += 1
        else:
            print("|", end="")

def CheckBoard():
    winning_combinations = [[1,2,3],
                            [4,5,6],
                            [7,8,9],
                            [1,4,7],
                            [2,5,8],
                            [3,6,9],
                            [1,5,9],
                            [3,5,7]]

            
    return False

def MakeMove(player):
    while True:
        move = int(input(f"Waar wil je een '{Symbols[player]}' zetten {Players[player]} "))
        if isinstance(move, int) and move >=1 and move <= 9 and GameBoard[move] == " " :
            break
        print("Ongeldige positie!")
    GameBoard[move] = Symbols[player]

def ResetBoard():
    Board = {7: " ", 8: " ", 9: " ", 
             4: " ", 5: " ", 6: " ", 
             1: " ", 2: " ", 3: " "}
    return Board
    
    
GameBoard = {7: " ", 8: " ", 9: " ", 
             4: " ", 5: " ", 6: " ", 
             1: " ", 2: " ", 3: " "}

Symbols = ["X", "O"]
Players = ["Player 1", "Player 2"]
Players[0] = input("Geef de naam van Player 1 in. \nPlayer 1 speelt met 'X' ")
Players[1] = input("Geef de naam van Player 2 in. \n(Player 2 speelt met 'O') ")


currentPlayer = random.randint(0,1)

while True:
    PrintBoard()
    MakeMove(currentPlayer)

    if CheckBoard():
        if input("Wil je opnieuw spelen? ").lower() == 'j':
            GameBoard = ResetBoard()
        else:
            break

    if currentPlayer == 1:
        currentPlayer = 0
    else:
        currentPlayer = 1

print("Finished")
