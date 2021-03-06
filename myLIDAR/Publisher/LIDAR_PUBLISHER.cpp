#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <polysync_message.hpp>
#include <polysync_node.hpp>
using namespace std;
 

int main()
{

	ifstream infile("/dev/ttyACM1");
    //Initialize and connect to PolySync.
    polysync::Node node( "KeziaLidar" );
 
    //Loop until we get a ctrl-c
    while( ! node.exitSignal() )
    {
        //Create a message
        polysync::message::LiDARPointStream pointStream;
       
        //Set the length of the stream buffer
        pointStream.setStreamLength( 10 );
        
/*  
        for( auto i = 0.0; i < 10.0; ++i )
        {
            //Add a point to the stream buffer, we'll use the value of i to increment the x position
            pointStream.streamPushBack
            (
                { LAYER_NONE, ECHO_NONE, POINT_NONE, 0.0, { i, i+2, i+4 } }
            );
        }
*/
       
        //Publish the message to PolySync
        polysync::message::publish( pointStream );
 
        //Wait for a second before we repeat
        sleep( 1 );
    }
    return 0;
}
