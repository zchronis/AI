import numpy as np
import math
import random

width = 7
height = 6

points = [[3,4,5,7,5,4,3],
          [4,6,8,10,8,6,4],
          [5,8,11,13,11,8,5],
          [5,8,11,13,11,8,5],
          [4,6,8,10,8,6,4],
          [3,4,5,7,5,4,3]]

def newGame():
    board = np.zeros((height, width), dtype=np.int)
    return board

def setupPlayers():
    print("Please enter integers for who is playing")
    print("User controlled  1")
    print("Mini-max AI      2*    only works for player two")
    print("Random           3")
    print("Rational Agent   4     with random moves when no win or blocking move is available")
    print("Rational Agent   5*    with minimax, only works for player two")
    playerOne = int(input("Player One: "))
    playerTwo = int(input("Player Two: "))
    attempts = 1
    if(playerOne > 1 and playerTwo > 1):
        attempts = int(input("How many epochs? "))
    return playerOne, playerTwo, attempts

def printBoard(board):
    for i in range(height):
        for j in range(width):
            print(board[i][j], end=" ")
        print()

def switchPlayer(currentPlayer):
    if(currentPlayer == 1):
        currentPlayer = 2
    else:
        currentPlayer = 1
    return currentPlayer

def place(board, col, piece):
    tempBoard = board.copy()
    for i in range(len(board)-1, -1, -1):
        if(tempBoard[i][col] == 0):
            tempBoard[i][col] = piece
            return tempBoard
    return tempBoard

#check for win
def winCheck(board, player):
    #check horizontal positions for a win
    for i in range(width-3):
        for j in range(height):
            if(board[j][i] == player and board[j][i+1] == player and board[j][i+2] == player and board[j][i+3] == player):
                return True
    #check vertical positions for a win
    for i in range(width):
        for j in range(height-3):
            if(board[j][i] == player and board[j+1][i] == player and board[j+2][i] == player and board[j+3][i] == player):
                return True
    #check right diagonal positions for a win
    for i in range(width-3):
        for j in range(height-3):
            if(board[j][i] == player and board[j+1][i+1] == player and board[j+2][i+2] == player and board[j+3][i+3] == player):
                return True
    #check left diagonal positions for a win
    for i in range(width-3):
        for j in range(3, height):
            if(board[j][i] == player and board[j-1][i+1] == player and board[j-2][i+2] == player and board[j-3][i+3] == player):
                return True

    return False

#check if location is valid
def valid(board, col):
    for i in range(6):
            if board[i][col] == 0:
                return True
    return False

#Get all valid moves
def getValidMoves(board):
    validMoves = []
    for i in range(width):
        if valid(board, i):
            validMoves.append(i)
    return validMoves

def evalBoard(board, player):
    totalPoints = 0
    for x in range(len(board)):
        for y in range(len(board[x])):
            if(board[x][y] == player):
                totalPoints += points[x][y]
    return totalPoints

# returns a column for the AI to play based off the decision minimax made
def miniMax(board, depth):
    possibleMoves = {}

    for i in range(width):
        if(valid(board, i)):
            newBoard = place(board, i, 2)
            possibleMoves[i] = explore(newBoard, depth-1, 1)
    
    alpha = -999999999
    bestMove = None

    allMoves = possibleMoves.items()
    random.shuffle(list(allMoves))

    for move, tempAlpha in allMoves:
        if(tempAlpha >= alpha):
            alpha = tempAlpha
            bestMove = move
    
    return bestMove, alpha

def explore(board, depth, currPlayer):
    possibleBoards = []

    for i in range(width):
        if(valid(board, i)):
            newBoard = place(board, i, currPlayer)
            possibleBoards.append(newBoard)

    if(depth == 0 or len(possibleBoards) == 0 or winCheck(board, currPlayer)):
        return evalBoard(board, currPlayer)

    alpha = -99999999
    for child in possibleBoards:
        alpha = max(alpha, -explore(child, depth-1, switchPlayer(currPlayer)))
    return alpha


