find_program(HAVE_SPHINX sphinx-build)
if(HAVE_SPHINX)
    execute_process(COMMAND sphinx-build source build WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/build/index.html ${CMAKE_CURRENT_SOURCE_DIR}/build/genindex.html)
    install(DIRECTORY build/ DESTINATION share/darbs/docs
            PATTERN ".svn" EXCLUDE
            PATTERN ".buildinfo" EXCLUDE
            PATTERN ".doctrees" EXCLUDE)
endif(HAVE_SPHINX)
