project( NavTest )

cmake_minimum_required( VERSION 2.8 )

FIND_PACKAGE( MRPT REQUIRED base)
FIND_PACKAGE( MRPT REQUIRED nav)

include( /usr/local/polysync/build_res.cmake )

include_directories( ${PSYNC_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} NavTest.cpp )

target_link_libraries( ${PROJECT_NAME} ${PSYNC_LIBS} )
target_link_libraries( ${PROJECT_NAME} ${MRPT_LIBS} )
set(CMAKE_BUILD_TYPE Release)
