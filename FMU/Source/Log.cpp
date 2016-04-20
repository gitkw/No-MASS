// Copyright 2016 Jacob Chapman

#include <fstream>
#include <iostream>
#include <string>
#include "Log.h"
// Define the static member, somewhere in an implementation file.
std::stringstream Log::buf;
bool Log::errorFlag = false;

Log::Log() {}

void Log::printLog() {
  if (errorFlag) {
    std::ofstream outfile;
    outfile.open("No-MASS.err", std::ios_base::app);
    outfile << "   *************"  << std::endl;
    outfile << "   ************* ===== ";
    outfile << "Final No-MASS (Detailed Occupancy) Error Summary =====";
    outfile << std::endl;
    std::string line;

    if (std::getline(Log::buf, line)) {
      outfile << "   **  Fatal  ** " << line  << std::endl;
    }
    while (std::getline(Log::buf, line)) {
      outfile << "   **   ~~~   ** " << line  << std::endl;
    }
    outfile << "   *************"  << std::endl;
    outfile << "   *************"  << std::endl;
    outfile.close();
  }
}

void Log::error() {
  errorFlag = true;
  Log::printLog();
}

void Log::reset() {
  errorFlag = false;
}

bool Log::getError() {
  return errorFlag;
}
