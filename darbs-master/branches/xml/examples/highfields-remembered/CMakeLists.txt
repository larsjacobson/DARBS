cmake_minimum_required (VERSION 2.6)
project (HighfieldsRememberedExample)

# The version number.
set (HighfieldsRememberedExample_VERSION_MAJOR 0)
set (HighfieldsRememberedExample_VERSION_MINOR 1)

if (WIN32)
    set(BOOST_ROOT "C:\\Program Files\\Boost-1.41.0")
    set(BOOST_LIBRARYDIR "C:\\Program Files\\Boost-1.41.0\\lib")
    set(BOOST_INCLUDEDIR "C:\\Program Files\\Boost-1.41.0\\include")
endif(WIN32)
find_package( Boost 1.38 REQUIRED)

file(GLOB_RECURSE source_files src/*)
include_directories(${Boost_INCLUDE_DIRS} ${LIBDL_INCLUDE_DIR})

add_library(stringmetrics SHARED ${source_files})

install(TARGETS stringmetrics DESTINATION lib)
