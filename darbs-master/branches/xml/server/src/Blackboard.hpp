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
 * @file Blackboard.hpp
 * @brief Header file for the DARBS blackboard
 */

#ifndef __BLACKBOARD_HPP
#define __BLACKBOARD_HPP

// Global includes
#include <string>
#include <list>
#include <iostream> 

// Local includes
#include "pugixml.hpp"
#include "Types.hpp"

// Namespace
using namespace pugi;

// Defines

// Type definitions

// Global variables

// Class definitions

/**
 * @brief A dynamic blackboard with an unspecified number of named partitions.
 *
 * The blackboard is essentially a queryable, modifiable collection of strings
 * arranged as partitions. Expressions can be added to a partition or deleted
 * from a partition. A partition can be searched for a pattern, which can
 * contain wildcard characters. Values matching these wildcards can be
 * extracted from the partition.
 */
class Blackboard
{
public:

    // construction
    Blackboard();

    // destruction
    ~Blackboard();
    
    // public access
    void process(xml_document &request,
                 xml_document &response,
                 xml_document &notification);

protected:
    // Request handlers
    void add(xml_node &request, xml_node &response, xml_node &notification);
    void del(xml_node &request, xml_node &response, xml_node &notification);
    void replace(xml_node &request, xml_node &response, xml_node &notification);
    void present(xml_node &request, xml_node &response);
    void get(xml_node &request, xml_node &response);
    void clear(xml_node &request, xml_node &response, xml_node &notification);

    // partitoning the blackboard
    void createBlackboard();

    xml_node getKey(std::string name, xml_node& parent);
    xml_node addMatch(xml_node& parent);
    
    xml_node getPartition(std::string name);

    void deletePartition(const xml_node &partition);

    bool deletePatternFromPartition(const xml_node &pattern,
                                    xml_node &partition, int limit);

    bool replacePatternOnPartition(const xml_node &findPattern,
                                   const xml_node &replacePattern,
                                   xml_node &partition, int limit);

    bool patternOnPartition(const xml_node &pattern,
                            const xml_node &partition);

    bool getMatchesOnPartition(const xml_node &pattern,
                               const xml_node &partition,
                               MultiVariableMap &matches, int limit);

private:
    // check whether pattern is an instance of expression
    bool isInstance( std::string pattern, std::string expression,
                    VariableMap *matches=NULL);

private:
    // data members
    xml_document blackboardDoc;
    xml_node blackboard;
};


#endif
