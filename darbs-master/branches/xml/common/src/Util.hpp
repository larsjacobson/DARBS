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
 * @file Util.hpp 
 * @brief Utility functions
 */

#ifndef __UTIL_HPP_
#define __UTIL_HPP

// Global includes 
#include <string>

// Local includes 
#include "pugixml.hpp"

// Namespaces
using namespace pugi;

// Defines 

// Global variables

// Class definitions

// Functions

std::string docToString(const xml_document &doc);
double stod(std::string str);
int stoi(std::string str);
std::string itos(int i);

xml_node addRequest(std::string type, xml_document &parent);
xml_node addPartition(std::string name, xml_node& parent);
xml_node addPattern(xml_node& parent);
xml_node addValue(std::string value, xml_node& parent);
#endif
