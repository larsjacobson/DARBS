cmake_minimum_required (VERSION 2.6)
project (Blackboard)

# The version number.
set (Blackboard_VERSION_MAJOR 0)
set (Blackboard_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)
file (GLOB_RECURSE pugixml ../pugixml/*)
set(source_files ${source_files} ${pugixml} ../common/src/TcpServer.cpp ../common/src/TcpConnection.cpp ../common/src/Logger.cpp ../common/src/Util.cpp)
include_directories(../common/src)
include_directories(../pugixml)

# Boost libs
if (WIN32)
    set(BOOST_ROOT "C:\\MinGW")
    set(BOOST_LIBRARYDIR "C:\\MinGW\\lib")
    set(BOOST_INCLUDEDIR "C:\\MinGW\\include\\boost")
    set(WINSOCK_LIBRARIES ws2_32 mswsock)
    add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

if (UNIX)
    find_package (Threads)
endif(UNIX)

set(Boost_USE_STATIC_LIBS ON)

find_package( Boost 1.40 REQUIRED signals system regex program_options)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(blackboard ${source_files})
target_link_libraries(blackboard ${WINSOCK_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS blackboard DESTINATION bin)
