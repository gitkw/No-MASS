// Copyright 2016 Jacob Chapman

#ifndef MODEL_LIGHTS_H
#define	MODEL_LIGHTS_H

class Model_Lights {
public:
    Model_Lights();

    bool arrival(bool state, double Lumint);
    bool intermediate(bool state, double Lumint);
    bool departure(bool state, double futureDuration);
private:
};

#endif	/* MODEL_LIGHTS_H */
