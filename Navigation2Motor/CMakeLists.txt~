project( Nav2Motor )

cmake_minimum_required( VERSION 2.8 )

FIND_PACKAGE( MRPT REQUIRED base)

include( /usr/local/polysync/build_res.cmake )

include_directories( ${PSYNC_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} Nav2Motor.cpp )

target_link_libraries( ${PROJECT_NAME} ${PSYNC_LIBS} )
target_link_libraries( ${PROJECT_NAME} ${MRPT_LIBS} )
