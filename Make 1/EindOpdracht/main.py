import random

print(r"""  _______       __    _____  ___            __            _______     ______    __   __  ___  # Print ASCII art
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
    for key in GameBoard:                   # For every key in GameBoard dictionary do following:
        print(GameBoard[key], end="")       # Print value of key without newline
        count += 1                          # Keep track of how many values we print
        if count == 3:                      # For every 3rd value printed print newline with spacer else print "|" and increase rowCount
            if rowCount == 3:               # If on 3rd rowCount dont print row spacer (-+-+-)
                print("\n")
                return
            print("")                       
            print("-+-+-")
            count = 0
            rowCount += 1
        else:
            print("|", end="")


def CheckBoard():
    winning_combinations = [[1, 2, 3],  # List with all winning combinations
                            [4, 5, 6],
                            [7, 8, 9],
                            [1, 4, 7],
                            [2, 5, 8],
                            [3, 6, 9],
                            [1, 5, 9],
                            [3, 5, 7]]
    for combination in winning_combinations:                # For every combination do following:
        first_value = GameBoard.get(combination[0])         # Get value to compare rest of combination to
        are_values_same = all(GameBoard[key] == first_value and GameBoard[key] != ' ' for key in combination)   # Use lambda to check if all values in GameBoard are the same as "first_Value"
                                                                                                                # And check if not ' '
        if are_values_same:
            return first_value                                  # If True return winning symbol
    if all(GameBoard[key] in ('X','O') for key in GameBoard):   # Check if GameBoard is full without anyone winning and return 'Draw'
        return 'Draw'
    return 'Ongoing'                                            # Else return 'Ongoing'


def MakeMove(player):
    while True:                                                 # Repeat untill valid move has been chosen
        move = int(
            input(f"Waar wil je een '{Symbols[player]}' zetten {Players[player]} "))    
        if isinstance(move, int) and move >= 1 and move <= 9 and GameBoard[move] == " ": # Check if "move" is empty (' ') and on the board (1-9)
            break
        print("Ongeldige positie!")
    GameBoard[move] = Symbols[player]                           # If valid play move


def ResetBoard():   
    for key in GameBoard:                                       # Reset GameBoard by filling all spaces with a space ' '
        GameBoard[key] = ' '
    Players[0] = input("Geef de naam van Player 1 in. ('X') ")  # Ask for player names
    Players[1] = input("Geef de naam van Player 2 in. ('O') ")


GameBoard = {7: " ", 8: " ", 9: " ",                            # Initialise GameBoard and Players with Symbols
             4: " ", 5: " ", 6: " ",
             1: " ", 2: " ", 3: " "}
Symbols = ["X", "O"]
Players = ["", ""]


currentPlayer = random.randint(0, 1)                            # Make startingplayer random

while True:     
    ResetBoard()                                                # Initialise Game
    while CheckBoard() == 'Ongoing':                            # Repeat gamelogic untill game is won or drawed
        PrintBoard()
        MakeMove(currentPlayer)

        if currentPlayer == 1:                                  # Switch currentplayer
            currentPlayer = 0
        else:
            currentPlayer = 1
    
    PrintBoard()                                                # When game is won or drawed print board one last time
    result = CheckBoard()
    if result == 'X':                                           # Send message depending on who won
        print(f"Hoera!! {Players[0]} heeft gewonnen!")
    elif result == 'O':
        print(f"Hoera!! {Players[1]} heeft gewonnen!")
    else:
        print("Gelijkspel!")
    
    if input("Wil je nog eens spelen? ").lower() != 'j':        # Ask if you want to replay
        break

print(r"""
 ___________  __    __       __      _____  ___   __   ___   ________       _______   ______     _______         _______   ___            __       ___  ___  __    _____  ___    _______      
("     _   ")/" |  | "\     /""\    (\"   \|"  \ |/"| /  ") /"       )     /"     "| /    " \   /"      \       |   __ "\ |"  |          /""\     |"  \/"  ||" \  (\"   \|"  \  /" _   "|     
 )__/  \\__/(:  (__)  :)   /    \   |.\\   \    |(: |/   / (:   \___/     (: ______)// ____  \ |:        |      (. |__) :)||  |         /    \     \   \  / ||  | |.\\   \    |(: ( \___)     
    \\_ /    \/      \/   /' /\  \  |: \.   \\  ||    __/   \___  \        \/    | /  /    ) :)|_____/   )      |:  ____/ |:  |        /' /\  \     \\  \/  |:  | |: \.   \\  | \/ \          
    |.  |    //  __  \\  //  __'  \ |.  \    \. |(// _  \    __/  \\       // ___)(: (____/ //  //      /       (|  /      \  |___    //  __'  \    /   /   |.  | |.  \    \. | //  \ ___     
    \:  |   (:  (  )  :)/   /  \\  \|    \    \ ||: | \  \  /" \   :)     (:  (    \        /  |:  __   \      /|__/ \    ( \_|:  \  /   /  \\  \  /   /    /\  |\|    \    \ |(:   _(  _|    
     \__|    \__|  |__/(___/    \___)\___|\____\)(__|  \__)(_______/       \__/     \"_____/   |__|  \___)    (_______)    \_______)(___/    \___)|___/    (__\_|_)\___|\____\) \_______)                                                                                                                                                                         
      """)