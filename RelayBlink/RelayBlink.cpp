/* Relay Toggle Program by James Reynolds 12/3/15

Supporting serial interfacing code referenced from http://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing

This program is designed to toggle the light on the robot to signal that it is autonomous mode. */

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
    
    
    int USB = open( "/dev/ttyACM0", O_RDWR| O_NOCTTY );
    
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);
    
    /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 ) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }
    
    /* Save old tty parameters */
    tty_old = tty;
    
    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)2400);
    cfsetispeed (&tty, (speed_t)2400);
    
    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    
    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
    
    /* Make raw */
    cfmakeraw(&tty);
    
    /* Flush Port, then applies attributes */
    tcflush( USB, TCIFLUSH );
    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }
    close(USB);
    //Loop until we get a ctrl-c
    while( ! node.exitSignal() )
    {
	int USB1 = open( "/dev/ttyACM1", O_RDWR| O_NOCTTY );
unsigned char cmd[] = "relay on 0\n\r";
int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
        

		close(USB1);
        usleep( 1000000 );
		cout <<cmd<<endl;
int USB2 = open( "/dev/ttyACM1", O_RDWR| O_NOCTTY );
unsigned char cmd2[] = "relay off 0\n\r";
int n_written2 = 0,
    spot2 = 0;

do {
    n_written2 = write( USB2, &cmd2[spot2], 1 );
    spot2 += n_written2;
} while (cmd2[spot2-1] != '\r' && n_written2 > 0);

cout <<cmd2<<endl;
		close(USB2);
		usleep( 1000000 );
        
    }
    return 0;
   	exit(EXIT_SUCCESS);
}


