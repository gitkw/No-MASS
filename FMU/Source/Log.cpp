/*
 * File:   Log.cpp
 * Author: jake
 *
 * Created on September 13, 2013, 12:26 PM
 */


#include "Log.h"
// Define the static member, somewhere in an implementation file.
std::stringstream Log::buf;
bool Log::errorFlag = false;
