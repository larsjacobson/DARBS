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
 * @file Precondition.hpp
 * @brief Header file for a Precondition
 */

#ifndef __PRECONDITION_HPP
#define __PRECONDITION_HPP

// Global includes
#include <string>
#include <set>
#include <boost/regex.hpp>

// Local includes
#include "pugixml.hpp"
#include "Types.hpp"
#include "ClientTypes.hpp"
#include "AbstractRunnable.hpp"

// Namespace
using namespace pugi;

// Defines

// Type definitions

// Global variables
enum PreconditionType {
    AND,
    OR,
    NOT
};

// Class definitions
class AbstractConditional;

/**
 * @brief A precondition which is evaluated to see if further action is needed
 *
 * Preconditions can either send requests to the blackboard, or run a subset of
 * requests locally.  Variables can be stored and retrieved for substitution
 * into patterns in the request.  Each precondition either contains a request,
 * or sub-preconditions which can contain requests or further
 * sub-preconditions.  All evaluation or variable finding functions are carried
 * out recursively starting at the first Precondition
 */
class Precondition : public AbstractRunnable {
public:

    // construction
    Precondition(AbstractConditional *conditional);
    Precondition(xml_node preconditionNode, AbstractConditional *conditional);

    // destruction
    ~Precondition();
    
    // public access
    bool evaluate();

    bool evaluateRequest();

    bool evaluateResponse(xml_node &response);

    bool incrementVariables();

    void addChild(xml_node &precondition);

    std::string getVariable(std::string);
    
    void addVariable(std::string variableName, std::string value);
    void appendVariable(std::string variableName, std::string value);

    void reset();

    bool compare(xml_node requestSub);

    void updatePartitionWatchList(xml_node &requestSub);
    bool partitionWatched(std::string partitionName);
protected:

private:
    MultiVariableMap variables;
    int variableIndex;

    xml_document *lastRequestDoc;
    xml_node lastRequest;
    bool lastReturnValue;
    PreconditionType type;

    std::set<std::string> partitionWatchList;
};


#endif
