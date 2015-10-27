//
//  main.cpp
//  MLP_framework
//
//  Created by Shauharda Khadka on 9/30/15.
//  Copyright © 2015 Shauharda Khadka. All rights reserved.
//

using namespace std;

#include <vector>
#include <iostream>
#include "Class_neuralnet_architecture.h"
#include <fstream>
#include <cmath>

#define total_epoch 100
#define total_stat_run 10
#define eta 0.1
#define linear_discount 01 //Factor multiplied to eta for the training of the linear output layer
#define tolerance 0.49
#define num_io 5


double choose_random(int start, int end, int precision);
void gradient_descent(neuralnet &neural_net, vector<double> error, bool output_sigmoid = false);
void get_training_set(vector<vector<double>> &training_set, string filename);
void get_training_set(vector<vector<double>> &training_set);
void print2DVector(vector< vector<double> > const &vector);
void print_Vector(vector<double> const &vector);
bool is_fail(double y1, double y2);
int get_num_spam(vector <vector<double>> set);
void write_to_csv_2D(vector< vector<double> > const &vector, string filename);
void compute_standard_error(vector< vector<double> > const &in_vector, string filename);

int main()
{
    
    
     //Populate training set
    vector <vector<double>> training_set (200, vector<double>(num_io * 2, 777));
    //get_training_set(training_set);
    get_training_set(training_set);    //Populate test set
    
    //Populate test set
    vector <vector<vector<double>>> test_set (3, vector<vector<double>>(200, vector<double>(num_io * 2,100))); //3d vector with each index storing a full 2d array of test data points
    get_training_set(test_set[0]);
    get_training_set(test_set[1]);
    get_training_set(test_set[2]);
    
    //Metric to store classification error performance
    vector <vector<vector<double>>> classification_error(test_set.size()+1, vector<vector<double>>(total_stat_run, vector<double>(total_epoch, 0)));
    
    
    //Start Stat runs
    for (int stat_run = 0; stat_run < total_stat_run; stat_run++)
        
    {
    
        //Call a neural network
        vector <int> network_topology = {num_io, 5, num_io};
        neuralnet auto_encoder(network_topology);
   
        //Train the network
        for (int epoch = 0; epoch < total_epoch; epoch++)
        {
            //Shuffle training data
            random_shuffle(training_set.begin(), training_set.end());
        
        
            for (int counter = 0; counter < training_set.size(); counter++)
            {
                auto_encoder.feedforward(training_set[counter]);//Feedforward through the network
                //Set (training_set[counter], 1) to make the output layer have non-linearity (sigmoid activation)..By default no parameters will yield linear AF in the output layer
                
                for (int i = 0; i < auto_encoder.num_output; i++)
                {
                    if (is_fail(auto_encoder.network_output[i], training_set[counter][auto_encoder.num_output + i]))
                    {
                        classification_error[0][stat_run][epoch] += 0.5;
                        break;
                    }
                    
                }
                
                //Compute error
                vector <double> error(auto_encoder.num_output);
                for (int i = 0; i < auto_encoder.num_output; i++)
                {
                    error[i] =  training_set[counter][auto_encoder.num_input + i] - auto_encoder.network_output[i];
                }
                
                
                //Learning algorithm (gradient descent)
                gradient_descent(auto_encoder, error);
                //Set (auto-encoder, error, 1) to make the output layer have non-linearity (sigmoid activation)..By default no parameters will yield linear AF in the output layer
                
            }//Training round complete
        
        
            
            //Testing
            for (int ii = 1; ii < classification_error.size(); ii++)
            {
         
                for (int counter = 0; counter < test_set[ii-1].size(); counter++)
                {
                    auto_encoder.feedforward(test_set[ii-1][counter]);//Feedforward through the network
                    //Set (training_set[ii-1][counter], 1) to make the output layer have non-linearity (sigmoid activation)..By default no parameters will yield linear AF in the output layer
                    
                    for (int i = 0; i < auto_encoder.num_output; i++)
                    {
                        if (is_fail(auto_encoder.network_output[i], training_set[counter][auto_encoder.num_output + i]))
                        {
                            classification_error[ii][stat_run][epoch] += 0.5;
                            break;
                        }
                        
                    }
                
                }//Loop around each test set
            }//Finished testing
            
        
        }//End of one epoch (training and testing)
    }//End of statistical runs
    
   
   
    
    //print2DVector(classification_error[2]);
    //write_to_csv_2D(classification_error[1], "Data_test1.csv");
    //write_to_csv_2D(classification_error[2], "Data_test2.csv");
    //write_to_csv_2D(classification_error[3], "Data_test3.csv");
    compute_standard_error(classification_error[0], "Data_training_set.csv");
    compute_standard_error(classification_error[1], "Data_test_set_1.csv");
    compute_standard_error(classification_error[2], "Data_test_set_2.csv");
    compute_standard_error(classification_error[3], "Data_test_set_3.csv");
    return 0;
    
    
}

