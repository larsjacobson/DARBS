find_program(HAVE_SPHINX sphinx-build)
if(HAVE_SPHINX)
    execute_process(COMMAND sphinx-build source build WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif(HAVE_SPHINX)
