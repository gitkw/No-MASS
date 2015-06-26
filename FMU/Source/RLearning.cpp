#include <algorithm>
#include <list>
#include "Utility.h"
#include "RLearning.h"

RLearning::RLearning()
{
    for(int i =0; i < 7; i++){
        for(int j =0; j < 4; j++){
            qTable[i][j] = 0;
        }
    }
}

int RLearning::greedySelection(int s){
    int a;
    if (Utility::randomDouble(0,1) > epsilon){
        a = getBestAction(s);
    } else {
        // selects a random action based on a uniform distribution
        a = Utility::randomDouble(0,4);
    }
    return a;
}

int RLearning::getBestAction(int s)
{

    int m = 0;
    for(int i =1; i < 4; i++){
        if(qTable[s][m] < qTable[s][i]){
            m = i;
        }else if(qTable[s][m] == qTable[s][i] && Utility::tossACoin()){
            m = i;
        }
    }
    return m;
}


void RLearning::updateQ(int s, int a, double r, int sp){
        double maxQ = *std::max_element(qTable[sp],qTable[sp]+4);
        qTable[s][a] = qTable[s][a] + alpha *  ( r + gamma*maxQ - qTable[s][a] );
}

void RLearning::printQ(){
    for(int i =0; i < 7; i++){
        for(int j =0; j < 4; j++){
            std::cout << i << " " << j << " " << qTable[i][j] << std::endl;
        }
    }
}
