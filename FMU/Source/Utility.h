// Copyright 2015 Jacob Chapman

#ifndef UTILITY_H_
#define UTILITY_H_

#include <random>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class Utility {
 public:
    template<typename T>
      using uTable = std::vector<std::vector<T>>;

    static double randomDouble(double min, double max);
    static int randomInt(int min, int max);
    static bool tossACoin();
    static void setSeed(int seed);
    static int cumulativeProbability(const std::vector<double> &v);
    static int cumulativeProbability(const double * v, const int len);
    static std::list<int> randomIntList(int number);
    static std::vector<std::string> splitCSV(const std::string & typeString);
    static int calculateNumberOfDays(int startDay, int startMonth,
                                              int endDay, int endMonth);
    template< typename T>
        static uTable<T> csvToTable(const std::string & filename, bool header) {
          std::string line;
          std::ifstream myfile(filename);
          Utility::uTable<T> t;
          if (myfile.is_open()) {
            while (getline(myfile, line)) {
              if (header) {
                header = false;
                continue;
              }
              std::vector<std::string> split = Utility::splitCSV(line);
              std::vector<T> items;
              for (std::string &item : split) {
                items.push_back(std::stod(item));
              }
              t.push_back(items);
            }
            myfile.close();
          }
          return t;
        }

    template< typename T>
        static uTable<T> csvToTable(const std::string & filename) {
          return csvToTable<T>(filename, false);
        }

 private:
    Utility();
    static std::random_device r;
    static std::mt19937_64 engine;
};

#endif  // UTILITY_H_
