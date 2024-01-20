Installation Instructions
=========================

Binary Packages
---------------

Binaries packages for Windows and .deb and .rpm builds for Linux are available for download from http://darbs.svn.sourceforge.net/viewvc/darbs/branches/xml/release/.

Building From Source
--------------------

Dependencies
^^^^^^^^^^^^
DARBS is fairly light on dependencies.  In order to build it you need:

* An up-to-date compiler chain (DARBS is currently tested using GCC-4.4 and MinGW, but should compile with any recent version GCC, MSVC++ or MinGW)
* Boost C++ libraries - http://www.boost.org
* CMake - http://www.cmake.org/

For building on Windows the `nuwen MinGW distribution <http://nuwen.net/mingw.html>`_ provides a complete toolchain including a compiled version of Boost.

Optional Dependencies
^^^^^^^^^^^^^^^^^^^^^

* Sphinx - Used to build the documentation - http://sphinx.pocoo.org/ 

Getting DARBS
^^^^^^^^^^^^^
A tarball of the latest stable version of DARBS can be downloaded from http://darbs.svn.sourceforge.net/viewvc/darbs/branches/xml/release/.

To get the latest version DARBS from the svn repository, run the following command::

    svn co https://darbs.svn.sourceforge.net/svnroot/darbs/branches/xml darbs 

Building DARBS
^^^^^^^^^^^^^^
To create the files needed to build each DARBS run the following commands from the root folder::

    cd build
    cmake -G "OUTPUT_TYPE" ../

Where you replace OUTPUT_TYPE with the build system you are going to use.  For a full list of supported generators see http://www.cmake.org/cmake/help/cmake-2-8-docs.html#section_Generators.

To build all the components of DARBS you then follow the regular procedure for whatever build system you are using.  For example on a Unix system the complete series of commands would be::

    cd build
    cmake -G "Unix Makefiles" ../
    make
    make install

