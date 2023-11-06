import random

# FEEDBACK: Functies versimpeld waaronder: PrintBoard() en MakeMove() 
# Bronnen vermeld
# Try Except clausule toegevoegd waar nodig

# Print ASCII art. ASCII art from https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
print(r"""  _______       __    _____  ___            __            _______     ______    __   __  ___ 
 /" __   )     |" \  (\"   \|"  \          /""\          /"      \   /    " \  |"  |/  \|  "|           
(__/ _) ./     ||  | |.\\   \    |        /    \        |:        | // ____  \ |'  /    \:  |
    /  //      |:  | |: \.   \\  |       /' /\  \       |_____/   )/  /    ) :)|: /'        |
 __ \_ \\      |.  | |.  \    \. |      //  __'  \       //      /(: (____/ //  \//  /\'    |
(: \__) :\     /\  |\|    \    \ |     /   /  \\  \     |:  __   \ \        /   /   /  \\   |
 \_______)    (__\_|_)\___|\____\)    (___/    \___)    |__|  \___) \"_____/   |___/    \___|
      """)

# Function prints the gameboard
# Simplified from last version after feedback
def PrintBoard():
    print(f"{GameBoard[7]}|{GameBoard[8]}|{GameBoard[9]}")
    print("-+-+-")
    print(f"{GameBoard[4]}|{GameBoard[5]}|{GameBoard[6]}")
    print("-+-+-")
    print(f"{GameBoard[1]}|{GameBoard[2]}|{GameBoard[3]}")

def CheckBoard():
    winning_combinations = [[1, 2, 3],  # List with all winning combinations
                            [4, 5, 6],
                            [7, 8, 9],
                            [1, 4, 7],
                            [2, 5, 8],
                            [3, 6, 9],
                            [1, 5, 9],
                            [3, 5, 7]]
    # For every combination do following:
    for combination in winning_combinations:
        # Get value to compare rest of combination to
        first_value = GameBoard.get(combination[0])
        # Use lambda to check if all values in GameBoard are the same as "first_Value" and check if not ' '
        are_values_same = all(GameBoard[key] == first_value and GameBoard[key] != ' ' for key in combination)
        if are_values_same:
            # If True return winning symbol
            return first_value
    # Check if GameBoard is full without anyone winning and return 'Draw'
    if all(GameBoard[key] in ('X', 'O') for key in GameBoard):
        return 'Draw'
    # Else return 'Ongoing'
    return 'Ongoing'

def MakeMove(player):
    # Repeat untill valid move has been chosen
    while True:
        try:
            move = input(f"Waar wil je een '{Symbols[player]}' zetten {Players[player]} ")
            move = int(move)        # Cast move to int and catch exception
            # Check if "move" is empty (' ') and on the board (1-9)
            if move in range(1,10) and GameBoard[move] == " ":
                break
            print(f"{move} ligt niet tussen 1 en 9!")
        except ValueError:
            print(f"{move} is geen nummer!")
    # If valid play move
    GameBoard[move] = Symbols[player]

def ResetBoard():
    # Reset GameBoard by filling all spaces with a space ' '
    for key in GameBoard:
        GameBoard[key] = ' '
    # Ask for player names
    Players[0] = input("Geef de naam van Player 1 in. ('X') ")
    Players[1] = input("Geef de naam van Player 2 in. ('O') ")

GameBoard = {7: " ", 8: " ", 9: " ",                            # Initialise GameBoard and Players with Symbols
             4: " ", 5: " ", 6: " ",
             1: " ", 2: " ", 3: " "}
Symbols = ["X", "O"]
Players = ["", ""]

# Make startingplayer random
currentPlayer = random.randint(0, 1)

while True:
    ResetBoard()                                                # Initialise Game
    while CheckBoard() == 'Ongoing':                            # Repeat gamelogic untill game is won or drawed
        PrintBoard()
        MakeMove(currentPlayer)

        if currentPlayer == 1:                                  # Switch currentplayer
            currentPlayer = 0
        else:
            currentPlayer = 1

    # When game is won or drawed print board one last time
    PrintBoard()
    result = CheckBoard()
    if result == 'X':                                           # Send message depending on who won
        print(f"Hoera!! {Players[0]} heeft gewonnen!")
    elif result == 'O':
        print(f"Hoera!! {Players[1]} heeft gewonnen!")
    else:
        print("Gelijkspel!")

    if input("Wil je nog eens spelen? (J/j) ").lower() != 'j':        # Ask if you want to replay
        break


# Print ASCII art. ASCII art based from https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
print(r"""
 ___________  __    __       __      _____  ___   __   ___   ________       _______   ______     _______         _______   ___            __       ___  ___  __    _____  ___    _______      
("     _   ")/" |  | "\     /""\    (\"   \|"  \ |/"| /  ") /"       )     /"     "| /    " \   /"      \       |   __ "\ |"  |          /""\     |"  \/"  ||" \  (\"   \|"  \  /" _   "|     
 )__/  \\__/(:  (__)  :)   /    \   |.\\   \    |(: |/   / (:   \___/     (: ______)// ____  \ |:        |      (. |__) :)||  |         /    \     \   \  / ||  | |.\\   \    |(: ( \___)     
    \\_ /    \/      \/   /' /\  \  |: \.   \\  ||    __/   \___  \        \/    | /  /    ) :)|_____/   )      |:  ____/ |:  |        /' /\  \     \\  \/  |:  | |: \.   \\  | \/ \          
    |.  |    //  __  \\  //  __'  \ |.  \    \. |(// _  \    __/  \\       // ___)(: (____/ //  //      /       (|  /      \  |___    //  __'  \    /   /   |.  | |.  \    \. | //  \ ___     
    \:  |   (:  (  )  :)/   /  \\  \|    \    \ ||: | \  \  /" \   :)     (:  (    \        /  |:  __   \      /|__/ \    ( \_|:  \  /   /  \\  \  /   /    /\  |\|    \    \ |(:   _(  _|    
     \__|    \__|  |__/(___/    \___)\___|\____\)(__|  \__)(_______/       \__/     \"_____/   |__|  \___)    (_______)    \_______)(___/    \___)|___/    (__\_|_)\___|\____\) \_______)                                                                                                                                                                         
      """)
