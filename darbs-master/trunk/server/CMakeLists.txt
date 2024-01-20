cmake_minimum_required (VERSION 2.6)
project (Blackboard)

# The version number.
set (Blackboard_VERSION_MAJOR 0)
set (Blackboard_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)
set(source_files ${source_files} ../common/src/TcpServer.cpp ../common/src/TcpConnection.cpp)
include_directories(../common/src)

# Boost libs
find_package( Boost 1.38 REQUIRED signals system regex)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(blackboard ${source_files})
target_link_libraries(blackboard ${Boost_LIBRARIES})
