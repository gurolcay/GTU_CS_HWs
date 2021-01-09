//
// Created by Gurol Cay on 6.11.2018.
//

#ifndef HW2_SIMULATOR_H
#define HW2_SIMULATOR_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "ISimulator.h"

class Simulator : public ISimulator {
public:
    Simulator();

    ~Simulator();

    void triggerADCPressure() override;

    void triggerADCTemperature() override;

    double readADCPressure() override;

    double readADCTemperature() override;

    void writeDACPump(const double value) override;

    void switchHeater(bool isOn) override;

private:
    double temperature;
    double pressure;
};


#endif //HW2_SIMULATOR_H
