cmake_minimum_required (VERSION 2.6)
project (Terminal)

# The version number.
set (Terminal_VERSION_MAJOR 0)
set (Terminal_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)
set(source_files ${source_files} ../common/src/TcpClient.cpp)
include_directories(../common/src)

find_package( Boost 1.38 REQUIRED signals system regex thread)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(darbsterminal ${source_files})
target_link_libraries(darbsterminal ${Boost_LIBRARIES})
