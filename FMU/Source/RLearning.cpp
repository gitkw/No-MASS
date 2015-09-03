#include <algorithm>
#include <list>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "SimulationConfig.h"
#include "Utility.h"
#include "RLearning.h"

RLearning::RLearning()
{
}

void RLearning::setup(){
    epsilon = SimulationConfig::info.rlearn;
    std::ifstream in_file;
    in_file.open("rlearning" + std::to_string(id) + ".dat");
    if(in_file.fail())
    {
        for(int i =0; i < states; i++){
            for(int j =0; j < actions; j++){
                qTable[i][j] = 0;
            }
        }
    }else{
        for (int count =0; count < states; count++){
            int act =0;
            std::string a;
            in_file >> a;
            std::stringstream ss(a);
            std::string item;
            while (std::getline(ss, item, ',')) {
                qTable[count][act] = std::stod(item);
                act++;
            }
        }
    }
}

int RLearning::greedySelection(int s){
    int a;

    if (Utility::randomDouble(0,1) < 1-epsilon){
        a = getBestAction(s);
    } else {
        // selects a random action based on a uniform distribution
        a = Utility::randomDouble(0,actions);
    }
    return a;
}

int RLearning::getBestAction(int s)
{

    int m = 0;
    for(int i =1; i < actions; i++){
        if(qTable[s][m] < qTable[s][i]){
            m = i;
        }else if(qTable[s][m] == qTable[s][i] && Utility::tossACoin()){
            m = i;
        }
    }
    return m;
}


void RLearning::updateQ(int s, int a, double r, int sp){
        double maxQ = *std::max_element(qTable[sp],qTable[sp]+actions);
        qTable[s][a] = qTable[s][a] + alpha *  ( r + gamma*maxQ - qTable[s][a] );
}

void RLearning::printQ(){


    std::ofstream myfile;
    myfile.open ("rlearning" + std::to_string(id) + ".dat");

    for(int i =0; i < states; i++){
        for(int j =0; j < actions; j++){
            myfile  << qTable[i][j]  << ",";
        }
        myfile  <<  std::endl;
    }

    myfile.close();

}

void RLearning::setId(int id){
    this->id = id;
}
