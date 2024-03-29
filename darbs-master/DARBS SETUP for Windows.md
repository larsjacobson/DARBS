DARBS SETUP for Windows

--Download Required Files
a.	https://cmake.org/download/ (CMake 3.22.1)
b.	https://nuwen.net/mingw.html (MinGW 18.0, includes GCC 11.2.0 & Boost 1.77.0)
e.	DARBS files (link to repository)
--Install CMake
a.	Run installation file with admin rights
b.	Select "Add System PATH"
c.	run> (verify installation)
            cmake --version
--Install MinGW
a.	Extract MinGW to a preferred location
b.	Add ..\MinGW\bin to system PATH
c.	run> (verify installation)
            gcc --version
--Compile DARBS (..\trunk)
a.	Open command prompt and navigate to ..\trunk\build folder
b.  Reset "CMakeCache" in (..\trunk\build) 
c.  Reset "CMakeLists" in (..\trunk\ks)
d.  Reset "CMakeLists" in (..\trunk\server)
e. Manually redirect towards local cmake installation. 1) //C compiler, 2) //CXX compiler, 3) //make program
e.	run> 
            cmake -G "MinGW Makefiles" ../
f.	Optional - You may need to edit CMakelists.txt in ..\trunk if you encounter errors such as "Unable to find the requested Boost libraries" and add the following:
g.	
            if (WIN32)
            set(BOOST_ROOT "e.g. C:/local/boost_1_67_0")
            set(BOOST_LIBRARYDIR "e.g. C:/local/boost_1_67_0/stage/lib")
            set(BOOST_INCLUDEDIR "e.g. C:/local/boost_1_67_0/include")endif(WIN32)
h.	
            if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "x64")
            endif() #--Sometimes, cmake may skip libraries expressed in this form "libboost_signals-mgw82-mt-d-x64-1_67.a"
d.	Optional - You may need to edit CMakelists.txt in these folders \server, \terminal, \ks. Add this line: 
            target_link_libraries(<target> ws2_32 mswsock) #--to avoid errors like "undefined reference to `__imp_WSAStartup'" etc
--Compile dlfcn-win32 library (#--"fatal error: dlfcn.h: No such file or directory #include <dlfcn.h>" during ksclient compilation on windows)
a.	Extract dlfcn-win32 to a preferred location
b.	Build using MinGW. Open command prompt and navigate to ..\dlfcn-win32 folder
c.	run> cmake -G "MinGW Makefiles"
d.	run> 
            make
e.	Change the name of dlfcn-win32-config.cmake to e.g. dlfcn-win32-config2.cmake and save. 
f.	Edit contents of dlfcn-win32-config.cmake and save. Comment the macros leaving only the following:
			set(dlfcn-win32_INCLUDEDIR "<dlfcn-win32 root folder>")
			if(NOT TARGET dlfcn-win32::dl)
  				include(<dlfcn-win32 root folder>/dlfcn-win32-targets.cmake)
			endif()
			set(dlfcn-win32_LIBRARIES dlfcn-win32::dl)
			set(dlfcn-win32_INCLUDE_DIRS ${dlfcn-win32_INCLUDEDIR})

g.	Edit CMakelists.txt in \ks folder. Add this lines:
            set (dlfcn-win32_DIR "<dlfcn-win32 root folder>")
            include_directories(" <dlfcn-win32 root folder>")
            if (WIN32)
            find_package(dlfcn-win32 REQUIRED)
            set(CMAKE_DL_LIBS dlfcn-win32::dl)
            endif(WIN32)
            target_link_libraries(ksclient ${CMAKE_DL_LIBS})
h.	run> 
            make
i.	copy libdl.dll from <dlfcn-win32 root folder> to ..\MinGW\bin
--Compile DARBS (..\xml\branches)
a.	Do same as item 5.
b.	If you encounter error " error: no match for 'operator=' (operand types are 'std::__cxx11::string' {aka 'std::__cxx11::basic_string<char>'} and 'boost::filesystem::path') name = path.filename();"
i.	Edit file: ..\ks\src\AbstractConditional.cpp
ii.	Change line 56: name = path.filename() to:
             name = path.string()

--To run examples, run using compiled exes in ..\xml\branches