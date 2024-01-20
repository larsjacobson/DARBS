// $Id$

/**
 * Copyright (C) 2010 Adrian Hopgood et al.
 *
 * Licensed under the Academic Free License version 3.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 */

/**
 * @file SharedObject.cpp
 */

// Global includes
#include <boost/function.hpp>
#include <iostream>

// Local includes
#include "SharedObject.hpp"
#include "Logger.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions

/**
 * @brief Constructor
 *
 * @param libraryName The name of the library to load
 */
SharedObject::SharedObject(std::string libraryName) {
    library = new boost::extensions::shared_library(libraryName, true);
    library->open();

    if(!library->is_open()) {
        // Library not found - try .so extension
        delete library;
        library = new boost::extensions::shared_library(libraryName + ".so",
                                                        true);
        library->open();
    }

    if (!library->is_open()) {
        // Library not found - try .dll extension
        delete library;
        library = new boost::extensions::shared_library(libraryName + ".dll",
                                                        true);
        library->open();
    }

    if (!library->is_open()) {
        LOG("Library " << libraryName << " could not be opened", ERROR);
    }
}

/**
 * @brief Destructor
 */
SharedObject::~SharedObject() {
    delete library;
}

/**
 * @brief Call a function in the library
 *
 * @param functionName The name of the function
 * @param arg An argument to pass to the function
 * @return The return value of the function, or "" if an error occured
 */
std::string SharedObject::call(std::string functionName, std::string arg) {
	std::string returnValue;
	
    boost::function<std::string(std::string)> function(library->get<std::string, std::string>(functionName));


    // check for error
    if(!function) {
        LOG("Error calling function " << functionName, ERROR);
        return("");
    }
    returnValue = function(arg);	
    return returnValue;
}

