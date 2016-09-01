/* Waypoint Navigation Test Program by James Reynolds and Sam Koenke 12/5/15

Serial interfacing code referenced at http://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing

This program is intended to test out waypoint navigation without any other supporting programs. At the time of writing, Polysync is not working properly, so multitasking had to be implemented.

TODO

Add MRPT driving algorithm.

*/




//Data type and programming Libraries
#include <iostream>
#include <thread>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <tuple>

//Polysync Libraries
#include <polysync_message.hpp>
#include <polysync_node.hpp>

//Math Libraries
#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>

//Serial Port Libraries
#include <stdio.h>      // standard input / output functions
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
using namespace std;


//Global variables for the threads to write to and the main thread to read from
float latitude, longitude, heading;
float xDist, yDist;
float wayptLat, wayptLong;

//GPS Functions

//Configure the serial connection
void GPSSetup()
{
    
    int USB = open( "/dev/ttyACM1", O_RDWR| O_NOCTTY );
    
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
    cfsetospeed (&tty, (speed_t)9600);
    cfsetispeed (&tty, (speed_t)9600);
    
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
}

//Collect the data from the GPS
void GPSData()
{
    stringstream infile;
    string line;
    int i = -1;
    int x = 0;
    string utc_hr,utc_min,utc_sec;
    string lat_deg,lon_deg;
    string lat_min,lon_min;
    string lat_dir,lon_dir;
    double est_hr,edt_hr,est_min,est_sec;
    float lat_deg_val,lon_deg_val;
    float lat_min_val,lon_min_val;
		bool done = 0;
		int USB = open( "/dev/ttyACM1", O_RDWR| O_NOCTTY );
		while(!done)
		{
        int n = 0,
        spot = 0;
        char buf = '\0';
        /* Whole response*/
        char response[1024];
        memset(response, '\0', sizeof response);
        
        do {
            n = read( USB, &buf, 1 );
            sprintf( &response[spot], "%c", buf );
            spot += n;
        } while( buf != '\r' && n > 0);
		 if (n < 0) {
            std::cout << "GPS Error reading: " << strerror(errno) << std::endl;
        }

        infile.clear();
        infile<<response;
        getline(infile, line);
		//cout<<line<<endl;
        i = line.find("$GPGGA,");
        if (i != -1 && line.size() > 50)
        {
            /*//extract timestamp string
            utc_hr = line.substr(i+7,2);
            utc_min = line.substr(i+9,2);
            utc_sec = line.substr(i+11,5);
            est_hr = atof(utc_hr.c_str())-5;
            if (est_hr < 0)
                est_hr = est_hr+24;
            edt_hr = atof(utc_hr.c_str())-4;
            if (edt_hr < 0)
                edt_hr = edt_hr+24;
            est_min = atof(utc_min.c_str());
            est_sec = atof(utc_sec.c_str());
            cout<<"(UTC/GMT) "<<utc_hr<<":"<<utc_min<<":"<<utc_sec<< endl;
            cout<<"(Eastern Standard Time)"<<est_hr<<":"<<est_min<<":"<<est_sec<<endl;
            cout<<"(Eastern Daylite Time)"<<edt_hr<<":"<<est_min<<":"<<est_sec<<endl;
			*/
            //extract Latitude string
            lat_deg = line.substr(i+17,2);
            lat_min = line.substr(i+19,8);
            lat_dir = line.substr(i+28,1);
            lat_deg_val = atof(lat_deg.c_str());
            lat_min_val = atof(lat_min.c_str());
            latitude = lat_deg_val + (lat_min_val/60);
            if (lat_dir=="S")
                latitude = 0.0-latitude;
            //cout << "Latitude: " <<latitude<<endl;
            //extract Longitude string
            lon_deg = line.substr(i+30,3);
            lon_min = line.substr(i+33,8);
            lon_dir = line.substr(i+42,1);
            lon_deg_val = atof(lon_deg.c_str());
            lon_min_val = atof(lon_min.c_str());
            longitude = lon_deg_val + (lon_min_val/60);
            if (lon_dir=="W")
                longitude = 0.0-longitude;
            //cout << "Longitude: " <<longitude<<endl;
        }
		}
		close(USB);
}

//Compass functions

//Functions to split the compass strings up
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void CompassSetup()
{

    int USB = open( "/dev/ttyUSB1", O_RDWR| O_NOCTTY );

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
    cfsetospeed (&tty, (speed_t)115200);
    cfsetispeed (&tty, (speed_t)115200);
    
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
}

