/*
 * File:   Log.h
 * Author: jake
 *
 * Created on September 13, 2013, 12:26 PM
 */

#ifndef LOG_H
#define	LOG_H

#include <sstream>
#include <iostream>
#define LOG Log()

// An example with a string stream.
class Log
{
   public:
      Log();

      template<typename T>
      Log &operator << (const T &t){
         if(!errorFlag){
           buf << t;
         }
         return * this;
      }

      void printLog();
      void error();
      void reset();
      bool getError();

   private:
      static std::stringstream buf;
      static bool errorFlag;
};

#endif	/* LOG_H */