#chooses the most rational move for the AI
def rational(board, player):
    for i in range(width):
        if(valid(board, i)):
            newBoard = board.copy()
            newBoard = place(board, i, player)
            newBoard2 = board.copy()
            newBoard2 = place(board, i, switchPlayer(player))
            if(winCheck(newBoard, player)):
                return i
            elif(winCheck(newBoard2, switchPlayer(player))):
                return i
    while(1):
            move = np.random.randint(0, high=width)
            if(board[0][move] == 0 ):
                return move

def rational2(board, player):
    for i in range(width):
        if(valid(board, i)):
            newBoard = board.copy()
            newBoard = place(board, i, player)
            newBoard2 = board.copy()
            newBoard2 = place(board, i, switchPlayer(player))
            if(winCheck(newBoard, player)):
                return i
            elif(winCheck(newBoard2, switchPlayer(player))):
                return i
    move, temp = miniMax(board, 5)
    return move

def main():
    playerOne, playerTwo, epocs = setupPlayers()
    playerOneScore = 0
    playerTwoScore = 0
    draws = 0
    for i in range(epocs):
        currentPlayer = 1
        board = newGame()
        winner = False
        while(winner == False):
            #check for draw
            if(len(getValidMoves(board)) == 0):
                print("Draw")
                draws += 1
                break
            #Human Player
            if((playerOne == 1 and currentPlayer == 1) or (playerTwo == 1 and currentPlayer == 2)):
                printBoard(board)
                move = int(input("Enter 1-7 to choose a column to drop your piece into: "))
                move -= 1
                board = place(board, move, currentPlayer)
                winner = winCheck(board, currentPlayer)
                if(winner):
                    print("The winner is player", currentPlayer)
                    printBoard(board)
                    if(currentPlayer == 1):
                        playerOneScore += 1
                    else:
                        playerTwoScore += 1
                    break

            #Mini-Max AI
            elif((playerOne == 2 and currentPlayer == 1) or (playerTwo == 2 and currentPlayer == 2)):
                move = miniMax(board, 5)
                board = place(board, move[0], currentPlayer)
                winner = winCheck(board, currentPlayer)
                if(winner):
                    print("The winner is player", currentPlayer)
                    #printBoard(board)
                    if(currentPlayer == 1):
                        playerOneScore += 1
                    else:
                        playerTwoScore += 1
                    break

            #Random
            if((playerOne == 3 and currentPlayer == 1) or (playerTwo == 3 and currentPlayer == 2)):
                legal = False
                while(legal == False):
                    move = np.random.randint(0, high=width)
                    if(board[0][move] == 0 ):
                        legal = True
                board = place(board, move, currentPlayer)
                winner = winCheck(board, currentPlayer)
                if(winner):
                    print("The winner is player", currentPlayer)
                    #printBoard(board)
                    if(currentPlayer == 1):
                        playerOneScore += 1
                    else:
                        playerTwoScore += 1
                    break
            
            #Rational AI
            if((playerOne == 4 and currentPlayer == 1) or (playerTwo == 4 and currentPlayer == 2)):
                move = rational(board, currentPlayer)
                board = place(board, move, currentPlayer)
                winner = winCheck(board, currentPlayer)
                if(winner):
                    print("The winner is player", currentPlayer)
                    #printBoard(board)
                    if(currentPlayer == 1):
                        playerOneScore += 1
                    else:
                        playerTwoScore += 1
                    break
            
            #Rational AI with minimax
            if((playerOne == 5 and currentPlayer == 1) or (playerTwo == 5 and currentPlayer == 2)):
                move = rational2(board, currentPlayer)
                board = place(board, move, currentPlayer)
                winner = winCheck(board, currentPlayer)
                if(winner):
                    print("The winner is player", currentPlayer)
                    #printBoard(board)
                    if(currentPlayer == 1):
                        playerOneScore += 1
                    else:
                        playerTwoScore += 1
                    break
            #printBoard(board)
            #print("")
            currentPlayer = switchPlayer(currentPlayer)
            
        #printBoard(board)
    print("Player 1:", playerOneScore, "Player 2:", playerTwoScore ,"Draws:", draws)
    
main()