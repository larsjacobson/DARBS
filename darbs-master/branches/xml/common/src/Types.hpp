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
 * @file Types.hpp
 * @brief Header file containing typedefs used throughout DARBS
 */
#ifndef __TYPES_HPP
#define __TYPES_HPP

// Global includes
#include <string>
#include <vector>
#include <boost/regex.hpp>

// Local includes

// Defines

// Type definitions
typedef std::map<std::string, std::string> VariableMap;
typedef std::map<std::string, std::vector<std::string> > MultiVariableMap;

typedef boost::match_results<std::string::const_iterator> MatchResults;

// Global variables

// Class definitions


#endif
