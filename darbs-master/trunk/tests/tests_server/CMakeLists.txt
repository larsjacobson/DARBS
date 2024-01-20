
enable_testing()
set(SERVER_SOURCE_DIR ../../../server/src)
include_directories(${SERVER_SOURCE_DIR})

add_executable(tests src/tests.cpp src/tests_LnTokenizer.cpp ${SERVER_SOURCE_DIR}/LnTokenizer.cpp)
add_test(ServerTests tests)





