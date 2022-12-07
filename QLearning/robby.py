#To run this program, simply type the following into a terminal "python3 robby.py"

import numpy as np

N = 5000
M = 200
eta = 0.2
gamma = 0.9

#DONE - Step 1: make board and place robby
#DONE - Step 2: take robby's location and scan adjacent tiles return array of current state
#DONE - Step 3: find current state in Q-matrix and perform e-greedy return action to take
#DONE - Step 4: reward robby for the action it took, compare previous reward value to new value


#intialize the board reset this once per episode
def makeBoard():
    board = np.random.randint(2, size=(10, 10))
    robby = [np.random.randint(10), np.random.randint(10)]
    return board, robby

#intialize Q-Matrix never reset this
def qMatrix():
    qMat = np.zeros((243, 5))
    return qMat

#permutations of Q-Matrix
def qPerm():
    index = 0
    qp = np.zeros((243, 5))
    for a in range(-1, 2):
        for b in range(-1, 2):
            for c in range(-1, 2):
                for d in range(-1, 2):
                    for e in range(-1, 2):
                        qp[index][0] = a
                        qp[index][1] = b
                        qp[index][2] = c
                        qp[index][3] = d
                        qp[index][4] = e
                        index += 1
    return qp

#scan robby's surroundings and current position
def scan(board, robby):
    N, S, W, E, curr = 0, 0, 0, 0, 0

    if(board[robby[0]][robby[1]]):
        curr = 1

    #North
    if(robby[0] - 1 < 0):
        N = -1
    elif(board[robby[0] - 1][robby[1]]):
        N = 1
    
    #South
    if(robby[0] + 1 > 9):
        S = -1
    elif(board[robby[0] + 1][robby[1]]):
        S = 1
    
    #West
    if(robby[1] - 1 < 0):
        W = -1
    elif(board[robby[0]][robby[1] - 1]):
        W = 1
    
    #East
    if(robby[1] + 1 > 9):
        E = -1
    elif(board[robby[0]][robby[1] + 1]):
        E = 1
    return N, S, W, E, curr

#finds location of in the Q-matrix for a given state
def findMatch(board, robby, qp):
    N, S, W, E, curr = scan(board, robby)
    for i in range(243):
        if(N == qp[i][0] and S == qp[i][1] and W == qp[i][2] and E == qp[i][3] and curr == qp[i][4]):
            return i

def eGreed(qMat, board, robby, qp, epsilon):
    random = np.random.randint(1, 100)
    match = findMatch(board, robby, qp)
    index = []
    if(random >= epsilon):
        max = -1
        for i in range(5):
            if(qMat[match][i] == max):
                index.append(i)
            if(qMat[match][i] > max):
                index = []
                max = qMat[match][i]
                index.append(i)
    else:
        min = 10000
        for i in range(5):
            if(qMat[match][i] == min):
                index.append(i)
            if(qMat[match][i] < min):
                index = []
                min = qMat[match][i]
                index.append(i)
    temp = len(index)
    if(temp <= 1):
        return index[0]
    return index[np.random.randint(temp - 1)]

def episode(qMat, qp, train):
    board, robby = makeBoard()
    epsilon = 10
    totalReward = 0
    for i in range(M):
        if(i != 1 and i % 50 == 0 and epsilon > 0 and train):
            epsilon -= 0.25
        reward = 0
        move = eGreed(qMat, board, robby, qp, epsilon)
        currentPerm = findMatch(board, robby, qp)
        N, S, W, E, curr = scan(board, robby)
        #move North
        if(move == 0):
            if(N == -1):
                reward = -5
            else:
                robby[0] = robby[0] - 1
        #move South
        if(move == 1):
            if(S == -1):
                reward = -5
            else:
                robby[0] = robby[0] + 1
        #move West
        if(move == 2):
            if(W == -1):
                reward = -5
            else:
                robby[1] = robby[1] - 1
        #move East
        if(move == 3):
            if(E == -1):
                reward = -5
            else:
                robby[1] = robby[1] + 1
        #current
        if(move == 4):
            if(curr == 1):
                reward = 10
                board[robby[0]][robby[1]] = 0
            else:
                reward = -1      
        
        totalReward += reward

        if(train):
            #next move
            nextM = eGreed(qMat, board, robby, qp, epsilon)
            nextPerm = findMatch(board, robby, qp)
            qMat[currentPerm][move] += eta * (reward + gamma * (qMat[nextPerm][nextM]) - qMat[currentPerm][move])
            
    return qMat, totalReward

def main():
    qMat = qMatrix()
    qp = qPerm()
    totalReward = 0
    print("TRAINING")
    for i in range(N):
        qMat, totalReward = episode(qMat, qp, True)
        #Print report
        if(i % 100 == 0):
            print("Episode: " + str(i))
            print("Total Reward: " + str(totalReward))
    totalReward = 0
    print("TESTING")
    for j in range(N):
        qMat, totalReward = episode(qMat, qp, False)
        #Print report
        if(j % 100 == 0):
            print("Episode: " + str(j))
            print("Total Reward: " + str(totalReward))

main()