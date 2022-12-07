#include <math.h>
#include <string>
#include <iostream>
using namespace std;

const int END = 30000;
const int SIZE = 30000;

struct Board
{
    int childCounter;
    int tiles[3][3];
    int blankX;
    int blankY;
    int fitness = 999999;
    Board* childs;
    string parents = "";
    int parentBoardCount = 0;
};

int solution[3][3] = {{1,2,3},{4,5,6},{7,8,0}};

Board OPENLIST[SIZE];
int OPENLISTEND;

Board CLOSEDLIST[SIZE]; 
int CLOSEDLISTEND;

Board setBoard(char*, int);
void printBoard(Board);
int getFitness(Board, int);
int oopFitness(Board);
Board moveTile(Board, int, int, int);
void makeChilds(Board, int);
bool compareBoard(Board, Board);
void bestFirstWrapper(Board, int);
void bestFirst(int, int);
bool scanOpen(Board);
bool scanClose(Board);
void shiftList(int);
void printPath(Board);
string tilesToString(Board);
bool canBeSolved(Board);
void aStar2(Board, int);
int manhattan(Board board);
int linearCon(Board);


int main(int argc, char* argv[])
{
    bool go = true;
    Board root;
    if(argc > 3)
        root = setBoard(argv[1], stoi(argv[2]));
    else return 0;
    cout<<"parent board:"<<endl;
    printBoard(root);

    if(stoi(argv[2]) == 0)
    {
        cout<<"Heuristic: Out of Place"<<endl;
    }
    if(stoi(argv[2]) == 1)
    {
        cout<<"Heuristic: Manhattan Distance"<<endl;
    }
    if(stoi(argv[2]) == 2)
    {
        cout<<"Heuristic: Linear Conflict"<<endl;
    }

    if(stoi(argv[3]) == 0)
    {
        cout<<"Algorithm: Best First"<<endl;
    }
    if(stoi(argv[3]) == 1)
    {
        cout<<"Algorithm: A*"<<endl;
    }

    go = canBeSolved(root);

    if(go && stoi(argv[3]) == 0)
    {
        bestFirstWrapper(root, stoi(argv[2]));
    }
    else if(go && stoi(argv[3]) == 1)
    {
        aStar2(root, stoi(argv[2]));
    }
    else
    {
        cout<<"\nThis board is not solvable\n";
    }

    return 1;
}

bool canBeSolved(Board board)
{
    int oneD[9];
    int index = 0;
    for(int x = 0; x < 3; x++) 
    {
        for(int y = 0; y < 3; y++) 
        {
            oneD[index] = board.tiles[x][y];
            ++index;
        }
    }
    int count = 0;

    for(int x = 0; x < 9; x++) 
    {
        for(int y = 0; y < 9; y++) 
        {
            if(x < y) 
            {
                if(oneD[x] > oneD[y] && oneD[x] != 0 && oneD[y] != 0)
                {
                    count++;
                }
            }
        }
    }

    if(count % 2 == 0) {
        return true;
    }
    else
        return false;
}

Board setBoard(char* input, int heuristic)
{
    Board newBoard;
    int x = 0;
    int y = 0;
    for(int i = 0; i < 9; ++i)
    {
        //put user input into the board
        newBoard.tiles[x][y] = (int)(input[i]) - 48;
        //finds the initial blank
        if(newBoard.tiles[x][y] == 0)
        {
            newBoard.blankX = x;
            newBoard.blankY = y;
        }
        ++y;
        if(y%3 == 0)
        {
            y = 0;
            ++x;
        }
    }
    newBoard.fitness = getFitness(newBoard, heuristic);
    return newBoard;
}

void printBoard(Board board)
{
    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            cout<<board.tiles[x][y]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void printPath(Board board)
{
    cout<<"steps: "<<board.parentBoardCount<<endl;
    cout<<board.parents;
    cout<<tilesToString(board)<<endl;
}

string tilesToString(Board board)
{
    string tiles = "";
    tiles += "| ";
    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            tiles += (char)(board.tiles[x][y] + 48);
        }
        //tiles += "|";
        tiles += " ";
    }
    //tiles += "\n";
    tiles += "|\n";
    return tiles;
}

int getFitness(Board board, int heuristic)
{
    switch (heuristic)
    {
        case 0:
            return oopFitness(board);
        case 1: 
            return manhattan(board);
        case 2:
            return linearCon(board);
    }
    return 0;
}


