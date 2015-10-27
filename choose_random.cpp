#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


using namespace std;



double choose_random(int start, int end)
{
    static bool isFirstRun(1);//Variable to ascertain that we only seed once
    if (isFirstRun == 1)
    {
        srand(time(NULL));
        isFirstRun = false;
    }
    
    double randomNum = (rand() % (1000 * (end - start)));
    randomNum = randomNum / 1000 + start;
    //generates randomNum between 1 and numOptions
    return randomNum;
}

double choose_random(int start, int end, int precision)
{
    static bool isFirstRun(1);//Variable to ascertain that we only seed once
    if (isFirstRun == 1)
    {
        srand(time(NULL));
        isFirstRun = false;
    }
    
    //Bound check
    if (start == end)
    {return start;}
    
    double randomNum = (rand() % (precision * ((end+1) - start)));
    randomNum = randomNum / precision + start;
    return randomNum;
}