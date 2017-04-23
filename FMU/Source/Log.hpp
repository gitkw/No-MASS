// Copyright 2017 Jacob Chapman

#ifndef LOG_H
#define	LOG_H

#include <sstream>
#include <iostream>

#define LOG Log()

 /**
  * @brief Logs the error messages
  * @details Logs the error messages for later writing to file
  */

class Log {
 public:
    Log();

    template<typename T>
    Log &operator << (const T &t){
       if(!errorFlag){
         buf << t;
         std::cout << t;
       }
       return * this;
    }

    static void printLog();
    void error();
    void reset();
    bool getError();

 private:
    static std::stringstream buf;
    static bool errorFlag;
};

#endif	/* LOG_H */
