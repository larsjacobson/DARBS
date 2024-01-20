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
 * @file Action.cpp
 */

// Global includes
#include <sstream>
#include <iostream>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/bind.hpp>

//Local includes
#include "AbstractConditional.hpp"
#include "Action.hpp"
#include "DarbsCmds.hpp"
#include "Logger.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
Action::Action(AbstractConditional *conditional):AbstractRunnable(conditional) {
}

Action::Action(xml_node actionNode, AbstractConditional *conditional)
    : AbstractRunnable(conditional) {
    xml_node requestNode = actionNode.child("request");

    if (!requestNode.empty()) {
        if (requestNode == actionNode.first_child() &&
            requestNode == actionNode.last_child()) {
            // Single request, so we can make it a member variable
            requestDoc.append_copy(requestNode);
            request = requestDoc.child("request");
        }
        else {
            // Multiple requests within the action, so split them
            // up into individual actions and add them
            for (requestNode; requestNode;
                 requestNode = requestNode.next_sibling("request")) {
                xml_document actionDoc;
                xml_node node = actionDoc.append_child();
                node.set_name("action");
                node.append_copy(requestNode);
                addChild(node);
            } 
        }
    }
    else {
        LOG("No request found within action", WARNING);
    }
}

/**
 * @brief Destructor.
 */
Action::~Action()
{
}

/**
 * @brief Add a child action to the action
 *
 * @param child An xml node containing the child action
 */
void Action::addChild(xml_node &child) {
    children.push_back(new Action(child, conditional));
}

/**
 * @brief Evaluate the action and all of its sub-actions
 *
 * @return Whether the action or sub-actions evaluated sucessfully
 */
bool Action::evaluate() {
    bool result = true;
    if (children.size() > 0) {
        // We have sub-actions, so evaluate them and compile the result
        for (int i=0; i<children.size(); i++) {
            result = result && children[i]->evaluate();
        }
    }
    else {
        // No sub-actions, so we must have a request.  Evaluate it!
        result = evaluateRequest();
    }
    return(result);
}

/**
 * @brief Executes the action's request, making necessary variable substitutions
 *
 * @return Whether running the action was sucessful
 */
bool Action::evaluateRequest() {
    std::string requestType = request.attribute("type").value();

    // Create a copy of the request, so we don't substitute the variables
    // permanantly
    xml_document requestSubDoc;
    requestSubDoc.append_copy(request);
    xml_node requestSub = requestSubDoc.child("request");

    substitutePatterns(requestSub);
    
    if (requestType == DARBS_RUN) {
        return(run(requestSub));
    }
    else if (requestType == DARBS_REPORT) {
        report(requestSub);
        return(true);
    }
    else {
        xml_node response = conditional->sendRequest(requestSub);
        if (response.name() != "error") {
            evaluateResponse(response);
        }
        return(true);
    }
    return (true);
}

/**
 * @brief Adds any matches within the response to the variable map
 *
 * @param response The response to evaluate
 */
void Action::evaluateResponse(xml_node &response) {
    xml_node partition = response.child("partition");
    for (partition; partition;
         partition = partition.next_sibling("partition")) {
        xml_node pattern = partition.child("pattern");
        for (pattern; pattern;
             pattern = pattern.next_sibling("pattern")) { 
            xml_node key = pattern.child("key");
            for (key; key; key = key.next_sibling("key")) {
                std::string keyName = key.attribute("name").value();
                xml_node match = key.child("match");
                if (match) {
                    addVariable(keyName, match.child_value());
                }
            }
        }
    }
}

/**
 * @brief Assign a value to a stored variable name
 *
 * @param variableName The name of the variable
 * @param value The value to assign
 */
void Action::addVariable(std::string variableName, std::string value) {
    LOG("Adding new variable " << variableName, INFO);
    variables[variableName] = value;
}

/**
 * @brief Return the value associated with a variable
 *
 * Searches in the Action's variable list, then looks recursively at sub-actions
 *
 * @param name The name of the variable
 * @return The value associated with the variable, or "" if it is not found
 */
std::string Action::getVariable(std::string name) {
    VariableMap::iterator it;
    it = variables.find(name);
    if (it != variables.end()) {
        return(it->second);
    }

    std::string value;
    for (int i=0; i<children.size(); i++) {
        int index = children.size() - i - 1;
        value = children[index]->getVariable(name);
        if (!value.empty()) {
            return value;
        }
    }
    return("");
}

/**
 * @brief Reset the variable map in the action and all sub-actions
 */
void Action::reset() {
    variables.clear();

    for (int i=0; i<children.size(); i++) {
        children[i]->reset();
    }
}
