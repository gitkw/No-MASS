
#include <cmath>
#include "Model_Appliance_Ownership.h"

Model_Appliance_Ownership::Model_Appliance_Ownership(){}

double Model_Appliance_Ownership::dishwasher(const int socclass, const int internet, const int cohabitants,
  const int hometype, const int ownrent, const int bedrooms, const int internetreg, const int houseyear) const
{
  double socclassA[5] = {0.099,-0.076,-0.228,-0.29,-0.587};
  double internetA[2] = {0,0.592};
  double cohabitantsA[] = {-1.092,-0.338};
  double hometypeA[] = {-1.375,-1.452,-1.161,-1.779,-1.317};
  double ownrentA[] = {-1.256,-0.929,-0.094,0.122};
  double bedroomsA[] = {-0.908,-0.871,-0.151,0.725,1.037};
  double internetregA[] = {0,-0.106};
  double houseyearA[] = {0.001};
  double constant=0.228;

  double d = std::exp(constant + socclassA[socclass] + internetA[internet] + cohabitantsA[cohabitants]
    + hometypeA[hometype]+ ownrentA[ownrent]+ bedroomsA[bedrooms]+ internetregA[internetreg]+ houseyearA[houseyear]);
  double Ownership = d/ (1+d);
  return Ownership;
}

double Model_Appliance_Ownership::tvless21(const int sex, const int internet, const int cohabitants
    , const int over15, const int education) const
  {

  double sexA[2] = {0,-0.271};
  double internetA[] = {0,0.113};
  double cohabitantsA[] = {-0.784,-0.337};
  double over15A[] = {-0.138,-0.32,-0.095,0.131,0.003,0.595};
  double educationA[] = {-0.073,-0.064,0.101,0.021,-0.346};
  double constant=1.327;

  double d = std::exp(constant + sexA[sex] + internetA[internet] + cohabitantsA[cohabitants] + over15A[over15] + educationA[education]);
  double Ownership = d/ (1+d);
  return Ownership;
}

double Model_Appliance_Ownership::tvmore21(const int sex, const int employment, const int cohabitants
  , const int hometype, const int ownrent, const int bedrooms
, const int education, const int internetregother) const
  {
  double sexA[2] = {0,0.507};
  double employmentA[] = {0.744,0.247,0.33,0.095,0.692,0.543};
  double cohabitantsA[] = {-0.967,-0.446};
  double hometypeA[] = {0.554,0.22,-0.058,0.008,-0.135};
  double ownrentA[] = {0.777,1.421,1.27,1.658};
  double bedroomsA[] = {-1.98,-1.57,-1.271,-0.94,-0.635};
  double educationA[] = {-0.266,0.21,0.571,0.566,0.309};
  double internetregotherA[2] = {0,0.422};
  double constant=0.536;

  double d = std::exp(constant + sexA[sex] + employmentA[employment] + cohabitantsA[cohabitants]
    + hometypeA[hometype]+ ownrentA[ownrent]+ bedroomsA[bedrooms]
  + educationA[education]+ internetregotherA[internetregother]);
  double Ownership = d/ (1+d);
  return Ownership;
}

double Model_Appliance_Ownership::microware() const
{
  return 0.8;
}

double Model_Appliance_Ownership::cooker(const int age, const int employment, const int bedrooms, const int education) const
{
  double ageA[] = {0.11,0.587,0.379,0.081,0.137,0.209};
  double employmentA[] = {0.84,0.961,0.623,0.698,0.371,0.724};
  double bedroomsA[] = {0.958,-0.059,0.184,0.617,0.688};
  double educationA[] = {0.186,-0.256,-0.001,0.018,0.277};
  double constant=-0.246;
  double d = std::exp(constant + ageA[age] + employmentA[employment] + bedroomsA[bedrooms]
    + educationA[education]);
  double Ownership = d/ (1+d);
  return Ownership;
}

double Model_Appliance_Ownership::fridge() const
{
  return 1.0;
}

