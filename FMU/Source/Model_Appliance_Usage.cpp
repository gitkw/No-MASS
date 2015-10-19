
#include "Utility.h"
#include "Model_Appliance_Usage.h"

Model_Appliance_Usage::Model_Appliance_Usage(){
  stateTv = Utility::randomInt(0.0,1.0);
  stateWashingMachine = Utility::randomInt(0.0,1.0);
  stateMicroWave = Utility::randomInt(0.0,1.0);
  stateCooker = Utility::randomInt(0.0,1.0);
  stateDishWasher = Utility::randomInt(0.0,1.0);
  stateFridge = Utility::randomInt(0.0,1.0);
}


double Model_Appliance_Usage::consumption(const int app, const int timeStep){
  double result = 0.0;
  if(onAt(app, timeStep)){
    result = getPower(app);
  }
  return result;
}

double Model_Appliance_Usage::getTransitionAt(const int app, const int state, const int i){
  double transition;
  switch(app){
    case 0:
      transition = transitionsCooker[state][i];
      break;
    case 1:
      transition = transitionsMicroWave[state][i];
      break;
    case 2:
      transition = transitionsTv[state][i];
      break;
    case 3:
      transition = transitionsWashingMachine[state][i];
      break;
    case 4:
      transition = transitionsDishWasher[state][i];
      break;
    case 5:
      transition = transitionsFridge[state][i];
      break;
  }
  return transition;
}

double Model_Appliance_Usage::getPower(const int app){
  double power = 0;
  int state;
  switch(app){
    case 0:
        power = getMeanFraction(app) * maxPowerCooker;
      break;
    case 1:
        power = getMeanFraction(app) * maxPowerMicroWave;
      break;
    case 2:
        power = getMeanFraction(app) * maxPowerTv;
      break;
    case 3:
        power = getMeanFraction(app) * maxPowerWashingMachine;
      break;
    case 4:
      power = getMeanFraction(app) * maxPowerDishWasher;
      break;
    case 5:
      power = getMeanFraction(app) * maxPowerFridge;
      break;
  }
  return power;
}

double Model_Appliance_Usage::getMeanFraction(const int app){
  int state;
  switch(app){
    case 0:
        state = stateCooker;
      break;
    case 1:
        state = stateMicroWave;
      break;
    case 2:
        state = stateTv;
      break;
    case 3:
        state = stateWashingMachine;
      break;
    case 4:
        state = stateDishWasher;
      break;
    case 5:
        state = stateFridge;
      break;
  }

  double random = Utility::randomDouble(0.0,1.0);
  double currSum = 0;
  for(int i =0; i < 10; i++){
    currSum = currSum + getTransitionAt(app, state, i);
    if(random < currSum){
      state = i;
      break;
    }
  }

  double MeanF = 0;
  switch(app){
    case 0:
        MeanF = meanFCooker[state];
        stateCooker = state;
      break;
    case 1:
        MeanF = meanFMicrowave[state];
        stateMicroWave = state;
      break;
    case 2:
        MeanF = meanFTv[state];
        stateTv = state;
      break;
    case 3:
        MeanF = meanFWashingMachine[state];
        stateWashingMachine = state;
      break;
    case 4:
        MeanF = meanFDishWasher[state];
        stateDishWasher = state;
      break;
    case 5:
        MeanF = meanFFridge[state];
        stateFridge = state;
      break;
  }

  return MeanF;
}


bool Model_Appliance_Usage::onAt(const int app, const int timeStep) const{
  double probability = 0.0;
  switch(app){
    case 0:
        probability = onCooker[timeStep];
      break;
    case 1:
        probability = onMicroWave[timeStep];
      break;
    case 2:
        probability = onTv[timeStep];
      break;
    case 3:
        probability = onWashingMachine[timeStep];
      break;
    case 4:
        probability = onDishWasher[timeStep];
      break;
    case 5:
        probability = onFridge[timeStep];
      break;
  }
  double random = Utility::randomDouble(0.0,1.0);
  bool on = false;
  if(random < probability){
      on = true;
  }
  return on;
}


/*
int Model_Appliance_Usage::print()
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
