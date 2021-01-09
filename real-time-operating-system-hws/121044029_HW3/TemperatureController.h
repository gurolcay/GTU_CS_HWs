//
// Created by Gurol Cay on 6.11.2018.
//

#ifndef HW2_TEMPERATURECONTROLLER_H
#define HW2_TEMPERATURECONTROLLER_H

#include <thread>
#include <sched.h>
#include "ISimulator.h"

class TemperatureController {
public:

    explicit TemperatureController(ISimulator *sim);

    virtual ~TemperatureController();

    double getC() const;
    double getD() const;

    void temperatureThreadFunc();

    double getTemperature() const;

private:
    const int timeInterval = 333; // milliseconds
    ISimulator *iSim;
    double temperature;

    const double C = 27.0; // temperature minimum bound
    const double D = 35.0; // temperature maximum bound

    std::thread *temperatureThread;
    bool isTempThreadTerminated;

};


#endif //HW2_TEMPERATURECONTROLLER_H
