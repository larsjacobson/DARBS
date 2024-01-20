/////////////////////////////////////////////////////////////////////////
//
// File: LnSharedObjectLoader.hpp
//
//////////////////////////////////////////////////////////////////////////
//
// $Id: LnSharedObjectLoader.hpp,v 1.1 2001/09/06 14:22:27 kcpw2 Exp $
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis: 
//
// Definition file for LnSharedObjectLoader class
// 
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2000/06/22: Lars Nolle: initial implementation
//
//////////////////////////////////////////////////////////////////////////
#ifndef __LN_SHARED_OBJECT_LOADER_HPP
#define __LN_SHARED_OBJECT_LOADER_HPP

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <stdarg.h>
#include <boost/extension/shared_library.hpp>

///////////////////////////// local includes /////////////////////////////


///////////////////////////// defines ////////////////////////////////////


///////////////////////////// global variables ///////////////////////////

///////////////////////////// class definitions //////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// LnSharedObjectLoader
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis:  
//
// LnSharedObjectLoader can be used to load shared objects dynamically at
// run-time. It also provides an interface to symbols (functions) from
// a loaded object.These functions must have a return value and need to
// have one of the following argument lists:
//
//    void call( string name, int&    retval, int argc, void* argv);   
//    void call( string name, float&  retval, int argc, void* argv);
//    void call( string name, double& retval, int argc, void* argv);
//    void call( string name, char*   retval, int argc, void* argv);
//    void call( string name, string  retval, int argc, void* argv);
//
// The functions may return a result using retval. If one of the functions
// is called, it is important to cast the argument vector argv to type
// (void*), and to cast it back to whatever type is needed in the 
// function.
//
// It is also important, that the shared library uses C-style linkage
// convention (like Fortran and Assambler). If the library contains
// C++ code, it has to be declared 'extern "C"'!
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2000/06/22: Lars Nolle: initial implementation
//
//////////////////////////////////////////////////////////////////////////
class  LnSharedObjectLoader
{
public:
    // constuctors
    LnSharedObjectLoader();
    LnSharedObjectLoader( std::string libName);    
    LnSharedObjectLoader( std::string libName, int flag); 
    LnSharedObjectLoader( std::string libName, int flag, std::string defaultDir);

    // destructor
    ~LnSharedObjectLoader();

    // load shared library
    void link( std::string libName );
    void link( std::string libName, int flag );

    // unload shared library
    void unlink();

    // set default directory for shared libraries
    void setDefaultDir( std::string directory );

    // call functions from library
    void call( std::string name, int&    retval, int argc, void* argv);   
    void call( std::string name, float&  retval, int argc, void* argv);
    void call( std::string name, double& retval, int argc, void* argv);
    void call( std::string name, char*   retval, int argc, void* argv);
    void call( std::string name, std::string& retval, int argc, void* argv);
    void call( std::string name, std::string& retval, std::string arg);
		std::string call( std::string name, int argc, void* argv);


protected:


private:

    // data members
    boost::extensions::shared_library*  m_handle;        // handle to shared library
    std::string m_defaultDir;    // default directory for shared libraries
};

#endif 
