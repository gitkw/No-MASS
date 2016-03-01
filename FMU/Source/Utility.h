// Copyright 2015 Jacob Chapman

#ifndef UTILITY_H
#define	UTILITY_H

#include <random>
#include <list>

class Utility
{
public:
    static double randomDouble(double min, double max);
    static int randomInt(int min, int max);
    static bool tossACoin();
    static void setSeed(int seed);
    static std::list<int> randomIntList(int number, int min, int max);
    static std::vector<std::string> splitCSV(std::string typeString);
private:
    Utility();
    static std::random_device r;
    static std::default_random_engine engine;

};

#endif	/* UTILITY_H */
