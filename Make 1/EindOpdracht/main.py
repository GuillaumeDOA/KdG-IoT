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
        count += 1
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
    winning_combinations = [[1, 2, 3],
                            [4, 5, 6],
                            [7, 8, 9],
                            [1, 4, 7],
                            [2, 5, 8],
                            [3, 6, 9],
                            [1, 5, 9],
                            [3, 5, 7]]
    for combination in winning_combinations:
        first_value = GameBoard.get(combination[0])
        # are_values_same = all(GameBoard[key] == first_value for key in combination)
        are_values_same = all(GameBoard[key] == first_value and GameBoard[key] != ' ' for key in combination)

        if are_values_same:
            return first_value
    if all(GameBoard[key] in ('X','O') for key in GameBoard):
        return 'Draw'
    return 'Ongoing'


def MakeMove(player):
    while True:
        move = int(
            input(f"Waar wil je een '{Symbols[player]}' zetten {Players[player]} "))
        if isinstance(move, int) and move >= 1 and move <= 9 and GameBoard[move] == " ":
            break
        print("Ongeldige positie!")
    GameBoard[move] = Symbols[player]


def ResetBoard():
    for key in GameBoard:
        GameBoard[key] = ' '
    Players[0] = input("Geef de naam van Player 1 in. ('X') ")
    Players[1] = input("Geef de naam van Player 2 in. ('O') ")


GameBoard = {7: " ", 8: " ", 9: " ",
             4: " ", 5: " ", 6: " ",
             1: " ", 2: " ", 3: " "}
Symbols = ["X", "O"]
Players = ["", ""]


currentPlayer = random.randint(0, 1)

while True:
    ResetBoard()
    while CheckBoard() == 'Ongoing':
        PrintBoard()
        MakeMove(currentPlayer)

        if currentPlayer == 1:
            currentPlayer = 0
        else:
            currentPlayer = 1
    
    PrintBoard()
    result = CheckBoard()
    if result == 'X':
        print(f"Hoera!! {Players[0]} heeft gewonnen!")
    elif result == 'O':
        print(f"Hoera!! {Players[1]} heeft gewonnen!")
    else:
        print("Gelijkspel!")
    
    if input("Wil je nog eens spelen? ").lower() != 'j':
        break

print("Finished")
