#include <polysync_message.hpp>
#include <polysync_node.hpp>
#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>
#include <vector>
#include <fstream>


using namespace std; 

int main()
{
	ofstream outstream;
	outstream.open ("LidarPointList.txt");
	vector<double> LidarVector;
	//int counter = 0;
	bool hasVector = false;
	double radius; //variable for the magnitude of the distance to an object point
	double theta; //variable for the angle to that object point. note that the angle is specially defined according to MRPT input
	double psuedometer_value; //the value of an object point in MRPT psuedometers
    polysync::Node node( "Lidar" );
    node.registerListener( MSG_TYPE_LIDAR_POINT_STREAM );

    while( ! node.exitSignal() )
    {
		void* msg = node.queuePopSafe( true );
        if( msg != nullptr )
        {
			
			polysync::message::LiDARPointStream message = 	polysync::message::LiDARPointStream( msg );
			//double pointVector [3]
			std::vector< ps_lidar_point > vector = message.dataStreamVector();
			for( auto point : vector )
    		{
				double* position = point.pos;
				double x = static_cast< double>( ( position[ 0 ] ) );
    			double y = static_cast< double>( ( position[ 1 ] ) );
   				double z = static_cast< double>( ( position[ 2 ] ) );

				radius = x*x + y*y; //simple polar calcuation of r

				if (y == 0) //if to determine theta in by the MRPT definition
					{
						theta = 0;
					}
				else if (y < 0)
					{
						theta = ((-1) * (M_PI/2)) - atan( x/y );
					}
				else
					{
						theta = (M_PI/2) - atan ( x/y);
					}
			
				psuedometer_value = sqrt( pow(radius,2) + pow(radius * theta,2) ); //The object point in MRPT psuedometer format

				LidarVector.push_back (psuedometer_value); // Add object point to LiDAR vector for MRPT

				outstream << std::setprecision(3) << x << ","<< std::setprecision(3) << y << endl;
				hasVector = true;
			}
			
        }

		//Daniel's notes:
		//This appears to be the point in the code where the sweep is done.
		//So for a master program, this code will be a function call that returns a C++ vector filled with pseudometers for the MRPT.
		//Rather than sleeping below, the program will simply run through the end and be done until the next function call.
		//This way, the master program can simply call for LiDAR points a sweep at a time at whatever timing it needs them.

		if (hasVector == true)
				{
					break;
				}	

    	//Wait for a millisecond before repeat
    	usleep( 1000 );	

    }

	outstream.close ();
    return 0;
};
