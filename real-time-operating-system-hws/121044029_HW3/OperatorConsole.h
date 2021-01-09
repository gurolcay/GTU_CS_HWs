//
// Created by Gurol Cay on 6.11.2018.
//

#ifndef HW2_OPERATORCONSOLE_H
#define HW2_OPERATORCONSOLE_H


#include <thread>
#include <cstdlib>
#include "PressureController.h"
#include "TemperatureController.h"

class OperatorConsole {
public:

    OperatorConsole(PressureController *pressureController, TemperatureController *temperatureController);

    virtual ~OperatorConsole();

    void operatorThreadFunc();

private:
    const int timeInterval = 500; // milliseconds
    bool isOperatorThreadTerminated;


    std::thread *operatorThread;

    PressureController *pressureController;
    TemperatureController *temperatureController;

    const int pressTimeInterval = 100; // milliseconds
    const int tempTimeInterval = 100; // milliseconds


};


#endif //HW2_OPERATORCONSOLE_H
