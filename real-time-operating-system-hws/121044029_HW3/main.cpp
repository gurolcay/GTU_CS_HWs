#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <sched.h>
#include "TemperatureController.h"
#include "ISimulator.h"
#include "Simulator.h"
#include "PressureController.h"
#include "OperatorConsole.h"

volatile bool terminating = false;

void signalHandler(int s) {
    std::cout << "signal caught" << std::endl;

    terminating = true;

}


int main() {

    struct sigaction sigIntHandler{};
    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    ISimulator *simulator = new Simulator();

    auto *temperatureController = new TemperatureController(simulator);
    auto *pressureController = new PressureController(simulator);
    OperatorConsole operatorConsole(pressureController, temperatureController);

    while (!terminating);

    return 0;
}