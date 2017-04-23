// Copyright 2016 Jacob Chapman

#ifndef MODEL_LIGHTS_H
#define	MODEL_LIGHTS_H

/**
 * @brief Models the lighting interactions
 * @details Models lighting intractions for an occupant adapted from
 * \n Reinhart, C. (2004). Lightswitch-2002: a model for manual and automated control of electric lighting and blinds. Solar Energy, 77(1), 15–28. Retrieved from http://www.sciencedirect.com/science/article/pii/S0038092X04000702
 */

class Model_Lights {
public:
    Model_Lights();

    bool arrival(bool state, double Lumint);
    bool intermediate(bool state, double Lumint);
    bool departure(bool state, double futureDuration);
private:
};

#endif	/* MODEL_LIGHTS_H */
