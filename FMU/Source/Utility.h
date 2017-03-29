// Copyright 2015 Jacob Chapman

#ifndef UTILITY_H_
#define UTILITY_H_

#include <random>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
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
    static std::vector<int> randomIntVect(int number);
    static void splitCSV(const std::string & typeString, std::vector<std::string> * types);
    static int calculateNumberOfDays(int startDay, int startMonth,
                                              int endDay, int endMonth);
    template< typename T>
      static uTable<T> csvToTable(const std::string & filename, bool header) {
        std::string line;
        std::ifstream myfile(filename);
        Utility::uTable<T> t;
        if (myfile.is_open()) {
          if(header) getline(myfile, line);
          while (getline(myfile, line)) {
            std::vector<std::string> split;
            Utility::splitCSV(line, &split);
            std::vector<T> items;
            for (std::string &item : split) {
              double v = NAN;
              if(item != ""){
                v = std::stod(item);
              }
              items.push_back(v);
            }
            t.push_back(items);
          }
          myfile.close();
        }
        return t;
      }


    static std::vector<std::string> csvToTableHead(const std::string & filename);
    static std::vector<double> csvToDouble(const std::string & s);
    static std::vector<int> csvToInt(const std::string & s);
    template< typename T>
        static uTable<T> csvToTable(const std::string & filename) {
          return csvToTable<T>(filename, false);
        }
    static std::mt19937_64 engine;
 private:
    Utility();
    static std::random_device r;

};

#endif  // UTILITY_H_
