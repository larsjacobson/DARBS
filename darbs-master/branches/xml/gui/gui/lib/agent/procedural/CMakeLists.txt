cmake_minimum_required (VERSION 2.6)
project (ProceduralExample)

# The version number.
set (ProceduralExample_VERSION_MAJOR 0)
set (ProceduralExample_VERSION_MINOR 1)

file(GLOB_RECURSE source_files src/*)

add_library(square SHARED ${source_files})
