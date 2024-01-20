cmake_minimum_required (VERSION 2.6)
project (DarbsUtilLibrary)

# The version number.
set (DarbsUtilLibrary_VERSION_MAJOR 0)
set (DarbsUtilLibrary_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)

find_package( Boost 1.38)

include_directories(${Boost_INCLUDE_DIRS})

add_library(darbsutil SHARED ${source_files})

set(dest "lib")

# Install to bin if in Win32, so LoadLibraryEX will automatically find it
if (WIN32)
    set(dest "bin")
endif (WIN32)
    

install(TARGETS darbsutil DESTINATION ${dest})
