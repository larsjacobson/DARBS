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
 * @file Rule.cpp
 */

// Global includes
#include <iostream>
#include <boost/bind.hpp>

//Local includes
#include "KnowledgeSource.hpp"
#include "Rule.hpp"
#include "Logger.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
Rule::Rule(TcpClient &client, KnowledgeSource *ks, boost::filesystem::path path)
    : AbstractConditional(client, path), ks(ks) {
    // Start out blocked, so we don't get the responses intended for the KS
    readConnection.block();
}

/**
 * @brief Destructor.
 */
Rule::~Rule() {
}

/**
 * @brief Main loop for the rule
 */
void Rule::run() {
    // Make sure the result of blackboard requests get sent here
    readConnection.unblock();

    bool result = false;
    bool haveIncremented = true;;
    while (haveIncremented) {
        LOG("Evaluating preconditions", INFO);
        result = precondition.evaluate();
        LOG("Preconditions evaluated " << result, INFO);

        if (result) {
            LOG("Evaluating actions", INFO);
            action.evaluate();
            timesFired += 1;
        }

        LOG("Incrementing variables", INFO);
        haveIncremented = precondition.incrementVariables();
    }
    precondition.reset();
    action.reset();

    readConnection.block();
}

/**
 * @brief Call a function contained in an external library
 *
 * @param libraryName The name of library containing the function
 * @param functionName The name of the function to call
 * @param arg An argument to pass to the function
 * @return The return value of the function
 */
std::string Rule::call(std::string libraryName, std::string functionName,
                       std::string arg) {
    return(ks->call(libraryName, functionName, arg));
}

        
