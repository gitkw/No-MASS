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
      Log(){
        
      }

      template<typename T>
      Log &operator << (const T &t){
         buf << t;
         return * this;
      }

      void printLog(){
        std::cout << buf;
      }
      void error(){
        errorFlag = true;
      }
      bool getError(){
        return errorFlag;
      }

   private:
      static std::stringstream buf;
      static bool errorFlag;
};

#endif	/* LOG_H */
