cmake_minimum_required (VERSION 2.6)
project (AgentClient)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules")

# The version number.
set (Client_VERSION_MAJOR 0)
set (Client_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)
file (GLOB_RECURSE pugixml ../pugixml/*)
set(source_files ${source_files} ${pugixml} ../common/src/TcpClient.cpp ../common/src/Logger.cpp ../common/src/Util.cpp)
include_directories(../common/src)
include_directories(../pugixml)

if (UNIX)
    find_package(LibDL REQUIRED) 
    find_package(Threads)
endif(UNIX)

if (WIN32)
    set(BOOST_ROOT "C:\\MinGW")
    set(BOOST_LIBRARYDIR "C:\\MinGW\\lib")
    set(BOOST_INCLUDEDIR "C:\\MinGW\\include\\boost")
    set(WINSOCK_LIBRARIES ws2_32 mswsock)
    add_definitions(-D_WIN32_WINNT=0x0501 -D_WIN32)
endif(WIN32)

set(Boost_USE_STATIC_LIBS ON)

find_package( Boost 1.38 REQUIRED signals system regex thread program_options filesystem)



include_directories(${Boost_INCLUDE_DIRS} ${LIBDL_INCLUDE_DIR})
include_directories(../)
add_executable(agentclient ${source_files})
target_link_libraries(agentclient ${WINSOCK_LIBRARIES} ${Boost_LIBRARIES} ${LIBDL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS agentclient DESTINATION bin)
