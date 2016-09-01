#include <polysync_message.hpp>
#include <polysync_node.hpp>
#include <iostream>
#include <utility>
#include <cmath>
#include <fstream>

using namespace std; 

int main()
{
	ofstream outstream;
	outstream.open ("LidarPointList.txt");
    polysync::Node node( "Lidar" );
    node.registerListener( MSG_TYPE_LIDAR_POINT_STREAM );

    while( ! node.exitSignal() )
    {
		void* msg = node.queuePopSafe( true );
        if( msg != nullptr )
        {
			//ps_msg_type type = polysync::message::getType( msg );
            //polysync::message::LiDARPointStream( msg ).print();
			//polysync::message::LiDARPointStream::points();
			polysync::message::LiDARPointStream message = 	polysync::message::LiDARPointStream( msg );
			//double pointVector [3];
			//pointVector = message.pos();
			//pointVector = message.dataStreamVector();
			std::vector< ps_lidar_point > vector = message.dataStreamVector();
			for( auto point : vector )
    		{
				double* position = point.pos;
				double x = static_cast< double>( std::floor( position[ 0 ] ) );
    			double y = static_cast< double>( std::floor( position[ 1 ] ) );
   				double z = static_cast< double>( std::floor( position[ 2 ] ) );
				outstream << "x " << x << " y " << y << " z " << z << endl;
/*Debug code segment
				cout << "Debug stop. Enter 1 for next step. Enter anything else to kill program." << endl;
				int debug;
				cin >> debug;
				if (debug != 1)
					{	
						outstream.close ();
						exit(1);
					}
*/
			}
			
			//polysync::message::LiDARPointStream( msg.dataStreamVector() ).print();
        }

    	//Wait for a millisecond before repeat
    	usleep( 1000 );
    }
	outstream.close ();
    return 0;
};
