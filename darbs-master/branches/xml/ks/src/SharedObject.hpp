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
 * @file SharedObject.hpp
 * @brief Header file a dynamically linked shared object
 */

#ifndef __SHARED_OBJECT_HPP
#define __SHARED_OBJECT_HPP

// Global includes
#include <string>
#include <boost/extension/shared_library.hpp>

// Local includes

// Namespace

// Defines

// Type definitions

// Global variables

/**
 * @brief A dynamically linked library
 */
class SharedObject {
public:
    // construction
    SharedObject(std::string libraryName);

    // destruction
    ~SharedObject();
    
    // public access
    std::string call(std::string function, std::string arg);

protected:

private:
    boost::extensions::shared_library *library;
};


#endif
