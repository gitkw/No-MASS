// Copyright 2016 Jacob Chapman

#ifndef MODEL_APPLIANCE_OWNERSHIP_H_
#define MODEL_APPLIANCE_OWNERSHIP_H_

class Model_Appliance_Ownership {
 public:
  Model_Appliance_Ownership();
  double microware() const;
  double fridge() const;
  double cooker(const int age, const int employment, const int bedrooms, const int education) const;
  double dishwasher(const int socclass, const int internet, const int cohabitants, const int hometype, const int ownrent, const int bedrooms, const int internetreg, const int houseyear) const;
  double tvless21(const int sex, const int internet, const int cohabitants, const int over15, const int education) const;
  double tvmore21(const int sex, const int employment, const int cohabitants, const int hometype, const int ownrent, const int bedrooms, const int education, const int internetregother) const;
  double washingMachine(const int sex, const int ownrent, const int bedrooms, const int internetregother) const;

};

#endif  //  MODEL_APPLIANCE_OWNERSHIP_H_
