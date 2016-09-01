project( coordcalc )

cmake_minimum_required( VERSION 2.8 )

include( /usr/local/polysync/build_res.cmake )

include_directories( ${PSYNC_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} coordcalc.cpp )

target_link_libraries( ${PROJECT_NAME} ${PSYNC_LIBS} )
