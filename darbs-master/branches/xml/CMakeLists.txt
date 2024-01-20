cmake_minimum_required (VERSION 2.6)
project (DARBS)

# The version number.
set (DARBS_VERSION_MAJOR 2)
set (DARBS_VERSION_MINOR 0)
set (DARBS_VERSION_PATCH 1)


set(BOOST_ROOT "C:/local/boost_1_67_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_67_0/stage/lib")
set(BOOST_INCLUDEDIR "C:/local/boost_1_67_0/include")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "x64")
endif() #--Sometimes, cmake may skip libraries expressed in this form "libboost_signals-mgw82-mt-d-x64-1_67.a"



add_subdirectory(server ${CMAKE_CURRENT_SOURCE_DIR}/server/build)
add_subdirectory(terminal ${CMAKE_CURRENT_SOURCE_DIR}/terminal/build)
add_subdirectory(ks ${CMAKE_CURRENT_SOURCE_DIR}/ks/build)
add_subdirectory(util ${CMAKE_CURRENT_SOURCE_DIR}/util/build)
add_subdirectory(docs)

# All install targets have to be here for CPack to work
install(DIRECTORY examples DESTINATION share/darbs
        PATTERN ".svn" EXCLUDE)

include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_NAME "DARBS")
set (CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${DARBS_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${DARBS_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${DARBS_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ framework for building multi-agent intelligent systems")
set(CPACK_PACKAGE_CONTACT "Joe Turner <joe@oampo.co.uk>")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_PACKAGE_EXECUTABLES "blackboard" "Blackboard Server" "agentclient" "Agent Client" "terminal" "Terminal")

if (UNIX)
# Not needed for static boost
#    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-signals-dev (>= 1.42.0), libboost-system-dev (>= 1.42.0), libboost-regex-dev (>=1.42.0), libboost-program-options-dev (>= 1.42.0), libboost-thread-dev (>=1.42.0), libboost-filesystem-dev (>= 1.42.0)")
    set(CPACK_GENERATOR "DEB;RPM")
    set(CPACK_SOURCE_GENERATOR "TBZ2")
endif(UNIX)

if (WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_SOURCE_GENERATOR "ZIP")
endif (WIN32)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

set(CPACK_SOURCE_IGNORE_FILES "build" "templates" "tests" ".svn")


include (CPack)
