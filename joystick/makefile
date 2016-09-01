main: main.o motor.o joystick.o
	g++ -Wall -W -Werror MotorController.o main.o joystick.o -o main

main.o: main.cpp
	g++ -c main.cpp

motor.o: MotorController.cpp
	g++ -c MotorController.cpp

joystick.o: joystick.cpp
	g++ -c joystick.cpp