void CompassData()
{		
		while(TRUE){
		int USB1 = open( "/dev/ttyUSB1", O_RDWR| O_NOCTTY );
		int n1 = 0,
        spot1 = 0;
        char buf1 = '\0';
        /* Whole response*/
        char response[1024];

        memset(response, '\0', sizeof response);
        
        do {
            n1 = read( USB1, &buf1, 1 );
            sprintf( &response[spot1], "%c", buf1 );
            spot1 += n1;
        } while( buf1 != '\r' && n1 > 0);
		        if (n1 < 0) {
            std::cout << "Compass Error reading: " << strerror(errno) << std::endl;
        }

        //Split up the heading
        std::vector<std::string> split1 = split(response, 'P');
        std::string heading_val = split1[0];
        heading_val.erase(0,3);
        float headingdeg= stof(heading_val,NULL);
        //heading = (headingdeg/180)*3.1415926;
		heading = headingdeg;
        
        //Split up the pitch angle
        std::vector<std::string> split2 = split(split1[1], 'R');
        std::string pitch = split2[0];
        float pitch_val = stof(pitch,NULL);
        
        //Split up the roll angle
        
        std::vector<std::string> split3 = split(split2[1], 'T');
        std::string roll = split3[0];
        float roll_val = stof(roll,NULL);
        
        //Display the stuff
        //std::cout<<"Heading: "<< heading <<std::endl;
        //std::cout<<"Pitch: "<< pitch_val <<std::endl;
        //std::cout<<"Roll: "<< roll_val <<std::endl;
        
		n1 = 0,
        spot1 = 0;
        buf1 = '\0';
		close(USB1);
		}
}

void CoordCalc()
{
//Local Coordinates in degrees

double coordLatLocal;
double coordLongLocal;

//Latitude to meters conversion factor constants
const double m1 = 111132.96;
const double m2 = -559.82;
const double m3 = 1.175;
const double m4 = -0.0023;

//Longitutde to meters conversion factor constants

const double p1 = 111412.84;
const double p2 = -93.5;
const double p3 = 0.118;

//Reference latitudes in radians

const double gvilleLatRad = 0.608664378;
const double oaklandLatRad = 0.744779116;
double refLat = gvilleLatRad; //Change this to gvilleLatRad or oaklandLatRad depending on the location.

//Lat & Long to meters conversion factor

double convLat;
double convLong;

					//Calculate conversion factors
					convLat = m1 + (m2*cos(2*refLat)) + (m3*cos(4*refLat)) + (m4*cos(6*refLat));
					convLong = (p1*cos(refLat)) + (p2*cos(3*refLat)) + (p3*cos(5*refLat));
					while(TRUE)
					{
					//Convert to local coordinates
					coordLatLocal = wayptLat - latitude;
					coordLongLocal = wayptLong - longitude;
					
					//Convert from degrees to meters
					xDist = coordLongLocal * convLong;
					yDist = coordLatLocal * convLat;
					//cout << xDist << ", " << yDist <<endl;
					usleep(1000);
					}
}

void RelaySetup()
{

    int USB = open( "/dev/ttyACM2", O_RDWR| O_NOCTTY );
    
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
}


void RelayBlink()
{
while(TRUE)
    {
	int USB1 = open( "/dev/ttyACM2", O_RDWR| O_NOCTTY );
unsigned char cmd[] = "relay on 0\n\r";
int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
		close(USB1);
        usleep( 1000000 );
int USB2 = open( "/dev/ttyACM2", O_RDWR| O_NOCTTY );
unsigned char cmd2[] = "relay off 0\n\r";
int n_written2 = 0,
    spot2 = 0;

do {
    n_written2 = write( USB2, &cmd2[spot2], 1 );
    spot2 += n_written2;
} while (cmd2[spot2-1] != '\r' && n_written2 > 0);
		close(USB2);
		usleep( 1000000 );
        
    }
}

void MotorControllerSetup()
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
}

void turnLeft()
{

	int USB1 = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
	unsigned char cmd[] = "!a0A\r";
	int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
		close(USB1);

}

void turnRight()
{
	int USB1 = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
	unsigned char cmd[] = "!A0A\r";
	int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
		close(USB1);

}

void driveStraight()
{

	int USB1 = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
	unsigned char cmd[] = "!b30\r";
	int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
		close(USB1);

}

void driveFStop()
{

	int USB1 = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
	unsigned char cmd[] = "!b00\r";
	int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
		close(USB1);

}

void driveTStop()
{

	int USB1 = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
	unsigned char cmd[] = "!a00\r";
	int n_written = 0,
    spot = 0;

do {
    n_written = write( USB1, &cmd[spot], 1 );
    spot += n_written;
} while (cmd[spot-1] != '\r' && n_written > 0);
		close(USB1);

}

