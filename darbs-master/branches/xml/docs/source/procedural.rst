Procedural agent
===========================

Procedural agents are :doc:`agent` which are used to run functions written in C++ contained in shared libraries.  This allows more complex manipulation of data than the adding, removing and simple string manipulation allowed by the core of DARBS.  

Creating a library
------------------
The first stage in being able to add custom functionality to DARBS is to write a shared library.  In order to compile your library you need a compiler chain which is compatible with the chain which compiled DARBS.  See the :doc:`install` for further information about this.  To try it out, attempt to compile and run the procedural example.  If this works you can get on with writing the functionality you need.

The structure of a library for use with DARBS is very simple, consisting of any number of functions each taking a single string argument, and returning another string.  If we again take the procedural example from the examples/ directory the function (which simply calculates the square of the input) is::

    extern "C"
    std::string square(std::string arg) {
        std::istringstream in(arg);
        double num;
        in >> num;
        double sq = num * num;
        std::stringstream out;
        out << sq;
        return(out.str());
    }

Notice how is converted to a double, and back to a string, to allow numerical operations to be carried out, but to keep the string-based storage used in DARBS.  Also notice the extern declaration above the function; this declaration is required for every function which you wish to call using DARBS.

When you have written your library it can be comiled into a .so, .dylib or .dll using a CMakeFile.txt similar to the one contained in examples/procedural.  See the `CMake documentation <http://www.cmake.org/cmake/help/documentation.html>` for more information about how to use CMake on your platform.  When compiled the library needs to be copied somewhere within your operating system's linker path.

Using the library
-----------------
With the library compiled, your custom functions can then be called using the :doc:`agent's <agent>` run request.  The request takes attributes giving the name of the library and function, and a variable name to store the string returned by the function.  The argument to call the function with is supplied by a pattern within the request.   

For example the procedural agent using the function above is as follows::

    <agent type="procedural" fireable="true">
        <precondition type="not">
            <request type="present">
                <partition name="control">
                    <pattern>
                        Square is fired
                    </pattern>
                </partition>
            </request>
        </precondition>

        <action>
            <request type="run" library="libsquare.so" function="square" variable="result">
                <pattern>2.34</pattern>
            </request>

            <request type="report">
                <pattern>
                    The square of 2.34 is ~result
                </pattern>
            </request>

            <request type="add">
                <partition name="control">
                    <pattern>
                        Square is fired
                    </pattern>
                </partition>
            </request>
        </action>
    </agent>
