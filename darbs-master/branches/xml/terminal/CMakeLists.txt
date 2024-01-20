cmake_minimum_required (VERSION 2.6)
project (Terminal)

# The version number.
set (Terminal_VERSION_MAJOR 0)
set (Terminal_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)
set(source_files ${source_files} ../common/src/TcpClient.cpp ../common/src/Logger.cpp)
include_directories(../common/src)

if (WIN32)
    set(BOOST_ROOT "C:\\MinGW")
    set(BOOST_LIBRARYDIR "C:\\MinGW\\lib")
    set(BOOST_INCLUDEDIR "C:\\MinGW\\include\\boost")
    set(WINSOCK_LIBRARIES ws2_32 mswsock)
    add_definitions(-D_WIN32_WINNT=0x0501 -D_WIN32)
endif(WIN32)

set(Boost_USE_STATIC_LIBS ON)

find_package( Boost 1.38 REQUIRED signals system regex thread)

if (UNIX)
    find_package (Threads)
endif(UNIX)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(darbsterminal ${source_files})
target_link_libraries(darbsterminal ${WINSOCK_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS darbsterminal DESTINATION bin)
