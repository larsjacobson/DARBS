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

#ifndef __ACTION_HPP
#define __ACTION_HPP

// Global includes
#include <string>
#include <vector>
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

// Class definitions
class AbstractConditional;

/**
 * @brief An action which can be run from a KS or Rule
 *
 * Actions can either send requests to the blackboard, or run a subset of
 * requests locally.  Variables can be stored and retrieved for substitution
 * into patterns in the request.  Each action either contains a request, or
 * sub-actions which can contain requests or further sub-actions.  All
 * evaluation or variable finding functions are carried out recursively starting * at the first Action.
 */
class Action : public AbstractRunnable {
public:

    // construction
    Action(AbstractConditional *conditional);

    Action(xml_node actionNode, AbstractConditional *conditional);

    // destruction
    ~Action();
    
    // public access
    bool evaluate();

    bool evaluateRequest();

    void evaluateResponse(xml_node &response);

    void addChild(xml_node &Action);

    std::string getVariable(std::string);

    void addVariable(std::string variableName, std::string value);

    void reset();

protected:

private:
    VariableMap variables;
};


#endif
