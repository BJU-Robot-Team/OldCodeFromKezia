#include <polysync_message.hpp>
#include <polysync_node.hpp>
#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>
#include <fstream>
#include <vsv_common.h>

using namespace std; 

int main()
{

    // local vars
    int                                 ret                 = DTC_RET( DTC_NONE );
 
    // generic sleep ticker
    int                                 do_sleep            = 0;
    unsigned int                        frame_ready         = 0;
    void                                *queue_element      = NULL;
	int									exit_signaled 		= 1;
    //SDL_Event                           sdl_event;
 
    // flags from command line arguments
    unsigned int                        stdout_enabled      = 0;
    unsigned int                        flip_enabled        = 0;
	//Video stream message queue.
	static GAsyncQueue              	*video_stream_queue = NULL;
    //ps_guid                             src_guid_arg        = 0;
    // create queue
    video_stream_queue = g_async_queue_new();

    while( 1 )
    {
		
		int frame_ready = 0;
		
		if (exit_signaled)
		{
			goto GRACEFUL_EXIT;
		}
		
		queue_element = g_async_queue_try_pop( video_stream_queue);
		if (queue_element)
		{ 
			do_sleep = 0;
			
			//type cast
			ps_video_stream_msg *msg = (ps_video_stream_msg*) queue_element;
			
			//processing the data
			
            if( (ret = process_video_stream( msg, &frame_ready ) ) != DTC_RET( DTC_NONE ) )
            {
            	goto GRACEFUL_EXIT_STMNT;
            } 
			//release instance
            if( (ret = psync_message_release_instance( &queue_element ) ) != DTC_RET( DTC_NONE ) )
            {
                psync_log_message( LOG_LEVEL_ERROR, "main -- psync_message_release_instance failed - error_code: %d", ret );
                goto GRACEFUL_EXIT_STMNT;
            }
 
            // deref
            msg = NULL;
            queue_element = NULL;
    	
    }
	GRACEFUL_EXIT_STMNT:
 
    exit_signaled = 1;
	
	// release
    if( vsv_data.is_init )
    {
        SDL_FreeSurface( vsv_data.surface );
        SDL_Quit();
    }
};
