cmake_minimum_required (VERSION 2.6)
project (DARBS)

# The version number.
set (DARBS_VERSION_MAJOR 0)
set (DARBS_VERSION_MINOR 1)

set(BOOST_ROOT "C:/local/boost_1_67_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_67_0/stage/lib")
set(BOOST_INCLUDEDIR "C:/local/boost_1_67_0/include")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "x64")
endif() #--Sometimes, cmake may skip libraries expressed in this form "libboost_signals-mgw82-mt-d-x64-1_67.a"



add_subdirectory(server ${CMAKE_CURRENT_SOURCE_DIR}/server/build)
add_subdirectory(terminal ${CMAKE_CURRENT_SOURCE_DIR}/terminal/build)
add_subdirectory(ks ${CMAKE_CURRENT_SOURCE_DIR}/ks/build)
add_subdirectory(docs)

