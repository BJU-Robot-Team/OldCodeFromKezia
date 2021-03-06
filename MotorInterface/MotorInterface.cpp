/* Motor Controller Interface Program by James Reynolds 12/3/15

Supporting serial interfacing code referenced from http://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing

This program sends commands to the motor controller */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <stdio.h>      // standard input / output functions
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
using namespace std;


int main()
{
    
    
    int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
    
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);
    
    /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 ) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }
    
    /* Save old tty parameters */
    tty_old = tty;
    
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
    
    /* Flush Port, then applies attributes */
    tcflush( USB, TCIFLUSH );
    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }
    close(USB);
    //Loop until we get a ctrl-c
    while(1)
    {
	//string inputString;
	//cin >> inputString;
	int USB1 = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
	//char inputArray[4];
//strcpy(inputArray, inputString.c_str());
//unsigned char* cmd = (unsigned char*)inputArray;
	unsigned char cmd[] = "!b50\r";
int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
        

		close(USB1);
		cout <<cmd<<endl;
		usleep(10000000);
        
    }
    return 0;
   	exit(EXIT_SUCCESS);
}


