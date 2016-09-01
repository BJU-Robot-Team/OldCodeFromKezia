#include <MotorController.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <iostream>
#include <fstream>

using namespace std;

void InitializeMotorController(int port);

MotorController::MotorController()
{
	// Open controller
	m_portNum = open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK | O_NDELAY);
	InitializeMotorController(m_portNum);
}

MotorController::~MotorController()
{
	// Close controller
	cout << "Closing Motor Controller Port" << endl;
	close(m_portNum);
}

void MotorController::setFSpeed(int speed)
{
	// Convoluted limit check
	speed = speed < -127 ? -127 : speed > 127 ? 127 : speed;
	m_lSpeed = speed;

	return;
}

void MotorController::setTSpeed(int speed)
{
	// Convoluted limit check
	speed = speed < -127 ? -127 : speed > 127 ? 127 : speed;
	m_rSpeed = speed;

	return;
}


void MotorController::sendMotorCommand()
{
	char cmd[50];

	// Determine forwards or backwards
	char motorA_ID = m_lSpeed < 0 ? 'a' : 'A';
	char motorB_ID = m_rSpeed < 0 ? 'b' : 'B';

	// Build the command and throw it out the serial port
	sprintf(cmd, "!%c%02x\r!%c%02x\r", motorA_ID, abs(m_lSpeed), motorB_ID, abs(m_rSpeed));
	write( m_portNum, cmd, sizeof(cmd) );

	// Show what we sent out
	sprintf(cmd, "Fwd or Reverse speed: %02d\nTurning speed: %02d\n", m_rSpeed, m_rSpeed);
	cout << cmd;

	return;
}

// Initialize the motor contoller's serial port: 9600 Baud, 7 Bit, Even Parity
void InitializeMotorController(int portNum)
{
	cout << "Initializing Motor Controller\n";

	if ( portNum < 0 )
	{
		cout << "Motor controller failed to load\n";
	}

	struct termios tty;
	memset(&tty, 0, sizeof tty);

	if ( tcgetattr(portNum, &tty) != 0 )
	{
		cout << "TODO: Figure out what this error-1 actually is...\n";
	}

	// Much of the defined constants are listed in the following web address
	// http://pubs.opengroup.org/onlinepubs/007908775/xsh/termios.h.html

	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);

	tty.c_cflag	|= PARENB;		// Enable parity
	tty.c_cflag	&= ~PARODD;		// Clear odd parity forcing even
	tty.c_cflag	&= ~CSTOPB;		// Clear double stop bit flag forcing single stop bit
	tty.c_cflag	&= ~CSIZE;		// Clear character size flag
	tty.c_cflag	|= CS7;			// Set to seven bit character size
	tty.c_cflag	&= ~CRTSCTS;		// Disable flow control

	tty.c_lflag	= 0;			// Clears all local  mode options
	tty.c_oflag	= 0;			// Clears all output mode options

	tty.c_cc[VMIN]	= 0;			// Reading port doesn't block port
	tty.c_cc[VTIME]	= 5;			// Half second read timeout

	tty.c_cflag	|= CREAD | CLOCAL;			// Enable READ & ignore control lines
	tty.c_iflag	&= ~(IXON | IXOFF | IXANY);		// Disable software flow control
	tty.c_lflag	&= ~(ICANON | ECHO | ECHOE | ISIG);	// Make "raw"
	tty.c_oflag	&= ~OPOST;				// Make "raw"
	
	tcflush( portNum, TCIFLUSH );


	if ( tcsetattr(portNum, TCSANOW, &tty ) != 0 )
	{
		cout << "TODO: Figure out what this error-2 actually is...\n";
	}

	return;
}
