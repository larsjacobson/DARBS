enable_testing()

set(KS_SOURCE_DIR ../../ks/src)
include_directories(${KS_SOURCE_DIR})

add_executable(tests src/tests.cpp)
add_test(KSTests tests)





