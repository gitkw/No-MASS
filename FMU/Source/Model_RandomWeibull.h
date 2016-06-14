// Copyright 2016 Jacob Chapman

#ifndef MODEL_RANDOMWEIBULL_H
#define	MODEL_RANDOMWEIBULL_H

class Model_RandomWeibull {
public:
    Model_RandomWeibull();

    static double randomWeibull(double scale, double shape);
    static double randomDouble();
    static double randomDouble(double min, double max);
    static double probability(double m);
};

#endif	/* MODEL_RANDOMWEIBULL_H */
