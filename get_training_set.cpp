#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>


using namespace std;

double choose_random(int start, int end, int precision);


#define b 0

void get_training_set(vector<vector<double>> &training_set, string filename)
{
    

    double a;
    string first; string line;
    int i(-1);
    ifstream file(filename);
    if (file.is_open())
    {
        
        while (getline(file, line, ','))
        {
            a = atof(line.c_str());
            if (i >=0 && i <1200)
                {training_set[(i)/6][(i % 6)+1] = a;}
            i++;
        }
        
    }
    file.close();
    
    
    ifstream myfile(filename);
    i = 0;
    
    if (myfile.is_open())
    {
        while (getline(myfile, line, '\n'))
        {
            a = atof(line.c_str());
            training_set[i][0] = a;
            i++;
        }
        
    }
    myfile.close();

}

void get_training_set(vector<vector<double>> &training_set)//AUTOENCODER

{
    for (int i = 0; i < training_set.size(); i++)
    {
        for (int k = 0; k < training_set[i].size()/2; k++)
        {
            //AutoEncoder
            training_set[i][k] = 0;
            training_set[i][(training_set[i].size()/2)+k] = training_set[i][k];
            
            
            /*
            training_set[i][k] = choose_random(1, 10, 1);
            training_set[i][(training_set[i].size()/2)+k] = training_set[i][k];
            */
        }
        int k = choose_random(0, training_set[0].size()/2-1, 1);
        training_set[i][k] = choose_random(2, 10, 1);
        training_set[i][(training_set[i].size()/2)+k] = training_set[i][k];
    }
    
}