//
// Created by Gurol Cay on 6.11.2018.
//

#ifndef HW2_PRESSURECONTROLLER_H
#define HW2_PRESSURECONTROLLER_H


#include <thread>
#include <iostream>
#include "ISimulator.h"


class PressureController {


public:
    explicit PressureController(ISimulator *iSim);

    virtual ~PressureController();

    void pressureThreadFunction();

    double getPressure() const;


private:
    const int timeInterval = 100;       // milliseconds
    const double maxPressure = 0.9;
    double pressure;
    const double presA = 21.0;
    const double presB = 12.0;
    bool isPressureThreadTerminated;

    ISimulator *iSim;
    std::thread *pressureThread;
};


#endif //HW2_PRESSURECONTROLLER_H
