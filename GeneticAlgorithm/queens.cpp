#include <math.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <time.h>

using namespace std;

const int POPULATION_SIZE = 500;
const int NUM_ITERATIONS = 100000;
const int MUTATION_PCT = 8;

struct Board
{
    int queenPos[8];
    int fitness = 0;
};


vector<Board> PARENTLIST;
vector<Board> CHILDLIST;

void makeBoards();
Board getRandom();
int getFitness(Board);
Board findParents();
Board coitus(Board, Board);
void sortChildren();
bool compare(Board, Board);
void report();
void age();
void printSolution(int);

int main()
{
    //DONE step 1: Create an intial population of random children
    //DONE step 2: Create a second generation of the same size as the original population
    //DONE substep 2.1: Select two parents use them to create a child, do this until second gen is the same size as initial
    //DONE step 3: repeat step 2 until max population reached.
    //DONE step 4: report once every 1000 epocs
    //DONE step 5: sort
    srand(time(NULL));
    makeBoards();
    
    for(int i = 0; i < NUM_ITERATIONS; ++i)
    {
        for(int j = 0; j < POPULATION_SIZE; ++j)
        {

            Board father = findParents();
            Board mother = findParents();
            Board child = coitus(father, mother);
            CHILDLIST.push_back(child);
        }
        sort(CHILDLIST.begin(), CHILDLIST.end(), compare);
        age();
        if(i % 1000 == 0)
        {
            cout<<"Iteration: #"<<i<<endl;
            cout<<"Most Fit Board: ";
            for(int i = 0; i < 8; ++i)
            {
                cout<<PARENTLIST[0].queenPos[i];
            }
            cout<<endl;
            report();
        }
    }
    cout<<endl;

    printSolution(0);
    
    return 0;
}

void makeBoards()
{
    for(int i = 0; i < POPULATION_SIZE; ++i)
    {
        PARENTLIST.push_back(getRandom());
    }
}

Board getRandom()
{
    Board newBoard;
    for(int i = 0; i < 8; ++i)
    {
        int random = rand() % 8;
        newBoard.queenPos[i] = random;
    }
    newBoard.fitness = getFitness(newBoard);
    return newBoard;
}

int getFitness(Board newBoard) {
    int score = 28;
    float xDiagonal;
    float yDiagonal;
    float current;

    for(int x = 0; x < 8; ++x) 
    {
        float y = newBoard.queenPos[x];
        for(int i = 0; i < 8; ++i) 
        {
            if(x == i) 
            {
                continue;
            }
            current = newBoard.queenPos[i];
            xDiagonal = abs(x - i);
            yDiagonal = abs(y - current);
            if(y == current)
            {
                --score;
            }
            else if(xDiagonal == yDiagonal)
            {
                --score;
            }
        }
    }
    return score;
}

Board findParents()
{
    float fitnessTotal = 0;

    for(int i = 0; i < POPULATION_SIZE; ++i)
    {
        fitnessTotal += PARENTLIST[i].fitness;
    }

    for(;;)
    {
        for(int i = 0; i < POPULATION_SIZE; ++i)
        {
            float tempFitness = PARENTLIST[i].fitness/fitnessTotal;
            float random = (rand() % 100)/100.0f;
            if(random < tempFitness)
            {
                return PARENTLIST[i];
            }
        }
    }
}

Board coitus(Board father, Board mother)
{
    Board child;
    int random = rand() % 7 + 1;

    for(int i = 0; i < 8; ++i)
    {
        if(i < random)
        {
            child.queenPos[i] = father.queenPos[i];
        }
        else
        {
            child.queenPos[i] = mother.queenPos[i];
        }
    }

    int xMan = rand() % 100 + 1;
    if(xMan < MUTATION_PCT)
    {
        int randQueen = rand() % 8;
        child.queenPos[randQueen] = rand() % 8;
    }

    child.fitness = getFitness(child);
    return child;
}

bool compare(Board one, Board two)
{
    if(one.fitness > two.fitness)
        return true;
    else
        return false;
}

void age()
{
    PARENTLIST.clear();
    for(int i = 0; i < POPULATION_SIZE; ++i)
    {
        PARENTLIST.push_back(CHILDLIST[i]);
    }
    CHILDLIST.clear();
}

void report()
{
    float avgFit = 0;
    for(int i = 0; i < POPULATION_SIZE; ++i)
    {
        avgFit += PARENTLIST[i].fitness;
    }
    avgFit = avgFit/POPULATION_SIZE;
    for(int i = 0; i < 8; ++i)
    {
        PARENTLIST[0].queenPos[i];
    }
    cout<<"Best Fitness: "<<PARENTLIST[0].fitness<<endl;
    cout<<"Avg  Fitness: "<<avgFit<<endl<<endl;
}

void printSolution(int boardNum)
{
    for(int i = 0; i < 8; ++i)
    {
        cout<<endl;
        for(int j = 0; j < 8; ++j)
        {
            if (PARENTLIST[boardNum].queenPos[j] == i)
            {
                cout<<"["<<PARENTLIST[boardNum].queenPos[j] + 1<<"]";
            }
            else
            {
                cout<<"[ ]";
            }
        }
    }
    cout<<endl;
}