bool is_fail(double y1, double y2)
{
    if (y2 > (y1 - tolerance) && y2 < (y1 + tolerance))
        return false;
    
    return true;
}

void gradient_descent(neuralnet &neural_net, vector<double> error, bool output_sigmoid)
 {
 
 
 
 vector <vector<double>> delta (neural_net.network.size()); //Delta keep track of deltas s.t. index last -> output layer
 
     //Set delta[last] referring to the output layer(linear AF)
     for (int i = 0; i < neural_net.num_output; i++)//
     {
         if(output_sigmoid == false)
            {delta[delta.size() - 1].push_back(error[i]);}
         else
         {
             delta[delta.size() - 1].push_back(error[i]*(neural_net.network_output[i])*(1-neural_net.network_output[i]));//Non-Linear activation
         }
     }
     
     //Set all other delta
     for (int i = delta.size()-2; i >= 0; i--)//Each layers of delta except the output layer
     {
         for (int n = 0; n < neural_net.network[i].size(); n++)//for each neurone in that layer
         {
             double summation_term = 0;
             for (int w = 0; w < neural_net.network[i+1].size(); w++)//for all connections of that neurone to the next layer
             {
                 summation_term += delta[i+1][w] * neural_net.network[i+1][w].weights[n+1];
             }
             delta[i].push_back(summation_term * neural_net.network[i][n].output * (1 - neural_net.network[i][n].output));
        }
     }
     
     //Weight update
     for (int layer = 0; layer < neural_net.network.size(); layer++)//for each neuronal layer
     {
         for (int n = 0; n < neural_net.network[layer].size(); n++)//for each neurones
         {
             for (int w = 0; w < neural_net.network[layer][n].weights.size(); w++)//for each weight
             {
                 if (layer == neural_net.network.size() - 1 && output_sigmoid == false)
                 {
                   neural_net.network[layer][n].weights[w] += linear_discount * eta * delta[layer][n] * neural_net.layer_buffer[layer][w];
                 }
                 else
                 {
                     neural_net.network[layer][n].weights[w] += eta * delta[layer][n] * neural_net.layer_buffer[layer][w];
                 }
             }
    
         }
         
     }
     
     
 }//End of gradient descent function




void write_to_csv_2D(vector< vector<double> > const &vector, string filename)
{
    ofstream myfile;
    myfile.open (filename);
    myfile.precision(4);
    for (int epoch = 0; epoch < vector[0].size(); epoch++)
    {
    
        for (int stat_run = 0; stat_run < vector.size(); stat_run++)
            {
                myfile << vector[stat_run][epoch] << ',';
            }
        myfile<<endl;
    
    }
    myfile.close();
}


void print2DVector(vector< vector<double> > const &vector)
{
    for (int i = 0; i < vector.size(); i++)
    {
        for (int j = 0; j < vector[i].size(); j++)
        {
            cout << vector[i][j] << "    ";
        }
        cout << endl<<endl;
    }
    
}

void print_Vector(vector<double> const &vector)
{
    for (int i = 0; i < vector.size(); i++)
    {
        cout << vector[i]<<endl;

    }
    
}

void compute_standard_error(vector< vector<double> > const &in_vector, string filename)
{
    vector <vector<double>> output_matrix (3, vector<double> (in_vector[0].size(), 0));
    
    //Compute average
    for (int epoch = 0; epoch < in_vector[0].size(); epoch++)
    {
        output_matrix[0][epoch]=epoch+1;
        
        for (int stat_run = 0; stat_run < in_vector.size(); stat_run++)
        {
            output_matrix[1][epoch] += in_vector[stat_run][epoch]/in_vector.size();
        }
        
    }
    
    //COmpute sd
    for (int epoch = 0; epoch < in_vector[0].size(); epoch++)
    {
        for (int stat_run = 0; stat_run < in_vector.size(); stat_run++)
        {
            output_matrix[2][epoch] += (output_matrix[1][epoch] - in_vector[stat_run][epoch]) *
            (output_matrix[1][epoch] - in_vector[stat_run][epoch]) / in_vector.size();
        }
        
    }
    
    for (int epoch = 0; epoch < in_vector[0].size(); epoch++)
    {
        if (epoch % 10 == 0)
             output_matrix[2][epoch] = sqrt(output_matrix[2][epoch])/(sqrt(10));
        else
            output_matrix[2][epoch] = 0;
    }
    
    
    ofstream myfile;
    myfile.open (filename);
    myfile.precision(4);
    for (int epoch = 0; epoch < in_vector[0].size(); epoch++)
    {
        for (int stat_run = 0; stat_run < output_matrix.size(); stat_run++)
        {
            myfile << output_matrix[stat_run][epoch] << ',';
        }
        myfile<<endl;
        
    }
    myfile.close();
    
}

