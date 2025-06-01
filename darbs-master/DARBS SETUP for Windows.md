# DARBS SETUP for Windows

## Download Required Files

- [CMake 3.22.1](https://cmake.org/download/)
- [MinGW 18.0](https://nuwen.net/mingw.html) (includes GCC 11.2.0 & Boost 1.77.0)
- DARBS files (link to repository)

## Install CMake

1. Run installation file with admin rights.
2. Select **Add System PATH**.
3. Verify installation:
   ```bash
   cmake --version
   ```

## Install MinGW

1. Extract MinGW to a preferred location.
2. Add `..\MinGW\bin` to system PATH.
3. Verify installation:
   ```bash
   gcc --version
   ```

## Compile DARBS (`..\trunk`)

1. Open Command Prompt and navigate to `..\trunk\build` folder.
2. Reset **CMakeCache** in `..\trunk\build`.
3. Reset **CMakeLists** in `..\trunk\ks`.
4. Reset **CMakeLists** in `..\trunk\server`.
5. Manually redirect towards local cmake installation:
   - C compiler
   - CXX compiler
   - Make program
6. Run:
   ```bash
   cmake -G "MinGW Makefiles" ../
   ```
7. Optional: If you encounter errors like **"Unable to find the requested Boost libraries"**, edit `CMakeLists.txt` in `..\trunk` and add:
   ```cmake
   if (WIN32)
       set(BOOST_ROOT "e.g. C:/local/boost_1_67_0")
       set(BOOST_LIBRARYDIR "e.g. C:/local/boost_1_67_0/stage/lib")
       set(BOOST_INCLUDEDIR "e.g. C:/local/boost_1_67_0/include")
   endif(WIN32)
   ```

8. Optional: Add architecture if needed:
   ```cmake
   if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
       set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "x64")
   endif()
   ```

9. Optional: To avoid errors like **"undefined reference to `__imp_WSAStartup'"**, edit `CMakeLists.txt` in these folders: `\server`, `\terminal`, `\ks`. Add:
   ```cmake
   target_link_libraries(<target> ws2_32 mswsock)
   ```

## Compile dlfcn-win32 Library

> To resolve: **"fatal error: dlfcn.h: No such file or directory #include <dlfcn.h>"** during `ksclient` compilation on Windows

1. Extract dlfcn-win32 to a preferred location.
2. Build using MinGW. Open Command Prompt and navigate to `..\dlfcn-win32` folder.
3. Run:
   ```bash
   cmake -G "MinGW Makefiles"
   make
   ```
4. Change the name of `dlfcn-win32-config.cmake` (e.g., `dlfcn-win32-config2.cmake`) and save.
5. Edit contents of `dlfcn-win32-config.cmake` and comment out macros, leaving only:
   ```cmake
   set(dlfcn-win32_INCLUDEDIR "<dlfcn-win32 root folder>")
   if(NOT TARGET dlfcn-win32::dl)
       include(<dlfcn-win32 root folder>/dlfcn-win32-targets.cmake)
   endif()
   set(dlfcn-win32_LIBRARIES dlfcn-win32::dl)
   set(dlfcn-win32_INCLUDE_DIRS ${dlfcn-win32_INCLUDEDIR})
   ```

6. Edit `CMakeLists.txt` in `\ks` folder and add:
   ```cmake
   set (dlfcn-win32_DIR "<dlfcn-win32 root folder>")
   include_directories("<dlfcn-win32 root folder>")
   if (WIN32)
       find_package(dlfcn-win32 REQUIRED)
       set(CMAKE_DL_LIBS dlfcn-win32::dl)
   endif(WIN32)
   target_link_libraries(ksclient ${CMAKE_DL_LIBS})
   ```

7. Run:
   ```bash
   make
   ```
8. Copy `libdl.dll` from `<dlfcn-win32 root folder>` to `..\MinGW\bin`.

## Compile DARBS (`..\xml\branches`)

1. Follow the same steps as **Compile DARBS (`..\trunk`)**.
2. If you encounter the error:
   ```
   error: no match for 'operator=' (operand types are 'std::__cxx11::string' and 'boost::filesystem::path') name = path.filename();
   ```
   - Edit file: `..\ks\src\AbstractConditional.cpp`
   - Change line 56:
     ```cpp
     name = path.filename();
     ```
     to:
     ```cpp
     name = path.string();
     ```

## Running Examples

- Use the compiled executables in `..\xml\branches`.
