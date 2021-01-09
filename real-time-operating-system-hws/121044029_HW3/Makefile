all:
	g++ -std=c++11 PressureController.cpp OperatorConsole.cpp TemperatureController.cpp Simulator.cpp main.cpp -c
	g++ PressureController.o OperatorConsole.o TemperatureController.o Simulator.o main.o -lpthread -lrt -o main

clean:
	rm -rf *.o main