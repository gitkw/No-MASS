// Copyright 2016 Jacob Chapman

#include <cmath>
#include <iostream>

#include "Model_Windows.h"

Model_Windows::Model_Windows() {
    durationOpen = 0;
    state = 0;

    aop = 2.151;
    bopout = 0.172;
    shapeop = 0.418;

    a01arr = -13.88;
    b01inarr = 0.312;
    b01outarr = 0.0433;
    b01absprevarr = 1.862;
    b01rnarr = -0.45;
    // P01int
    a01int = -12.23;
    b01inint = 0.281;
    b01outint = 0.0271;
    b01presint = -0.000878;
    b01rnint = -0.336;
    // P01dep
    a01dep = -8.75;
    b01outdep = 0.1371;
    b01absdep = 0.84;
    b01gddep = 0.83;
    // P10dep
    a10dep = -8.54;
    b10indep = 0.213;
    b10outdep = -0.0911;
    b10absdep = 1.614;
    b10gddep = -0.923;
}

void Model_Windows::setDurationVars(double aop, double bopout, double shapeop) {
    this->aop = aop;
    std::cout << "aop " << aop << std::endl;
    this->bopout = bopout;
    std::cout << "bopout " << bopout << std::endl;
    this->shapeop = shapeop;
    std::cout << "shapeop " << shapeop << std::endl;
}

void Model_Windows::setArrivalVars(double a01arr, double b01inarr,
      double b01outarr, double b01absprevarr, double b01rnarr) {
    this->a01arr = a01arr;
    std::cout << "a01arr " << a01arr << std::endl;
    this->b01inarr = b01inarr;
    std::cout << "b01inarr " << b01inarr << std::endl;
    this->b01outarr = b01outarr;
    std::cout << "b01outarr " << b01outarr << std::endl;
    this->b01absprevarr = b01absprevarr;
    std::cout << "b01absprevarr " << b01absprevarr << std::endl;
    this->b01rnarr = b01rnarr;
    std::cout << "b01rnarr " << b01rnarr << std::endl;
}

void Model_Windows::setInterVars(double a01int, double b01inint,
      double b01outint, double b01presint, double b01rnint) {
  this->a01int = a01int;
  std::cout << "a01int " << a01int << std::endl;
  this->b01inint = b01inint;
  std::cout << "b01inint " << b01inint << std::endl;
  this->b01outint = b01outint;
  std::cout << "b01outint " << b01outint << std::endl;
  this->b01presint = b01presint;
  std::cout << "b01presint " << b01presint << std::endl;
  this->b01rnint = b01rnint;
  std::cout << "b01rnint " << b01rnint << std::endl;
}

void Model_Windows::setDepartureVars(double a01dep, double b01outdep,
    double b01absdep, double b01gddep, double a10dep, double b10indep,
    double b10outdep, double b10absdep, double b10gddep) {
  this->a01dep = a01dep;
  std::cout << "a01dep " << a01dep << std::endl;
  this->b01outdep = b01outdep;
  std::cout << "b01outdep " << b01outdep << std::endl;
  this->b01absdep = b01absdep;
  std::cout << "b01absdep " << b01absdep << std::endl;
  this->b01gddep = b01gddep;
  std::cout << "b01gddep " << b01gddep << std::endl;
  this->a10dep = a10dep;
  std::cout << "a10dep " << a10dep << std::endl;
  this->b10indep = b10indep;
  std::cout << "b10indep " << b10indep << std::endl;
  this->b10outdep = b10outdep;
  std::cout << "b10outdep " << b10outdep << std::endl;
  this->b10absdep = b10absdep;
  std::cout << "b10absdep " << b10absdep << std::endl;
  this->b10gddep = b10gddep;
  std::cout << "b10gddep " << b10gddep << std::endl;
}

void Model_Windows::setWindowState(bool state) {
    this->state = state;
}

bool Model_Windows::getWindowState() const {
    return state;
}

void Model_Windows::setDurationOpen(int durationOpen) {
    this->durationOpen = durationOpen;
}

int Model_Windows::getDurationOpen() const {
    return durationOpen;
}

double Model_Windows::calculateDurationOpen(double outdoorTemperature) {
    return randomWeibull(exp(aop + bopout * outdoorTemperature), shapeop);
}

void Model_Windows::arrival(double indoorTemperature, double outdoorTemperature,
    double previousDuration, bool rain, int timeStepLengthInMinutes) {
    double Rain = (rain ? 1.f : 0.f);
    //  log(1/a)= 0.871
    if (!state) {
        float m = a01arr + b01inarr * indoorTemperature +
                  b01outarr * outdoorTemperature +
                  b01rnarr * Rain +
                  b01absprevarr *
                  ((previousDuration > 8.f * 60.f * 60.f) ? 1.f : 0.f);
        float prob01arr = exp(m) / (1.f + exp(m));
        double drand = randomDouble();

        if (drand < prob01arr) {
            durationOpen = calculateDurationOpen(outdoorTemperature);
            state = 1;
        } else {
            state = 0;
            durationOpen = 0.f;
        }
    } else {
        durationOpen = calculateDurationOpen(outdoorTemperature);
        if (durationOpen < timeStepLengthInMinutes) {
            state = 0;
            durationOpen = 0;
        } else {
            state = 1;
            durationOpen = durationOpen - timeStepLengthInMinutes;
        }
    }
}

void Model_Windows::intermediate(double indoorTemperature,
    double outdoorTemperature, double currentDuration, bool rain,
    int timeStepLengthInMinutes) {

    double Rain = (rain ? 1.f : 0.f);
    if (!state) {
        double m = a01int + b01inint * indoorTemperature +
                      b01outint * outdoorTemperature +
                      b01presint * currentDuration +
                      b01rnint * Rain;
        float prob01int = exp(m) / (1.f + exp(m));
        double drand = randomDouble();
        if (drand < prob01int) {
            durationOpen = calculateDurationOpen(outdoorTemperature);
            state = 1;
        } else {
            state = 0;
             durationOpen = 0.f;
        }
    } else {
        if (durationOpen < timeStepLengthInMinutes) {
            state = 0;
            durationOpen = 0.f;
        } else {
            state = 1;
            durationOpen = durationOpen - timeStepLengthInMinutes;
        }
    }
}

void Model_Windows::departure(double indoorTemperature,
    double dailyMeanTemperature, double futureDuration, double groundFloor ) {

    // double durationOpen;
    double drand = randomDouble();

    double durationLongerThanEightHours = 0.0;

    if (futureDuration >= 8.f * 60.f * 60.f) {
        durationLongerThanEightHours = 1.0;
    }
    if (!state) {
        double m = a01dep + b01outdep * dailyMeanTemperature +
                  b01absdep * durationLongerThanEightHours +
                  b01gddep * groundFloor;
        float prob01deplong = exp(m) / (1.f + exp(m));
        if (drand < prob01deplong) {
            state = 1;
        } else {
            state = 0;
            durationOpen = 0.f;
        }
    } else {
        double m = a10dep + b10indep * indoorTemperature +
                    b10outdep * dailyMeanTemperature +
                    b10absdep * durationLongerThanEightHours +
                    b10gddep * groundFloor;
        float prob10deplong = exp(m) / (1.f + exp(m));
        if (drand < prob10deplong) {
            state = 0;
            durationOpen = 0.f;
        } else {
            state = 1;
        }
    }
}
