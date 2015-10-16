#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <cassert>

#include "Model_Appliance_Ownership.h"

Appliance::Appliance(){
    mt.seed (rd());
}

std::array< double, 10 > Appliance::profile(){
      double Intercept[9]	= {3.433,	1.878,	-0.223,	-3.701,	0.676,	-0.03,	0.415,	-1.623,	-16.377};
      double age2[9] =	{0.493, 0.714, 0.564, -15.195, 0.487, -0.243, 0.912, 1.678, 1.357};
      double season2[9]= {0.048,	-0.057,	-0.033,	-0.102,	-0.137,	-0.06,	0.192,	0.338,	-0.077};
      double retired1[9] = {0,0,0,0,0,0,0,0,0};
      double unemp0[9] = {0.392,	-0.249,	0.173,	0.927,	-1.214,	-0.006,	0.321,	-0.054,	12.921};
      double civstat1[9] = {0.234,	0.018,	-0.051,	-0.88,	-0.001,	0.522,	0.176,	0.113,	0.833};
      double famstat3[9] = {0,0,0,0,0,0,0,0,0};
      double computer0[9] = {0.003,-0.125,	-0.068,	-0.822,	0.17,	-0.701,	-0.081,	-0.066,	0.106};
      double day1[9] = {0.799,	0.422,	0.386,	1.4,	-0.125,	-0.233,	0.304,	0.254,	0.704};
      double sex2[9] = {0,0,0,0,0,0,0,0,0};
      double edtry1[9] = {-0.056,	-0.314,	-0.189,	-0.188,	0.031,	-1.397,	-0.399,	0.222,	0.244};
      double g[10];
      std::array< double, 10 > P;

      //g1 = A0 + A1*X1+ A2*X2+……..+ An*Xn.
      for(int i =0; i < 9; i++){
        g[i] = Intercept[i] +age2[i] +season2[i] +computer0[i] +civstat1[i] +unemp0[i] +retired1[i] + edtry1[i] + famstat3[i] + day1[i] + sex2[i];
      }
      g[9] = 0;

      //P1 = exp(g1) / (exp(g1) + exp(g2)+……. + exp(g10)).
      double d =0;

      for(int i =0; i <= 9; i++){
        d += std::exp(g[i]);
      }

      double sum = 0;
      for(int i =0; i <= 9; i++){
        P[i] = std::exp(g[i]) / d;
        sum += P[i];
      }
      return P;
}

double Appliance::ownership()
{



  double sex1 = -0.824;
  double ownrent1 = -0.609;
  double bedrooms1 = 0.673;
  double internetregother = 1.231;
  double constant = 1.325;

  double d = std::exp(constant + ownrent1 + bedrooms1 + internetregother);
  double Ownership = d/ (1+d);
  return Ownership;
}


double Appliance::printer(){

std::cout << currentstates[0][0] << std::endl;
std::cout << currentstates[0][1] << std::endl;
std::cout << currentstates[1][0] << std::endl;
std::cout << currentstates[1][1] << std::endl;
}

int Appliance::getState(){

  return currentState;
}

double Appliance::getPower(){

  return getMeanFraction() * currentMaxPower;
}

double Appliance::getMeanFraction(){

  int state = stateWashingMachine;

  std::uniform_real_distribution<double> dist(0, 1);
  double random = dist(mt);
double currSum = 0;
  for(int i =0; i < 2; i++){
  currSum = currSum + currentTransition[stateWashingMachine][i];
    if(random < currSum){
      state = i;
      break;
    }
  }
  currentstates[stateWashingMachine][state]++;
  currentState = state;
  stateWashingMachine = state;
  return currentMeanF[state];
}


bool Appliance::onAt(const int timeStep){
  std::uniform_real_distribution<double> dist(0, 1);
  double random = dist(mt);

  if(random > currentOn[timeStep]){
      return true;
  }
  return false;
}


void Appliance::setAppliance(const int app){
  switch(app){
    case 1:
    /*
      currentOn = onTv;
      currentTransition = transitionsTv;
      currentMeanF = meanFTv;
      currentMaxPower = maxPowerTv;
*/
      break;
    case 2:
    currentOn = onWashingMachine;
    //currentTransition = transitionsWashingMachine;
    currentMeanF = meanFWashingMachine;
    currentMaxPower = maxPowerWashingMachine;
      break;
    case 3:

      break;
    case 4:

      break;


  }
}

/*
int Appliance::print()
{

  ///// general appraoch
  // turn on appliance

  int years = 1;
  double power[years][365][144];
  double on[years][365][144];
  double states[years][365][144];
  // for each year
  for(int y = 0; y < years; y++){
    // for each day
    for(int p = 0; p < 365; p++){
      // for each 10 minute
      for(int x = 0; x < 144; x++){
        // std::cout << "washingMachine switchon: " << washingMachine[x] << std::endl;
        // Duration of appliance use




        double random = dist(mt);

        power[y][p][x] = 0;
        on[y][p][x] = 0;
        states[y][p][x] = 0;

        if(random > tv[x]){
            continue;
        }




          int state;

          random = dist(mt);
          for(int i =0; i < sizeof(tvTransitions); i++){
            if(random < tvTransitions[i]){
              state = i+1;
              break;
            }
          }

          //    std::cout << "washingMachine ran: " << random << std::endl;
          //    std::cout << "washingMachine state: " << state << std::endl;

          // get mean F
          double meanF;
          // mean f form tables page 122
          switch(state){
            case 1:
              meanF = 0.0308;
              break;
            case 2:
              meanF = 0.2955;
              break;
          }

          switch(state){
            case 1:
              meanF = 0.7711;
              break;
            case 2:
              meanF = 0.4708;
              break;
            case 3:
              meanF = 0.1054;
              break;
            case 4:
              meanF = 0.9356;
              break;


          }

          //  std::cout << "washingMachine meanF: " << meanF << std::endl;

          // multiple mean f by maximum power taken from matlab scripts
          double maxPower = 82.34494;
          maxPower = 56.2;
          //  std::cout << "washingMachine power: " << meanF * maxPower  << std::endl;
          power[y][p][x] = meanF * maxPower;
          on[y][p][x] = 1;
          states[y][p][x] = state;

      }
    }
  }

    std::ofstream myfile;
    myfile.open ("power.dat");
    // for each day
    // for each 10 minute
    for(int y = 0; y < years; y++){
      for(int p = 0; p < 365; p++){
        for(int x = 0; x < 144; x++){
          myfile  << power[y][p][x] ;

            if(x < 144 -1)myfile<< ", ";
        }
        myfile  << std::endl;
      }
    }
    myfile.close();

      myfile.open ("on.dat");
      // for each day
      // for each 10 minute

    for(int y = 0; y < years; y++){
      for(int p = 0; p < 365; p++){
        for(int x = 0; x < 144; x++){

            myfile  << on[y][p][x];
            if(x < 144 -1)myfile<< ", ";

        }

        myfile  << std::endl;

      }
      }
      myfile.close();

      myfile.open ("states.dat");
      // for each day
      // for each 10 minute

    for(int y = 0; y < 1; y++){
      for(int p = 0; p < 365; p++){
        for(int x = 0; x < 144; x++){

            if(states[y][p][x] > 0){
                myfile  << states[y][p][x];
                myfile<< ", ";


            }
        }


      }
      }

        myfile  << std::endl;
      myfile.close();

  return 0;
}
*/
