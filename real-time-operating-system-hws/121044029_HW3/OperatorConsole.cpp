//
// Created by Gurol Cay on 6.11.2018.
//

#include <iomanip>
#include "OperatorConsole.h"


void OperatorConsole::operatorThreadFunc() {
    std::cout << "OperatorConsole::threadFunction starting" << std::endl;
    std::chrono::high_resolution_clock::time_point start, end, check_point;
    std::chrono::high_resolution_clock::time_point pressLastUpdateTime;
    std::chrono::high_resolution_clock::time_point tempLastUpdateTime;

    long int pressTimeUpdateDiff = 0;
    long int tempTimeUpdateDiff = 0;

    bool pressMissesDeadlines = false;
    bool tempMissesDeadlines = false;


    std::cout << std::fixed << std::setprecision(2);

    pressLastUpdateTime = std::chrono::high_resolution_clock::now();
    tempLastUpdateTime = std::chrono::high_resolution_clock::now();

    while (!isOperatorThreadTerminated) {
        start = std::chrono::high_resolution_clock::now();
        pressMissesDeadlines = true;
        tempMissesDeadlines = true;

        for (int i = 0; i < 49; ++i) {
            check_point = std::chrono::high_resolution_clock::now();

            pressTimeUpdateDiff = std::chrono::duration_cast<std::chrono::milliseconds>(
                    check_point - pressLastUpdateTime).count();
            pressTimeUpdateDiff = pressTimeUpdateDiff - pressTimeInterval;
            if (pressTimeUpdateDiff > 0 && pressMissesDeadlines) {
                std::cout << "WARNING: PressureController deadline miss by " << pressTimeUpdateDiff << " milliseconds"
                          << std::endl;
                pressMissesDeadlines = false;
            }


            tempTimeUpdateDiff = std::chrono::duration_cast<std::chrono::milliseconds>(
                    check_point - tempLastUpdateTime).count();
            if (tempTimeUpdateDiff > 0 && tempMissesDeadlines) {
                std::cout << "WARNING: TemperatureController deadline miss by " << tempTimeUpdateDiff
                          << " milliseconds" << std::endl;

                tempMissesDeadlines = false;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }


        std::cout << "Temperature : " << temperatureController->getTemperature()
                  << " Pressure : " << pressureController->getPressure() << std::endl;


        end = std::chrono::high_resolution_clock::now();


        std::chrono::milliseconds time_span =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // temp ve press in ne kadar surdugu

        std::this_thread::sleep_for(
                std::chrono::milliseconds((int) (timeInterval - time_span.count()))); // geri kalan bekleme surem


    }


    std::cout << "OperatorConsole::threadFunction exiting" << std::endl;
}

OperatorConsole::~OperatorConsole() {
    isOperatorThreadTerminated = true;
    operatorThread->join();
    delete operatorThread;
    operatorThread = nullptr;
}

OperatorConsole::OperatorConsole(PressureController *pressureController, TemperatureController *temperatureController)
        : pressureController(pressureController), temperatureController(temperatureController) {
    isOperatorThreadTerminated = false;

    operatorThread = new std::thread([&]() {
        operatorThreadFunc();
    });
}
