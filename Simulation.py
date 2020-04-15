import subprocess
import sys
import time

def winner(board,player):

    boardWidth = len(board)
    boardHeight = len(board[0])
    tile = player
    # check horizontal spaces
    for y in range(boardHeight):
        for x in range(boardWidth - 3):
            if board[x][y] == tile and board[x+1][y] == tile and board[x+2][y] == tile and board[x+3][y] == tile:
                return True

    # check vertical spaces
    for x in range(boardWidth):
        for y in range(boardHeight - 3):
            if board[x][y] == tile and board[x][y+1] == tile and board[x][y+2] == tile and board[x][y+3] == tile:
                return True

    # check / diagonal spaces
    for x in range(boardWidth - 3):
        for y in range(3, boardHeight):
            if board[x][y] == tile and board[x+1][y-1] == tile and board[x+2][y-2] == tile and board[x+3][y-3] == tile:
                return True

    # check \ diagonal spaces
    for x in range(boardWidth - 3):
        for y in range(boardHeight - 3):
            if board[x][y] == tile and board[x+1][y+1] == tile and board[x+2][y+2] == tile and board[x+3][y+3] == tile:
                return True

    return False

def iswinner(state):
    return winner(state,"r") or winner(state,"y")

def print_board(state):
    print(state)
    ls = state.split(",")
    for i in range(5,-1,-1):
        for j in range(0,7):
            print(ls[i][j],end = " ")
        print()

def process_move(inp):
    return int(str(inp)[2])

def print_time(finish):
    print("Ran in {} seconds.\n".format(finish))

def get_move(language,player,state,current_player):
    if(current_player == "red"):
        if(player == "h"):
            return int(input("Enter a column between 0 and 6: "))

        if(language == "c"):
            with subprocess.Popen(["./RedPlayer",state,current_player],stdout=subprocess.PIPE) as proc:
                return proc.stdout.read()
        elif(language == "python"):
            with subprocess.Popen(["python3", player, state, current_player],stdout=subprocess.PIPE) as proc:
                return proc.stdout.read()
    else:
        if(player == "h"):
            return int(input("Enter a column between 0 and 6: "))

        if(language == "c"):
            with subprocess.Popen(["./YellowPlayer",state,current_player],stdout=subprocess.PIPE) as proc:
                return proc.stdout.read()
        elif(language == "python"):
            with subprocess.Popen(["python3", player, state, current_player],stdout=subprocess.PIPE) as proc:
                return proc.stdout.read()

def update_board(state,move,player):
    ls = state.split(",")
    for i in range(0,6):
        ls[i] = list(ls[i])
        if(ls[i][move] == "."):
            ls[i][move] = player[0]
            ls[i] = "".join(ls[i])
            break
        ls[i] = "".join(ls[i])
    return ",".join(ls)

def main():

    default_state = ".......,.......,.......,.......,.......,......."

    if(len(sys.argv)!= 3):
        print("Incorrect number of arguments.")
    else:
        player_red = sys.argv[1] # get filenames
        player_yellow = sys.argv[2]
        language_red = "python"
        language_yellow = "python"

        if(player_red != "h"): # h is for human player
            if(player_red.endswith(".cpp")):
                subprocess.run(["g++","-o","RedPlayer",player_red])
                language_red = "c"
            elif(player_red.endswith(".c")):
                subprocess.run(["gcc","-lm","-w","-std=c99","-o","RedPlayer","ConnectFour.c","*.c"])
                language_red = "c"

        if(player_yellow != "h"):
            if(player_yellow.endswith(".cpp")):
                subprocess.run(["g++","-o","YellowPlayer",player_yellow])
                language_yellow ="c"
            elif(player_yellow.endswith(".c")):
                subprocess.run(["gcc","-lm","-w","-std=c99","-o","YellowPlayer",player_yellow,"*.c"])
                language_yellow = "c"

        state = default_state
        current_player = "red" # red goes first
        move_count = 0
        timeouts = True

        while(not iswinner(state.split(",")) and move_count < 42):
            start = time.time()
            print("Move {}:".format(move_count+1))

            if(current_player == "red"):

                move = get_move(language_red,player_red,state,current_player)
                finish = time.time()

                if(finish-start > 1 and timeouts == True and player_red != "h"):
                    print("----------------------------------TIME OUT!----------------------------------")
                else:
                    if(player_red != "h"):
                        move = process_move(move)
                    print("Red places in column " + str(move))
                    state = update_board(state,move,current_player)
                move_count += 1
                print_board(state)
                current_player = "yellow"

                if(player_red != "h"):
                    print_time(finish-start)
                else:
                    print()
            else:
                move = get_move(language_yellow,player_yellow,state,current_player)
                finish = time.time()

                if(time.time()-start>1 and timeouts == True and player_yellow != "h"):
                    print("----------------------------------TIME OUT!----------------------------------")
                else:
                    if(player_yellow != "h"):
                        move = process_move(move)
                    print("Yellow places in column " + str(move))
                    state = update_board(state,move,current_player)
                move_count += 1
                print_board(state)
                current_player = "red"

                if(player_yellow != "h"):
                    print_time(finish-start)
                else:
                    print()

        if(winner(state.split(","),"r")):
            print("Red Wins!")
        elif(winner(state.split(","),"y")):
            print("Yellow Wins!")
        else:
            print("Draw!")

main()
