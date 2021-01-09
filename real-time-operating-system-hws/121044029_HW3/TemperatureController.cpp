//
// Created by Gurol Cay on 6.11.2018.
//

#include <iostream>
#include "TemperatureController.h"

TemperatureController::TemperatureController(ISimulator *iSim) : iSim(iSim) {
    sched_param sch;
    int policy;
    isTempThreadTerminated = false;

    temperatureThread = new std::thread([&]() {
        temperatureThreadFunc();
    });

    pthread_getschedparam(temperatureThread->native_handle(), &policy, &sch);
    sch.sched_priority = 10;
//  sicakligi ve basinci dengede tutmaya calisiyorum, burada SCHED_FIFO secmemin nedeni
//  yapilan isin yarida kesilmemesi icin. cunku eger sicakligi arttirirken tam arttirmadan
//  hemen once bu thread'i durdurup basinci arttirir isem ve sonra bu thread'i tekrar
//  calistirir isem (isinan havanin basinci artar) basinci cok daha fazla arttirmis olurum
//  bu yuzden sicaklik arttirilacak ise bolunmeden yapilmalidir.
    if (pthread_setschedparam(temperatureThread->native_handle(), SCHED_FIFO, &sch)) {
        std::cout << "Failed to set temperature thread priority : " << std::strerror(errno) << '\n';
    }
}

TemperatureController::~TemperatureController() {
    isTempThreadTerminated = true;
    temperatureThread->join();
    delete temperatureThread;
    temperatureThread = nullptr;
}

void TemperatureController::temperatureThreadFunc() {
    std::cout << "TemperatureController::threadFunction starting" << std::endl;
    std::chrono::high_resolution_clock::time_point start, end;

    while (!isTempThreadTerminated) {
        start = std::chrono::high_resolution_clock::now();

        iSim->triggerADCTemperature();
        temperature = iSim->readADCTemperature();

        if (temperature < getC()) {
            iSim->switchHeater(true);
        } else if (temperature > getD()) {
            iSim->switchHeater(false);
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds time_span =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::this_thread::sleep_for(std::chrono::milliseconds((int) (timeInterval - time_span.count())));
    }
    std::cout << "TemperatureController::threadFunction exiting" << std::endl;
}

/**
 * Getter for temperature minimum bound
 * @return temperature minimum bound
 */
double TemperatureController::getC() const {
    return C;
}

double TemperatureController::getTemperature() const {
    return temperature;
}

/**
 * Getter for temperature maximum bound
 * @return temperature maximum bound
 */
double TemperatureController::getD() const {
    return D;
}
