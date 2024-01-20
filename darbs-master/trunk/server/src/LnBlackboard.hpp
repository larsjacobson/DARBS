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
 * @file LnBlackboard.hpp
 * @brief Header file for the DARBS blackboard
 */

#ifndef __LNBLACKBOARD_HPP
#define __LNBLACKBOARD_HPP

// Global includes
#include <string>
#include <list>
#include <map>
#include <iostream> 

// Local includes

// Defines

// error messages
#define PARTITION_NOT_FOUND      "partition not found!"
#define ARBS_NO_NAMES_DEFINED    "no partitions defined!"
#define ARBS_ERROR               "error"
#define ARBS_CONFIRM             "confirm"
#define ARBS_COMMAND_NOT_FOUND   "command not found"
#define ARBS_FALSE               "false"
#define ARBS_TRUE                "true"

//ARBS commands
#define ARBS_ADD                 "add"

#define ARBS_DEL                 "del_first"
#define ARBS_DEL_ALL             "del_all"

#define ARBS_CLR                 "clr_partition"
#define ARBS_CLR_ALL             "clr_board"

#define ARBS_RET                 "ret_first"
#define ARBS_RET_ALL             "ret_all"

#define ARBS_GET                 "get_contents"

#define ARBS_SETUP_BLACKBOARD    "setup_blackboard"
#define ARBS_ADD_PARTITIONS      "add_partitions"
#define ARBS_GET_PARTITIONS      "get_partitions"

#define ARBS_PARTITION_EXIST     "partition_exist"
#define ARBS_PARTITION_NOT_EXIST "partition_not_exist"

#define ARBS_ON_PARTITION        "on_partition"
#define ARBS_NOT_ON_PARTITION    "not_on_partition"

#define ARBS_ON_BLACKBOARD       "on_blackboard"
#define ARBS_NOT_ON_BLACKBOARD   "not_on_blackboard"

// Type definitions
typedef std::map<std::string, std::string, std::less<std::string> > partition_type;
typedef partition_type::value_type value_type;


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
class LnBlackboard
{
public:

    // construction
    LnBlackboard();
    LnBlackboard(std::string partitions);

    // destruction
    ~LnBlackboard();
    
    // public access
    std::string process( std::string expression );

protected:

    // partitoning the blackboard
    void setUpBlackboard( std::string partitions, int clr = true, std::string pattern = "" );

    // clear all partitions
    void clearBlackboard();

    // clear partition
    void clearPartition( std::string partition );

    // get names of all existing partitions
    std::string getPartitionNames();

    // check existence of a partition
    int checkExistenceOfPartition( std::string name);

    // check for absents
    int absentOnBlackboard( std::string statement );
    int absentOnPartition( std::string statement, std::string partition );

    // check for presents
    std::string getFirstMatch( std::string pattern, std::string partition );
    std::string getAllMatches( std::string pattern, std::string partition );

    // write on partition
    void add( std::string pattern, std::string partition );

    // remove from partition
    void removeFirstMatch( std::string pattern, std::string partition );
    void removeAllMatches( std::string pattern, std::string partition);

    // access to partition
    std::string getContents( std::string partition);

private:
    // parse command from server
    std::string parse( std::string cmd, std::string pattern, std::string partition);

    // check whether pattern is an instance of expression
    std::string isInstance( std::string pattern, std::string expression );

    // deletes the first and the last char
    std::string core( std::string pattern );

    // remove unessesary seperators from string
    std::string compress( std::string strng);

private:
    // data members
    partition_type  m_blackboard; // map of partitions    
};


#endif
