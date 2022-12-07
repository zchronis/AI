#include <math.h>
#include <string>
#include <stdlib.h>
#include <iostream>


using namespace std;

float rando();
float normal(float, float, float, float);
int main()
{
    srand(time(NULL));
    float learningRate = 0.1;
    float x = rando();
    float y = rando();
    float prevX = x;
    float prevY = y;

    for(int i = 0; i < 500; ++i)
    {
        
        if(i > 0 && normal(x, prevX, y, prevY) < 0.00001)
        {
            cout<<"normal: "<<normal(x, prevX, y, prevY)<<endl;
            cout<<"Solution: "<<i<<endl;
            cout<<"x: "<<x<<endl;
            cout<<"y: "<<y<<endl;
            return 1;
        }
        prevX = x;
        prevY = y;
        x -= learningRate * (10*x + 40); 
        y -= learningRate * (2*y - 12);
    }
    cout<<"FAIL"<<endl;
    cout<<"x: "<<x<<endl;
    cout<<"y: "<<y<<endl;
    return 0;
}

float rando()
{    
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = 10.0 - -10.0;
    float r = random * diff;
    return -10.0 + r;
}

float normal(float x1, float x2, float y1, float y2)
{
    float ans = 0;
    float xPow = pow(x1 - x2, 2);
    float yPow = pow(y1 - y2, 2);
    ans = sqrt(xPow + yPow);
    return ans;
}