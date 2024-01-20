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
 * @file Precondition.cpp
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
#include "Precondition.hpp"
#include "DarbsCmds.hpp"
#include "Logger.hpp"
#include "Util.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 * @param conditional The conditional which contains the precondition
 */
Precondition::Precondition(AbstractConditional *conditional)
    : AbstractRunnable(conditional) {
}

/**
 * @brief Constructor
 * @param preconditionNode The xml node containing the precondition's definition
 * @param conditional The conditional which contains the precondition
 */
Precondition::Precondition(xml_node preconditionNode,
                           AbstractConditional *conditional)
    : AbstractRunnable(conditional) {
    xml_attribute typeAttr = preconditionNode.attribute("type");
    if (!typeAttr.empty()) {
        std::string typeVal = typeAttr.value();
        if (typeVal == "and") {
            type = AND;
        }
        else if (typeVal == "or") {
            type = OR;
        }
        else if (typeVal == "not") {
            type = NOT;
        }
    }
    else {
        type = AND;
    }
    
    // Add all the sub-preconditions
    xml_node precondition = preconditionNode.child("precondition");
    for (precondition; precondition;
         precondition = precondition.next_sibling("precondition")) {
         addChild(precondition);
    }

    xml_node requestNode = preconditionNode.child("request");
    if (!requestNode.empty()) {
        if (requestNode == preconditionNode.first_child() &&
            requestNode == preconditionNode.last_child()) {
            // Single request within a precondition, so we can make it a member
            // variable
            requestDoc.append_copy(requestNode);
            request = requestDoc.child("request");
        }
        else {
            // Multiple requests within the precondition, so split them
            // up into individual preconditions and add them
            for (requestNode; requestNode;
                 requestNode = requestNode.next_sibling("request")) {
                xml_document preconditionDoc;
                xml_node node = preconditionDoc.append_child();
                node.set_name("precondition");
                node.append_copy(requestNode);
                addChild(node);
            }
        }
    }
}


/**
 * @brief Destructor.
 */
Precondition::~Precondition()
{
}

/**
 * @brief Add a child precondition to the precondition
 *
 * @param child An xml node containing the child precondition
 */
void Precondition::addChild(xml_node &child) {
   children.push_back(new Precondition(child, conditional));
}

/**
 * @brief Evaluate the precondition and all of its sub-preconditions
 *
 * @return Whether the precondition evaluates true
 */
bool Precondition::evaluate() {
    bool result = false;
    if (children.size() > 0) {
        // We have sub-preconditions, so evaluate them and compile the result
        for (int i=0; i<children.size(); i++) {
            result = children[i]->evaluate();
            if (type == AND) {
                if (result) {
                    continue;
                }
                else {
                    break;
                }
            }
            else if (type == OR) {
                if (result) {
                    break;
                }
                else {
                    continue;
                }
            }
        }
    }
    else {
        // No sub-preconditions, so we must have a request.  Evaluate it!
        result = evaluateRequest();
    }

    // Invert the result if necessary
    if (type == NOT) {
        result = !result;
    }
    return(result);
}

/**
 * @brief Move to the next value of variables within this or sub-preconditions
 *
 * The variable to increment is determined based upon the order in the KS or
 * rule file.  It searches depth first, then in reverse order through the
 * sub-preconditions until a non-incremented precondition is found, finally
 * looking at any variables contained in this precondition.
 *
 * @return Whether a variable index has been was incremented
 */
bool Precondition::incrementVariables() {
    // Try to increment sub-preconditions first
    if (children.size() > 0) {
        bool incremented;
        int index;
        // Increment later preconditions before earlier ones
        for (int i=0; i<children.size(); i++) {
            // Loop backwards
            index = children.size() - i - 1;
            Precondition *precondition = (Precondition *) children[index];
            incremented = precondition->incrementVariables();
            if (incremented) {
                return(true);
            }
        }
    }

    // No sub-preconditions incremented, so try to increment this
    // precondition's variables
    if (!variables.empty() &&
        variableIndex < variables.begin()->second.size() - 1) {
        variableIndex += 1;
        return(true);
    }
    else {
        // Reset the variable index in case an earlier precondition increments
        variableIndex = 0;
    }

    return(false);
}

/**
 * @brief Evaluate the precondition's request
 *
 * @return Whether the request evaluated true
 */
bool Precondition::evaluateRequest() {
    
    std::string requestType = request.attribute("type").value();

    // Create a copy of the request, so we don't substitute the variables
    // permanantly
    xml_document requestSubDoc;
    requestSubDoc.append_copy(request);
    xml_node requestSub = requestSubDoc.child("request");

    updatePartitionWatchList(requestSub);
    substitutePatterns(requestSub);

    if (requestType == DARBS_COMPARE) {
        return(compare(requestSub));
    }
    else if (requestType == DARBS_RUN) {
        return(run(requestSub));
    }
    else if (requestType == DARBS_REPORT) {
        report(requestSub);
        return(true);
    }
    else {
        // Request needs evaluating on the blackboard
        // We only evaluate the request if it is different from the previous
        // request.  
        if (lastRequestDoc == NULL ||
            docToString(*(lastRequestDoc)) != docToString(requestSubDoc)) {
            delete(lastRequestDoc);
            lastRequestDoc = new xml_document();
            lastRequestDoc->append_copy(requestSub);
            lastRequest = lastRequestDoc->child("request");

            xml_node response = conditional->sendRequest(requestSub);
            if (response.name() == "error") {
                // Try to get out of the KSs main loop as quickly as possible
                if (type == NOT) {
                    lastReturnValue = true;
                }
                else {
                    lastReturnValue = false;
                }
            }
            else {
                lastReturnValue = evaluateResponse(response);
            }
        }
        return(lastReturnValue);
    }
}

