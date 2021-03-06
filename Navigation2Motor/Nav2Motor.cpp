/*Navigation to Motor Test Program by James Reynolds and Sam Koenke with Daniel Marinelli contributing.

This program is designed to convert the MRPT output to an integer output for the Roboteq motor controller.

*/

#include <mrpt/base.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

/*Declare Variables*/

//Maximum Velocity Magnitude (m/s)
const double magMax = 1.56;

//Desired Velocity (input from MRPT)
double mag;

//Magnitude input to the motor controller (serial output)
int imcMag;

//Angle input to the motor controller (serial output)
int imcAngle;

//Maximum motor controller input value (serial output)
const int maxImc = 100;

//Velocity per integer value of input
double vStep;

//Current heading (degrees from north, input from compass in degrees)
double ang1;
double ang1Rad;

//Route course (desired direction, input from MRPT)
double ang2;

//Actual turn angle (calculated from ang1 and ang2)
double ang3;

//Calculated number of radians per integer output
double aStep;

int main()
{

	/*Get input for testing without MRPT and compass*/
	cout << "Enter MRPT desired velocity.\n";
	cin >> mag;
	cout << "Enter MRPT desired angle.\n";
	cin >> ang2;
	cout << "Enter compass heading.\n";
	cin >> ang1;

	/*Calculate Intermediates*/

	vStep = (magMax / maxImc);

	//Convert from degrees to radians
	if (ang1 <= 180){
	ang1Rad = ((ang1 * M_PI)/180);
	} else
	{
	ang1Rad = (((ang1 * M_PI)/180)-(2 * M_PI));
	}

	//Calculate the actual turn angle.
	ang3 = (-1 * ang1Rad) + ang2;

	//Make sure it's between -Pi and Pi
	if (ang3 > M_PI)
	{
	ang3 = ang3 - (2 * M_PI);
	} else if (ang3 < (-1* M_PI))
	{
	ang3 = ang3 + (2 * M_PI);
	}

	/*Calculate output velocity (Channel 1?)*/
	imcMag = int(mag / vStep);

	/*Calculate the angle step*/
	aStep = (M_PI / (127 - imcMag));

	/*Calculate ouput angle integer (Channel 2?)*/
	imcAngle = int(ang3 / aStep);

	/*Display output for testing*/
	cout << "Velocity: " << imcMag << endl;
	cout << "Angle: " << imcAngle << endl;
}




