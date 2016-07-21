// Copyright 2016 Jacob Chapman

#ifndef Building_Appliances_H
#define	Building_Appliances_H

#include <vector>
#include "Appliance_Large.h"


class Building_Appliances {
public:
    Building_Appliances();

    void setup();
    void step();
    void postprocess();
    void postTimeStep();


private:
  std::vector<Appliance_Large> large;

};

#endif	/* Building_Appliances_H */
