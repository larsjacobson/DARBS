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
 * @file Util.cpp
 * @brief Utility functions
 *
 * Detailed description of file
 */

// Global includes 
#include <sstream>

// Local includes
#include "Util.hpp"

// Namespaces

// Defines

// Global variables

// Member functions

// Functions

std::string docToString(const pugi::xml_document &doc) {
    std::stringstream outStringStream;
    pugi::xml_writer_stream streamWriter(outStringStream);
    doc.save(streamWriter);
    return(outStringStream.str());
}

double stod(std::string str) {
    std::istringstream in(str);
    double num;
    in >> num;
    return(num);
}

int stoi(std::string str) {
    std::istringstream in(str);
    int num;
    in >> num;
    return(num);
}

std::string itos(int i) {
    std::ostringstream ss;
    ss << i;
    return(ss.str());
}

/**
 * @brief Adds a request with a type attribute to an xml document
 * @param type The type of request
 * @param parent The parent document to add the node to
 */
xml_node addRequest(std::string type, xml_document &parent) {
    std::string requestStr = "<request type=\"" + type + "\" />"; 
    parent.load(requestStr.c_str());
    xml_node request = parent.child("request");
    return(request);
}

/**
 * @brief Adds a partition with a name attribute to an xml node
 * @param name The name of the partition
 * @param parent The parent node to add the partition to
 */
xml_node addPartition(std::string name, xml_node& parent) {
    xml_node partition = parent.append_child();
    partition.set_name("partition");
    partition.append_attribute("name").set_value(name.c_str());
    return(partition);
}

/**
 * @brief Adds a pattern to an xml node
 * @param parent The parent node to add the pattern to
 */
xml_node addPattern(xml_node& parent) {
    xml_node pattern = parent.append_child();
    pattern.set_name("pattern");
    return(pattern);
}

/**
 * @brief Adds a value attribute to an xml node
 * @param name The name of the partition
 * @param parent The parent node to add the value to
 */
xml_node addValue(std::string value, xml_node& parent) {
    xml_node node = parent.append_child(node_pcdata);
    node.set_value(value.c_str());
    return(node);
}
