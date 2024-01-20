cmake_minimum_required (VERSION 2.6)
project (KSClient)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules")

# The version number.
set (Client_VERSION_MAJOR 0)
set (Client_VERSION_MINOR 1)

set(BOOST_ROOT "C:/local/boost_1_67_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_67_0/stage/lib")
set(BOOST_INCLUDEDIR "C:/local/boost_1_67_0/include")

file(GLOB_RECURSE source_files src/*)
set(source_files ${source_files} ../common/src/TcpClient.cpp)
include_directories(../common/src)

if (UNIX)
    find_package(LibDL REQUIRED)
endif(UNIX)

find_package( Boost 1.38 REQUIRED signals system regex thread)

include_directories(${Boost_INCLUDE_DIRS} ${LIBDL_INCLUDE_DIR})
include_directories(../)
add_executable(ksclient ${source_files})
target_link_libraries(ksclient ${Boost_LIBRARIES} ${LIBDL_LIBRARY})



set (dlfcn-win32_DIR "C:\Users\genna\OneDrive\Desktop\JOB\DARBSenv\dlfcn-win32-master")
include_directories("C:\Users\genna\OneDrive\Desktop\JOB\DARBSenv\dlfcn-win32-master")
if (WIN32)
find_package(dlfcn-win32 REQUIRED)
set(CMAKE_DL_LIBS dlfcn-win32::dl)
endif(WIN32)
target_link_libraries(ksclient ${CMAKE_DL_LIBS})