/**
 * @brief Works out from a blackboard response whether the request was sucessful
 *
 * Evaluates true unless there is a DARBS_FALSE value anywhere within the
 * response.  Also adds any matches within the response to the variable map.
 *
 * @param response The response to evaluate
 * @return Whether the request was sucessful
 */
bool Precondition::evaluateResponse(xml_node &response) {
    bool returnValue = true;
    std::string value = response.child_value();
    if (value == DARBS_FALSE) {
        returnValue = false;
    }
    
    xml_node partition = response.child("partition");
    for (partition; partition;
         partition = partition.next_sibling("partition")) {
        value = partition.child_value();
        if (value == DARBS_FALSE) {
            returnValue = false;
        }

        xml_node pattern = partition.child("pattern");
        for (pattern; pattern;
             pattern = pattern.next_sibling("pattern")) {
            value = pattern.child_value();
            if (value == DARBS_FALSE) {
                returnValue = false;
            }
            
            xml_node key = pattern.child("key");
            for (key; key; key = key.next_sibling("key")) {
                std::string keyName = key.attribute("name").value();
                // Clear the variable for a new response as the previous
                // values are no longer relevant to the request
                variables[keyName].clear();
                xml_node match = key.child("match");
                for (match; match; match = match.next_sibling("match")) {
                    appendVariable(keyName, match.child_value());
                }
            }
        }
    }
    return (returnValue);
}


/**
 * @brief Append a value to a stored variable name
 *
 * @param variableName The name of the variable
 * @param value The value to assign
 */
void Precondition::appendVariable(std::string variableName, std::string value) {
    LOG("Adding variable " << variableName, INFO);
    variables[variableName].push_back(value);
}

/**
 * @brief Assign a value to a stored variable name
 *
 * @param variableName The name of the variable
 * @param value The value to assign
 */
void Precondition::addVariable(std::string variableName, std::string value) {
    LOG("Adding variable " << variableName, INFO);
    variables[variableName].clear();
    variables[variableName].push_back(value);
}

/**
 * @brief Return the current value associated with a variable
 *
 * Searches in the Precondition's variable list, then looks recursively at
 * sub-preconditions
 *
 * @param name The name of the variable
 * @return The value associated with the variable, or "" if it is not found
 */
std::string Precondition::getVariable(std::string name) {
    MultiVariableMap::iterator it;
    it = variables.find(name);
    if (it != variables.end()) {
        return(it->second[variableIndex]);
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
 * @brief Reset the variable map in the precondition and all sub-preconditions
 */
void Precondition::reset() {
    variableIndex = 0;
    delete lastRequestDoc;
    lastRequestDoc = NULL;
    variables.clear();

    for (int i=0; i<children.size(); i++) {
        children[i]->reset();
    }
}

/**
 * @brief Takes the partitions from a request and adds them to a watch list
 * @param requestSub The request to take the partition from
 */
void Precondition::updatePartitionWatchList(xml_node &requestSub) {
    xml_node partition = requestSub.child("partition");
    for (partition; partition; partition=partition.next_sibling("partition")) {
        partitionWatchList.insert(partition.attribute("name").value());
    }
}

/**
 * @brief Checks whether a partition is contained in the watch list
 * @param partitionName The name of the partition to search for
 * @return Whether the partition is in the watch list
 */
bool Precondition::partitionWatched(std::string partitionName) {
    // Always return true if we are watching all partitions, using a '?'
    // wildcard
    if (partitionWatchList.find("?") != partitionWatchList.end()) {
        return(true);
    }
    if (partitionWatchList.find(partitionName) != partitionWatchList.end()) {
        return(true);
    }
    else {
        // Check sub-preconditions to see whether the partition is watched
        for (int i=0; i<children.size(); i++) {
            if (((Precondition *)children[i])->partitionWatched(partitionName)) {
                return(true);
            }
        }
    }
    return(false);
}

/**
 * @brief Compares two values using a operator defined in a request
 *
 * @param requestSub The request containing the compare command
 * @return The result of the comparison
 */
bool Precondition::compare(xml_node requestSub) {
    std::string op = requestSub.attribute("operator").value();
    if (op == "") {
        LOG("No operator in compare statement", ERROR);
        return(false);
    }

    xml_node firstPattern = requestSub.child("pattern");
    xml_node secondPattern = firstPattern.next_sibling("pattern");

    if (firstPattern.empty() || secondPattern.empty()) {
        LOG("Compare needs two patterns", ERROR);
        return(false);
    }

    double firstNum = stod(firstPattern.child_value());
    double secondNum = stod(secondPattern.child_value());

    LOG("Running compare: " << firstNum << " " << op << " " << secondNum,
        INFO);

    if (op == "==") {
        return(firstNum == secondNum);
    }
    else if (op == "!="){
        return(firstNum != secondNum);
    }
    else if (op == "<"){
        return(firstNum < secondNum);
    }
    else if (op == "<="){
        return(firstNum <= secondNum);
    }
    else if (op == ">"){
        return(firstNum > secondNum);
    }
    else if (op == ">="){
        return(firstNum >= secondNum);
    }
    LOG("Operator not recognised", ERROR);
    return(false);
}
    
