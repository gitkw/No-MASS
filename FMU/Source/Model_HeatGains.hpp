// Copyright 2016 Jacob Chapman

#ifndef HEATGAIN_H
#define HEATGAIN_H

 /**
  * @brief Models the occupant heat gains and PMV
  * @details Models the occupant heat gains and PMV adapted from
  * \n ISO. (2005). ISO 7730: Ergonomics of the thermal environment Analytical determination and interpretation of thermal comfort using calculation of the PMV and PPD indices and local thermal comfort criteria. Management (Vol. 3). https://doi.org/10.1016/j.soildyn.2004.11.005
  */

class Model_HeatGains {
public:
    Model_HeatGains();

    void calculate(
        double metabolicRate,
        double reativeHumidity,
        double meanRadiantTemperature,
        double externalWork,
        double ta,
        double clo,
        double airVelocity);
    double getAllHeatGains();
    double getSweatEvaporation() const;
    double getDryRespiration() const;
    double getLatentRespirationHeatGains() const;
    double getConvectiveHeatGains() const;
    double getRadiantHeatGains() const;
    double getPmv() const;
    double getPpd() const;

private:
    void aHCTCLcalc(
        double ta,
        double icl,
        double airVelocityAndBodyMovement,
        double surfaceAreaOfClothing,
        double metabolicRate,
        double externalWork,
        double actualMeanRadiantTemperature);

    double computePaCIBSEGuideC(double actualMeanRadiantTemperature, double reativeHumidity) const;

    double ppd;
    double pmv;
    double radiantHeatGains;
    double convectiveHeatGains;
    double latentRespirationHeatGains;
    double dryRespiration;
    double sweatEvaporation;
    double vapourDiffusion;
    int maxI;
    double hc;
    double tcl;
    double xn;

};

#endif  /* HEATGAIN_H */
