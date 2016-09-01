/*
GPS Coordinate Calculation Program by James Reynolds and Sam Koenke

This program reads the current latitude and longitude from a connected GPS. Using the waypoint destination as its target (0,0), it outputs the meters in x and y coordinates to reach the target.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

//Input from GPS in degrees
double coordLat;
double coordLong;

//Destination in degrees
double wayptLat;
double wayptLong;

//Local Coordinates in degrees

double coordLatLocal;
double coordLongLocal;

//Local XY distance in meters

double xDist;
double yDist;

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


int main()
{

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
	ifstream infile("/dev/ttyACM0");
	
			
	//Get our destination
	cout << "Enter the waypoint latitude.\n";	
	cin >> wayptLat;
	cout << "Enter the waypoint longitude.\n";
	cin >> wayptLong;
 
	//Loop until we get a ctrl-c
	while(true)
	{
		getline(infile, line);
		if (infile != NULL)
		{
			if (line.size() > 50)
			{
				i = line.find("$GPGGA,");
				if (i != -1)
				{
					cout<<line<<endl;
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
					

					//Start the calculations
					

					//Assign our current location
					coordLat = latitude;
					coordLong = longitude;
					
					//Calculate conversion factors
					convLat = m1 + (m2*cos(2*refLat)) + (m3*cos(4*refLat)) + (m4*cos(6*refLat));
					convLong = (p1*cos(refLat)) + (p2*cos(3*refLat)) + (p3*cos(5*refLat));

					//Convert to local coordinates
					coordLatLocal = wayptLat - coordLat;
					coordLongLocal = wayptLong - coordLong;
					
					//Convert from degrees to meters
					xDist = coordLongLocal * convLong;
					yDist = coordLatLocal * convLat;
					cout << xDist << ", " << yDist <<endl;
					
				}
						
			}

		}
	if (infile == NULL)
	{
		cout<<"GPS offline"<<endl;
	}	
	}
	return 0;
}
