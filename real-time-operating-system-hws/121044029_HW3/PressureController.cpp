//
// Created by Gurol Cay on 6.11.2018.
//

#include "PressureController.h"

PressureController::PressureController(ISimulator *iSim) : iSim(iSim) {
    sched_param sch;
    int policy;
    isPressureThreadTerminated = false;
    pressureThread = nullptr;

    pressureThread = new std::thread([&]() {
        pressureThreadFunction();
    });

    pthread_getschedparam(pressureThread->native_handle(), &policy, &sch);
    sch.sched_priority = 10;
//  sicakligi ve basinci dengede tutmaya calisiyorum, burada SCHED_FIFO secmemin nedeni
//  yapilan isin yarida kesilmemesi icin. cunku eger sicakligi arttirirken tam arttirmadan
//  hemen once bu thread'i durdurup basinci arttirir isem ve sonra bu thread'i tekrar
//  calistirir isem (isinan havanin basinci artar) basinci cok daha fazla arttirmis olurum
//  bu yuzden sicaklik arttirilacak ise bolunmeden yapilmalidir.
    if (pthread_setschedparam(pressureThread->native_handle(), SCHED_FIFO, &sch)) {
        std::cout << "Failed to set pressure thread priority : " << std::strerror(errno) << '\n';
    }


}

PressureController::~PressureController() {
    isPressureThreadTerminated = true;
    pressureThread->join();
    delete pressureThread;
    pressureThread = nullptr;
}

void PressureController::pressureThreadFunction() {
    std::cout << "PressureController::threadFunction starting" << std::endl;
    std::chrono::high_resolution_clock::time_point start, end;

    while (!isPressureThreadTerminated) {
        start = std::chrono::high_resolution_clock::now();
        iSim->triggerADCPressure();
        pressure = iSim->readADCPressure();
        if (pressure < maxPressure) {
            iSim->writeDACPump(presA - presB * pressure);
        } else {
            iSim->writeDACPump(0);
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds time_span =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::this_thread::sleep_for(std::chrono::milliseconds((int) (timeInterval - time_span.count())));
    }
    std::cout << "PressureController::threadFunction exiting" << std::endl;
}

double PressureController::getPressure() const {
    return pressure;
}