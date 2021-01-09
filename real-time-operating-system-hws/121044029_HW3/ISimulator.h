//
// Created by Gurol Cay on 6.11.2018.
//

#ifndef HW2_ISIMULATOR_H
#define HW2_ISIMULATOR_H
class ISimulator {
public:
    virtual void triggerADCPressure() = 0;
    virtual void triggerADCTemperature() = 0;
    virtual double readADCPressure() = 0;
    virtual double readADCTemperature() = 0;
    virtual void writeDACPump(double value) = 0;
    virtual void switchHeater(bool is_open) = 0;
};
#endif //HW2_ISIMULATOR_H