double Model_Appliance_Ownership::washingMachine(const int sex, const int ownrent, const int bedrooms, const int internetregother) const
{

  double sexA[2] = {0,-0.824};
  double ownrentA[4] = {-0.609, -0.309, 0.765, 1.134};
  double bedroomsA[5] = {0.673, 1.493, 2.042, 3.198, 3.678};
  double internetregotherA[2] = {0,1.231};
  double constant = 1.325;

  double d = std::exp(constant + sexA[sex] + ownrentA[ownrent] + bedroomsA[bedrooms] + internetregotherA[internetregother]);
  double Ownership = d/ (1+d);
  return Ownership;
}


/*

t’dryer,
double internet[2] = {0,0.493};
double cohabitants[2] = {-1.085,-0.449};
double bedrooms[5] = {-0.593,-1.099,-0.174,0.346,0.905};
double constant=0.918;

s’instant,
double employment[] = {0.054,-0.373,-0.313,-0.067,-0.282,-0.176};
double hometype[] = {0.101,1.269,1.177,1.072,1.299};
double ownrent[] = {0.227,-0.256,0.336,0.454};
double bedrooms[] = {-0.634,-0.841,-0.505,-0.206,-0.271};
double Constant=-0.214;

s’pumped,
double socclass[] = {0.263,-0.078,-0.253,-0.126,-0.03};
double over15[] = {0.469,0.255,0.605,0.443,0.666,0.093,1.236};
double bedrooms[] = {1.059,0.805,0.92,1.155,1.409};
double internetreg[] = {0.163,0.216};
double Constant=-2.625;

c’relectric,

el’heater,
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double cohabitants[] = {};
double cohabitants[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double ownrent[] = {};
double ownrent[] = {};
double ownrent[] = {};
double ownrent[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double internetreg1[] = {};
double houseyear,Constant=;
-0.887,-0.611,-0.268,0.008,0.255,0.68,-0.304,-0.423,-0.651,-0.611,-0.135,0.407,-0.041,-2.093,-1.852,-1.612,-2.003,-1.627,1.002,0.27,0.657,0.292,0.149,0.044,0.236,0.237,0.427,0.231,-0.002,3.481,
freezer,
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double Constant=;
-1.158,-0.904,-0.574,-0.387,-0.37,-0.413,0.696,0.702,1.017,0.534,1.092,-1.773,-1.205,-1.254,-1.075,-1.067,-0.774,-0.455,-0.322,-0.394,-0.017,0.131,0.572,0.833,
w’pump,
double sex[] = {};
double employment[] = {};
double employment[] = {};
double employment[] = {};
double employment[] = {};
double employment[] = {};
double employment[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double bedrooms[] = {};
double Constant=;
0.333,0.075,0.206,0.519,-0.122,-0.308,-0.302,-0.65,-1.117,-0.38,-1.345,-0.21,0.207,0.014,0.074,0.151,0.754,-1.136,
immersion  ,
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double internet[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double hometype[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double Constant=;
0.347,0.329,0.174,0.127,-0.541,0.245,0.856,1.285,1.133,0.82,0.928,-0.099,-0.354,-0.194,-0.105,0.175,-0.159,,



desktop ,
double internet[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double Constant=;
2.064,-0.885,-0.107,-0.175,0.377,0.315,0.525,0.523,-1.504
laptop,
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double age[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double socclass[] = {};
double internet[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double education[] = {};
double internetregoth,incomeclass[] = {};
double incomeclass[] = {};
double incomeclass[] = {};
double incomeclass[] = {};
double incomeclass[] = {};
double Constant=;
2.368,0.474,0.363,0.437,0.057,-0.428,0.391,0.085,0.028,-0.088,-0.261,2.166,-1.548,-1.419,-1.623,-1.157,-0.762,-1.179,-0.319,-0.636,-0.336,-0.159,-0.248,0.077,0.296,-0.247,0.015,0.097,-0.091,0.334,-0.201
g’console,
double cohabitants[] = {};
double cohabitants[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double over15[] = {};
double internetreg[] = {};
double Constant=;
-5.423,-2.327,-1.352,-2.005,-0.669,0.109,0.301,0.244,0.636,1.999


*/
