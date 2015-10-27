
//
//  Class_MLP.h
//  MLP_framework
//
//  Created by Shauharda Khadka on 9/30/15.
//  Copyright © 2015 Shauharda Khadka. All rights reserved.
//

#ifndef Class_MLP_h
#define Class_MLP_h

#include <vector>
#include <cmath>

using namespace std;

double choose_random(int start, int end, int precision);

class neuron
{
    
public:
    vector <double> weights;//Set of weights for the neuron s.t 0 index refers to the bias weight
    double output;//Output of the specific neuron
    
    void randomize_weights(int n) //Takes input n (num of inputs) and initializes random weights
    {
        for (int i = 0; i < n; i++)
            weights.push_back(choose_random(-1, 1, 1000)); //Initialize weights randomly between -1 and 1 with precision 1000 or 3 --> 0.123 or 0.324
    }
    
    void feedforward(vector <double> input)
    {
        double x = 0;
        for (int i = 0; i < input.size(); i++)
        {
            x += input[i] * weights[i];
        }
        output = sigmoid(x);
        
    }
    
    void feedforward(vector <double> input, bool k)
    {
        double x = 0;
        for (int i = 0; i < input.size(); i++)
        {
            x += input[i] * weights[i];
        }
        output = x;
        
    }
    
    
    double sigmoid(double x)
    {
        return (1 / (1 + exp(-x)));
    }
    
};//End Class Neuron


class neuralnet
{
    
public:
    int num_input;
    int num_output;
    vector <vector<neuron>> network; // dim 1: no of neuronal layers starting with hidden dim 2: num of neurons in each layer
    vector <vector<double>> layer_buffer;
    vector <double> network_output;
    
    
    neuralnet(const vector<int> network_topology) //Constructor
    {
        
        num_input = network_topology[0];
        num_output = network_topology[network_topology.size()-1];
        network_output.resize(num_output);
        
        //Form network size (Neuronal layers = total layers - 1 (input layer))
        network.resize(network_topology.size()-1);
        for (int i = 0; i <network.size(); i++)
        {
            network[i].resize(network_topology[1+i]);
        }
        
        //Size layer_buffer
        layer_buffer.resize(network.size());//Number of buffer overlay needed (buffer to feed inputs to layers)
        for (int i = 0; i < layer_buffer.size(); i++)
        {
            layer_buffer[i].resize(network_topology[i]+1);
            layer_buffer[i][0] = 1; //Bias term
            
        }
        
        //Initialize random weights
        for (int layer = 0; layer < network.size(); layer++) // For each layer
        {
            for (int i = 0; i < network[layer].size(); i++) // for each neurone
            {
                network[layer][i].randomize_weights(network_topology[layer]+1);
            }
        }
    
    }
    
    void feedforward(vector <double> input, bool output_sigmoid = 0)
    {
        
        //Fill input layer_buffer or layer_buffer[0]
        for (int i = 0; i < layer_buffer[0].size()-1; i++)
        {
            layer_buffer[0][i+1] = input[i]; //Assign feedforward output as input to the next layer
        }
        
        //Feedforward loop
        for (int layer = 0; layer < network.size()-1; layer++)//for each neurnoal layer
        {
            
            //Feedforward
            for (int i = 0; i < network[layer].size(); i++)//for each neurone
            {
                network[layer][i].feedforward(layer_buffer[layer]);
               layer_buffer[layer+1][i+1] = network[layer][i].output; //Assign feedforward results to next layer buffer
            }
        
        }
        
        //Compute the network output
        for (int i = 0; i < num_output; i++)
        {
            //Use special feedforward (overload) to compute the linear combination but no sigmoid for the output layer
            
            //Choose to overload or not
            
            if (output_sigmoid == 0)
            {
                network[network.size()-1][i].feedforward(layer_buffer[layer_buffer.size()-1], 1);
            }
            
            else
            {
                network[network.size()-1][i].feedforward(layer_buffer[layer_buffer.size()-1]);
            }
            network_output[i] = network[network.size()-1][i].output;
        }
        
    }
    

};//End Class neuralnet











#endif /* Class_MLP_h */
