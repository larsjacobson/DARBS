// $id$

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
 * @file Rule.hpp
 * @brief Header file for a Knowledge Source
 */

#ifndef __RULE_HPP
#define __RULE_HPP

// Global includes
#include <string>
#include <boost/filesystem.hpp>

// Local includes
#include "pugixml.hpp"
#include "TcpClient.hpp"
#include "AbstractConditional.hpp"

// Namespace
using namespace pugi;

// Defines

// Type definitions

// Global variables

// Class definitions

class KnowledgeSource;

/**
 * @brief A rule to be evaluated in a Rule Based KS
 *
 * Rules are simple precondition/action pairs.  Each time they are run the
 * precondition is re-evaluated, and if it is true then the action is evaluated.
 */
class Rule : public AbstractConditional {
public:
    // construction
    Rule(TcpClient &client, KnowledgeSource *ks, boost::filesystem::path path);

    // destruction
    ~Rule();
    
    // public access
    void run();

    std::string call(std::string libraryName, std::string functionName,
                     std::string arg);
protected:
private:
    KnowledgeSource *ks;
};


#endif
