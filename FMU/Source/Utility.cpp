// Copyright 2015 Jacob Chapman

#include <ctime>
#include <list>
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Utility.h"

std::random_device Utility::r;
std::mt19937_64 Utility::engine(r());

Utility::Utility() {}

void Utility::setSeed(int seed) {
    Utility::engine.seed(seed);
    Utility::randomDouble(0, 1);
}

double Utility::randomDouble(double min, double max) {
    std::uniform_real_distribution<double> uniform_dist(min, max);
    return uniform_dist(engine);
}

bool Utility::tossACoin() {
    return randomDouble(0.0, 1.0) < 0.5;
}

int Utility::randomInt(int min, int max) {
    std::uniform_int_distribution<int> uniform_dist(min, max);
    return uniform_dist(engine);
}

std::vector<int> Utility::randomIntVect(int number) {
    std::vector<int> numbers;
    for (int i =0; i < number; i++) {
        numbers.push_back(i);
    }
    std::random_shuffle(numbers.begin(), numbers.end());
    return numbers;
}

std::vector<std::string> Utility::splitCSV(const std::string & typeString) {
    std::vector<std::string> types;
    const char *str = typeString.c_str();
    char c = ',';
    do {
        const char *begin = str;
        while (*str != c && *str) {
            str++;
        }
        types.push_back(std::string(begin, str));
    } while (0 != *str++);
    return types;
}

int Utility::calculateNumberOfDays(const int startDay,
                                          const int startMonth,
                                          const int endDay,
                                          const int endMonth) {
    static const int daysInMonth[] =
                              {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = (startDay -1);
    for (int i = startMonth -1; i < endMonth -1; i++) {
        days += daysInMonth[i];
    }
    days += endDay;
    return days;
}

int Utility::cumulativeProbability(const double * v, const int len) {
  return Utility::cumulativeProbability(std::vector<double>(v, v + len));
}

int Utility::cumulativeProbability(const std::vector<double> &p) {
  int activity = -1;
  double sum = 0;
  double drand = Utility::randomDouble(0.0, 1.0);
  for (unsigned int i = 0; i < p.size(); i++) {
    sum += p[i];
    if (sum >= drand) {
        activity = i;
        break;
    }
  }
  return activity;
}
