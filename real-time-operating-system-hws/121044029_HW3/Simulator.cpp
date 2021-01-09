//
// Created by Gurol Cay on 6.11.2018.
//



#include "Simulator.h"

Simulator::Simulator() {
    temperature = 0;
    pressure = 0;
}


void Simulator::triggerADCPressure() {
//    std::cout << "Pressure ADC triggered!" << std::endl;
}

void Simulator::triggerADCTemperature() {
//    std::cout << "Temperature ADC triggered!" << std::endl;
}

double Simulator::readADCPressure() {
    srand(time(NULL));
    return (rand() % 15) / 10.0;
}

double Simulator::readADCTemperature() {
    srand(time(NULL));
    return rand() % 40 + 10.0;
}

void Simulator::writeDACPump(double value) {
    pressure = value / 10.0;
//    std::cout << "Pump is on, writing pressure: " << pressure << std::endl;
}

void Simulator::switchHeater(bool isOn) {
    if (isOn) {
//        std::cout << "Heater is on" << std::endl;
        temperature += 0.2;
    } else {
//        std::cout << "Heater is off" << std::endl;
        temperature -= 0.2;
    }
}

Simulator::~Simulator() {

}
