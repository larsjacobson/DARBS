enable_testing()

set(SERVER_SOURCE_DIR ../../server/src)
include_directories(${SERVER_SOURCE_DIR})
include_directories(../../pugixml)

file(GLOB_RECURSE pugixml ../../pugixml/*)

find_package( Boost 1.38 REQUIRED signals system regex)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(../../common/src)

add_executable(tests src/tests.cpp src/tests_Blackboard.cpp ${SERVER_SOURCE_DIR}/Blackboard.cpp ${pugixml} ../../common/src/Logger.cpp)
target_link_libraries(tests ${Boost_LIBRARIES})
add_test(ServerTests tests)