int main()
{	

	//Get Waypoint Destination
	cout << "Enter the waypoint latitude.\n";	
	cin >> wayptLat;
	cout << "Enter the waypoint longitude.\n";
	cin >> wayptLong;		
	
	//Initialize the Serial Connections
	GPSSetup();
	CompassSetup();
	RelaySetup();
	MotorControllerSetup();
	
	//Create the threads
	thread startGPS (GPSData);
	thread startCompass (CompassData);
	thread startBlink (RelayBlink);
	thread startCoordCalc (CoordCalc);
	
	//Separate the threads and let them run
	startGPS.detach();
	startCompass.detach();
	startBlink.detach();
	startCoordCalc.detach();

	//Loop until we get a ctrl-c
    while(1)
    {
		//cout<<"Latitude & Longitude: "<<latitude<<", "<<longitude<<endl;
		usleep(10000000);
		cout<<"Heading: "<<heading<<endl;
		cout<< "X-Distance: "<< xDist <<endl;
		cout<< "Y-Distnace: "<< yDist <<endl;
		usleep(10000000);
		
		//Navigate to waypoint
		//Go to x coordinate
		if(xDist>0)
		{
			while(heading > 91 || heading < 89)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading > 90 || heading <= 270)
				{
				cout<<"Turning Left 1"<<endl;
					turnLeft();
				}
				else
				{
				cout<<"Turning Right 1"<<endl;
					turnRight();
				}
			}
			driveTStop();
			while(xDist > 1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 1"<<endl;
			driveStraight();
			}
			driveFStop();
		}
		else
		{
			while(heading > 271 || heading < 269)
			{
						cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading < 90 || heading >= 270)
				{
				cout<<"Turning Left 2"<<endl;
					turnLeft();
				}
				else
				{
				cout<<"Turning Right 2"<<endl;
					turnRight();
				}
			}
			driveTStop();
			while(xDist < -1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 2"<<endl;
			driveStraight();
			}
			driveFStop();
		}
		
		//Go to y coordinate
		if(yDist>0)
		{
			while(heading < 359 && heading > 1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading <= 180)
				{
				cout<<"Turning Left 3"<<endl;
					turnLeft();
				usleep(10000);
				}
				else
				{
				cout<<"Turning Right 3"<<endl;
					turnRight();
				usleep(10000);
				}
			}
			driveTStop();
			while(yDist > 1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 3"<<endl;
			driveStraight();
			}
			driveFStop();
		}
		else
		{
			while(heading < 179 || heading > 181)
			{
						cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading <= 180)
				{
				cout<<"Turning Right 4"<<endl;
					turnRight();
				}
				else
				{
				cout<<"Turning Left 4"<<endl;
					turnLeft();
				}
			}
			driveTStop();
			while(yDist < -1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 4"<<endl;
			driveStraight();
			}
			driveFStop();
		}
	if(xDist>0)
		{
			while(heading > 91 || heading < 89)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading > 90 || heading <= 270)
				{
				cout<<"Turning Left 1"<<endl;
					turnLeft();
				}
				else
				{
				cout<<"Turning Right 1"<<endl;
					turnRight();
				}
			}
			driveTStop();
			while(xDist > 1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 1"<<endl;
			driveStraight();
			}
			driveFStop();
		}
		else
		{
			while(heading > 271 || heading < 269)
			{
						cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading < 90 || heading >= 270)
				{
				cout<<"Turning Left 2"<<endl;
					turnLeft();
				}
				else
				{
				cout<<"Turning Right 2"<<endl;
					turnRight();
				}
			}
			driveTStop();
			while(xDist < -1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 2"<<endl;
			driveStraight();
			}
			driveFStop();
		}
		
		//Go to y coordinate
		if(yDist>0)
		{
			while(heading < 359 && heading > 1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading <= 180)
				{
				cout<<"Turning Left 3"<<endl;
					turnLeft();
				usleep(10000);
				}
				else
				{
				cout<<"Turning Right 3"<<endl;
					turnRight();
				usleep(10000);
				}
			}
			driveTStop();
			while(yDist > 1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 3"<<endl;
			driveStraight();
			}
			driveFStop();
			return 0;
		}
		else
		{
			while(heading < 179 || heading > 181)
			{
						cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
				if (heading <= 180)
				{
				cout<<"Turning Right 4"<<endl;
					turnRight();
				}
				else
				{
				cout<<"Turning Left 4"<<endl;
					turnLeft();
				}
			}
			driveTStop();
			while(yDist < -1)
			{
			cout<<"Heading: "<<heading<<endl;
			cout<< "X-Distance: "<< xDist <<endl;
			cout<< "Y-Distnace: "<< yDist <<endl;
			cout<<"Drive Straight 4"<<endl;
			driveStraight();
			}
			driveFStop();
			return 0;
		}
	}
}


