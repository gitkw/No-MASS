// Copyright 2016 Jacob Chapman

#ifndef SIMULATIONTIME_H_
#define SIMULATIONTIME_H_

#include <vector>

/**
 * @brief Keeps track of time
 * @details Keeps track of time
 */
class SimulationTime {
 public:
    static void preprocess();
    static void trackTime();
    static void reset();

    static int stepCount;
    static int databaseIdStepCount;
    static int minute;
    static int databaseIdMinute;
    static int hour;
    static int databaseIdHour;
    static int day;
    static int databaseIdDay;
    static int month;
    static int databaseIdMonth;
    static int hourOfDay;
    static int databaseIdHourOfDay;
    static int minuteOfDay;
    static int databaseIdMinuteOfDay;


 private:
    static std::vector<int> monthCount;
    SimulationTime();
};

#endif  // SIMULATIONTIME_H_
