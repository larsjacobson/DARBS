Installation Instructions
=========================

DARBS is currently only available from the subversion repository, and needs to be compiled manually.

Dependencies
------------
DARBS is fairly light on dependencies.  In order to build it you need:

* A fairly up-to-date compiler chain (DARBS is currently tested using GCC-4.4, but should compile with any recent version GCC, MSVC++ or MinGW)
* Boost C++ libraries - http://www.boost.org
* CMake - http://www.cmake.org/

Optional Dependencies
---------------------

* Sphinx - Used to build the documentation - http://sphinx.pocoo.org/ 

Getting DARBS
-------------
To get the latest version DARBS from the svn repository, run the following command::

    svn co https://darbs.svn.sourceforge.net/svnroot/darbs/trunk darbs 

Building DARBS
--------------
To create the files needed to build each DARBS run the following commands from the root folder::

    cd build
    cmake -G "OUTPUT_TYPE" ../

Where you replace OUTPUT_TYPE with the build system you are going to use.  For a full list of supported generators see http://www.cmake.org/cmake/help/cmake-2-8-docs.html#section_Generators.

To build all the components of DARBS you then follow the regular procedure for whatever build system you are using.  For example on a unix system the complete series of commands would be::

    cd build
    cmake -G "Unix Makefiles" ../
    make
    make install

