#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <polysync_message.hpp>
#include <polysync_node.hpp>
#include <stdio.h>      // standard input / output functions
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
using namespace std;

int main()
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
	float latitude,longitude;

	int USB = open( "/dev/ttyACM0", O_RDWR| O_NOCTTY );

	//Initialize and connect to PolySync.
	polysync::Node node( "Compass" );

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

	//Loop until we get a ctrl-c
	while( ! node.exitSignal() )
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
    std::cout << "Error reading: " << strerror(errno) << std::endl;
}
else if (n == 0) {
    std::cout << "Read nothing!" << std::endl;
}
else {
    std::cout << "Response: " << response << std::endl;
}
infile.clear();
infile<<response;
getline(infile, line);
i = line.find("$GPGGA,");
if (i != -1)
				{
					//cout<<line<<endl;
					//extract timestamp string
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
					//extract Latitude string
					lat_deg = line.substr(i+17,2);
					lat_min = line.substr(i+19,8);
					lat_dir = line.substr(i+28,1);
					lat_deg_val = atof(lat_deg.c_str());
					lat_min_val = atof(lat_min.c_str());
					latitude = lat_deg_val + (lat_min_val/60);
					if (lat_dir=="S")
						latitude = 0.0-latitude;
					cout << "Latitude: " <<latitude<<endl;
					//extract Longitude string
					lon_deg = line.substr(i+30,3);
					lon_min = line.substr(i+33,8);
					lon_dir = line.substr(i+42,1);
					lon_deg_val = atof(lon_deg.c_str());
					lon_min_val = atof(lon_min.c_str());
					longitude = lon_deg_val + (lon_min_val/60);
					if (lon_dir=="W")
						longitude = 0.0-longitude;
					cout << "Longitude: " <<longitude<<endl;
					

					//Create a message
					polysync::message::PlatformMotion gps_message;
			
					//Update message
					gps_message.setLatitude(latitude);
					gps_message.setLongitude(longitude);

					//Publish the message to PolySync
					polysync::message::publish( gps_message );
			 
					//Wait for a millisecond before repeat
					usleep( 1000 );
				}
}
	return 0;
   	exit(EXIT_SUCCESS);
}