//out of place fitness
int oopFitness(Board board)
{
    int fitness = 0;
    if(board.tiles[0][0] != 1)
        ++fitness;
    if(board.tiles[0][1] != 2)
        ++fitness;
    if(board.tiles[0][2] != 3)
        ++fitness;
    if(board.tiles[1][0] != 4)
        ++fitness;
    if(board.tiles[1][1] != 5)
        ++fitness;
    if(board.tiles[1][2] != 6)
        ++fitness;
    if(board.tiles[2][0] != 7)
        ++fitness;
    if(board.tiles[2][1] != 8)
        ++fitness;
    if(board.tiles[2][2] != 0)
        ++fitness;
    return fitness;
}

int manhattan(Board board)
{
    int fitness = 0;
    for(int a = 0; a < 3; ++a)
    {
        for(int b = 0; b < 3; ++b)
        {
            for(int c = 0; c < 3; ++c)
            {
                for(int d = 0; d < 3; ++d)
                {
                    if(board.tiles[a][b] == solution[c][d])
                    {
                        fitness += abs(a - c) + abs(b - d);
                    }
                }
            }
        }
    }
    return fitness;
}

int linearCon(Board board)
{
    int fitness = 0;
    int conflict = 0;

    for(int a = 0; a < 3; ++a)
    {
        for(int b = 0; b < 3; ++b)
        {
            for(int c = 0; c < 3; ++c)
            {
                for(int d = 0; d < 3; ++d)
                {
                    if(board.tiles[a][b] == solution[c][d])
                    {
                        fitness += abs(a - c) + abs(b - d);
                    }
                }
            }
        }
    }

    for(int a = 0; a < 3; ++a)
    {
        for(int b = 0; b < 3; ++b)
        {
            if(a > 0)
            {
                if(board.tiles[b][a-1] == solution[b][a] && board.tiles[b][a] == solution[b][a-1])
                {
                    ++conflict;
                }
            }
            if(a < 2)
            {
                if(board.tiles[b][a+1] == solution[b][a] && board.tiles[b][a] == solution[b][a+1])
                {
                    ++conflict;
                }
            }
            if(b > 0)
            {
                if(board.tiles[b-1][a] == solution[b][a] && board.tiles[b][a] == solution[b-1][a])
                {
                    ++conflict;
                }
            }
            if(b < 2)
            {
                if(board.tiles[b+1][a] == solution[b][a] && board.tiles[b][a] == solution[b+1][a])
                {
                    ++conflict;
                }
            }

        }
    }    

    return fitness + (2 * conflict);
}

Board moveTile(Board board, int destX, int destY, int heuristic)
{
    Board newBoard;

    newBoard.blankX = destX;
    newBoard.blankY = destY;
    
    //deep copy
    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            newBoard.tiles[x][y] = board.tiles[x][y];
        }
    }
    

    //swap
    newBoard.tiles[board.blankX][board.blankY] = newBoard.tiles[destX][destY];
    newBoard.tiles[destX][destY] = 0;

    newBoard.fitness = getFitness(newBoard, heuristic);

    return newBoard;
}

void makeChilds(Board board, int heuristic)
{
    Board newBoard;
    int newChildCounter = 0;

    if(board.blankX - 1 >= 0)
    {
        //up

        newBoard = moveTile(board, board.blankX - 1, board.blankY, heuristic);
        if(!scanOpen(newBoard) && !scanClose(newBoard))
        {
            newBoard.parents = board.parents + tilesToString(board);
            newBoard.parentBoardCount = board.parentBoardCount + 1;
            ++OPENLISTEND;
            OPENLIST[OPENLISTEND] = newBoard;
            ++newChildCounter;
        }
    }
    if(board.blankX + 1 <= 2)
    {
        //down

        newBoard = moveTile(board, board.blankX + 1, board.blankY, heuristic);
        if(!scanOpen(newBoard) && !scanClose(newBoard))
        {
            newBoard.parents = board.parents + tilesToString(board);
            newBoard.parentBoardCount = board.parentBoardCount + 1;
            ++OPENLISTEND;
            OPENLIST[OPENLISTEND] = newBoard;
            ++newChildCounter;
        }
    }
    if(board.blankY - 1 >= 0)
    {
        //left

        newBoard = moveTile(board, board.blankX, board.blankY - 1, heuristic);
        if(!scanOpen(newBoard) && !scanClose(newBoard))
        {
            newBoard.parents = board.parents + tilesToString(board);
            newBoard.parentBoardCount = board.parentBoardCount + 1;
            ++OPENLISTEND;
            OPENLIST[OPENLISTEND] = newBoard;
            ++newChildCounter;
        }
    }
    if(board.blankY + 1 <= 2)
    {
        //right

        newBoard = moveTile(board, board.blankX, board.blankY + 1, heuristic);
        if(!scanOpen(newBoard) && !scanClose(newBoard))
        {
            newBoard.parents = board.parents + tilesToString(board);
            newBoard.parentBoardCount = board.parentBoardCount + 1;
            ++OPENLISTEND;
            OPENLIST[OPENLISTEND] = newBoard;
            ++newChildCounter;
        }
    }
    board.childCounter = newChildCounter;    
}

