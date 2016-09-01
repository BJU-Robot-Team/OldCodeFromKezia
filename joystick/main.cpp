#include "MotorController.h"
#include "joystick.h"
#include <stdio.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <iostream>
#include <fstream>

//This is a modification of an old program for the motor controller.
//This code is stable, but the variables and functions for the motor are mislabeled.
//Forward motion gets a negative number. This program needs to be cleaned up.
//James Reynolds 10/2015


using namespace std;


int F_SPEED = 0;
int T_SPEED = 0;
int output = 0;

int main()
{
	// Open file descriptor for motor controller
	MotorController *motor = new MotorController();
  	// Create an instance of Joystick
  	Joystick joystick("/dev/input/js0");

	bool quit = false;

	while (!quit)
	{

  	// Ensure that it was found and that we can use it
  	if (!joystick.isFound())
  	{
    printf("Joystick not found.\n");
    quit = true;
  	}

	// Attempt to sample an event from the joystick
    JoystickEvent event;
	
    if (joystick.sample(&event))
    {
	if (event.isButton())
      {
        printf("Button %u is %s\n",
          event.number,
          event.value == 0 ? "up" : "down");

		//Press the B button on the Xbox controller to stop the robot.
		if (event.number == 1 && event.value == 1)
		{
		T_SPEED = 0;
		F_SPEED = 0;
		motor->setTSpeed(T_SPEED);
		motor->setFSpeed(F_SPEED);
		motor->sendMotorCommand();
		}

      }
      else if (event.isAxis())
      {
        printf("Axis %u is at position %d\n", event.number, event.value);
		if (event.number == 0)
		{
			//Turning control on the left joystick.
			if (event.value < -5000)
			{
				output = event.value;
				output = output * -1;
				output = pow(output, 0.3);
				output = output * -1;
				F_SPEED = output;
				motor->setFSpeed(F_SPEED);
			}
			else if (event.value < 5000)
			{
				output = 0;
				F_SPEED = output;
				motor->setFSpeed(F_SPEED);
			}
			else
			{
				output = event.value;
				output = pow(output, 0.3);
				F_SPEED = output;
				motor->setFSpeed(F_SPEED);
			}
			// Assign motor speeds
			motor->sendMotorCommand();
		}
		if (event.number == 1)
		{
			//Forward control on the left joystick
			if (event.value < -3000)
			{
				output = event.value;
				output = output * -1;
				output = pow(output, 0.35);
				output = output * -1;
				T_SPEED = output;
				motor->setTSpeed(T_SPEED);
			}
			else if (event.value < 3000)
			{
				output = 0;
				T_SPEED = output;
				motor->setTSpeed(T_SPEED);
			}
			else
			{
				output = event.value;
				output = pow(output, 0.35);
				T_SPEED = output;
				motor->setTSpeed(T_SPEED);
			}
			
	// Assign motor speeds
	motor->sendMotorCommand();
		}
		//Reverse control with the right trigger.
		if (event.number == 2)
		{
		output = event.value;
		output = output + 32767;
		output = pow(output, 0.4368);
		F_SPEED = output;
		motor->setTSpeed(F_SPEED);
		// Assign motor speeds
		motor->sendMotorCommand();
		}	
		//Forward control with the left trigger.
		if (event.number == 5)
		{
		output = event.value;
		output = output + 32767;
		output = pow(output, 0.4368);
		output = output * -1;
		F_SPEED = output;
		motor->setTSpeed(F_SPEED);
		// Assign motor speeds
		motor->sendMotorCommand();
		}
      }

    }
	

	
	}
	return 0;
}

