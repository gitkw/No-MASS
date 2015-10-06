/*
 * File:   Model_Activity.h
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#ifndef MODEL_ACTIVITY_H
#define	MODEL_ACTIVITY_H

#include <vector>
#include <SimulationConfig.h>

class Model_Activity {
public:
    Model_Activity();
    std::vector<double> getAgentActivities(const int agentID);

private:
    void parseConfiguration(const std::string filename);
    std::vector<double> disaggregate(const int agentID) const;
    std::vector<double> multinominal(const int agentID);
    std::string getSeason(const int month) const;
    std::string getDay(const int day) const;

    std::map<int, std::map<std::string, std::vector<double>>> dictionary;

};

#endif	/* MODEL_ACTIVITY_H */
