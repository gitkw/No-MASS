// Copyright 2015 Jacob Chapman

#include <ctime>
#include <list>
#include <vector>
#include <random>
#include <string>

#include "Utility.h"

std::random_device Utility::r;
std::default_random_engine Utility::engine(r());

Utility::Utility() {
}

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

std::list<int> Utility::randomIntList(int number, int min, int max) {
    std::vector<int> numbers;
    std::list<int> randNumbers;
    for (int i =0; i < number; i++) {
        numbers.push_back(i);
    }

    while (!numbers.empty()) {
        int i = randomInt(0, numbers.size() -1);
        randNumbers.push_back(numbers[i]);
        numbers.erase(numbers.begin() +i);
    }
    return randNumbers;
}

std::vector<std::string> Utility::splitCSV(std::string typeString) {
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