void bestFirstWrapper(Board board, int heuristic)
{
    int attempts = 0;
    OPENLISTEND = 0;
    CLOSEDLISTEND = 0;
    OPENLIST[OPENLISTEND] = board;
    ++OPENLISTEND;
    bestFirst(attempts, heuristic);
}

void bestFirst(int attempts, int heuristic)
{
    //do work until too many attempts or solution found
    if(attempts >= END)
    {
        cout<<"\nNo solution found, maximum attempts reached.\n"<<endl;
        return;
    }

    if(OPENLISTEND == 0)
    {
        cout<<"\nThis puzzle could not be solved with the given input.\n"<<endl;
        return;
    }

    //find most fit board
    Board mostfit = OPENLIST[0];
    int mostfitLocation = 0;
    for(int i = 1; i < OPENLISTEND; ++i)
    {
        if(mostfit.fitness > OPENLIST[i].fitness)
        {
            mostfit = OPENLIST[i];
            mostfitLocation = i;
        }
    }

    //move most fit board to closed list
    shiftList(mostfitLocation);

    if(mostfit.fitness == 0)
    {
        cout<<"\nSolution found"<<endl;
        cout<<"attempts: "<<attempts<<endl;
        printPath(mostfit);
        return;
    }
    else
    {
        CLOSEDLIST[CLOSEDLISTEND] = mostfit;
        ++CLOSEDLISTEND;
    }

    //add children to open list 
    makeChilds(mostfit, heuristic);

    return bestFirst(attempts + 1, heuristic);
}

void aStar2(Board board, int heuristic)
{
    int attempts = 0;
    bool solutionFound = false;
    OPENLISTEND = 0;
    CLOSEDLISTEND = 0;
    OPENLIST[OPENLISTEND] = board;
    ++OPENLISTEND;
    while(OPENLISTEND > 0 && attempts < END && !solutionFound)
    {
        Board mostfit = OPENLIST[0];
        int mostfitLocation = 0;    
        for(int i = 1; i < OPENLISTEND; ++i)
        {
            if(mostfit.fitness + mostfit.parentBoardCount > OPENLIST[i].fitness + OPENLIST[i].parentBoardCount)
            {
                mostfit = OPENLIST[i];
                mostfitLocation = i;
            }
        }
        if(mostfit.fitness == 0)
        {
            solutionFound = true;
            cout<<"\nSolution found"<<endl;
            cout<<"attempts: "<<attempts<<endl;
            printPath(mostfit); 
        }
        else
        {
            //move most fit board to closed list
            shiftList(mostfitLocation);
            CLOSEDLIST[CLOSEDLISTEND] = mostfit;
            ++CLOSEDLISTEND;
            makeChilds(mostfit, heuristic);
            ++attempts;
        }

    }
    if(!solutionFound)
    {
        cout<<"\nAttempts exceeded..."<<endl;
    }

}

bool compareBoard(Board oldBoard, Board newBoard)
{
    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            if(oldBoard.tiles[x][y] != newBoard.tiles[x][y])
            {
                //board is different
                return false;
            }
        }
    }
    //board is the same
    return true;
}

bool scanOpen(Board board)
{
    for(int i = 0; i < OPENLISTEND; ++i)
    {
        if(compareBoard(OPENLIST[i], board))
        {
            return true;
        }
    }
    return false;
}

bool scanClose(Board board)
{
    for(int i = 0; i < CLOSEDLISTEND; ++i)
    {
        if(compareBoard(CLOSEDLIST[i], board))
        {
            return true;
        }
    }
    return false;
}

void shiftList(int mostfitLocation)
{
    for(int i = mostfitLocation; i < OPENLISTEND; ++i)
    {
        if(i + 1 <= OPENLISTEND)
        {
            OPENLIST[i] = OPENLIST[i + 1];
        }
    }
    --OPENLISTEND;
}
