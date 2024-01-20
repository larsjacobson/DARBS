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
  USA
 *
 */

/**
 * @file AbstractRunnable.cpp
 */

// Global includes
#include <sstream>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/trim.hpp>

//Local includes
#include "AbstractConditional.hpp"
#include "AbstractRunnable.hpp"
#include "Logger.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
AbstractRunnable::AbstractRunnable(AbstractConditional *conditional):conditional(conditional) {
}

/**
 * @brief Destructor.
 */
AbstractRunnable::~AbstractRunnable()
{
}

/**
 * @brief Substitute variables in all patterns contained within a a request
 *
 * @param requestSub The xml request containing the patterns to substitute
 */
void AbstractRunnable::substitutePatterns(xml_node &requestSub) {
    // Grab all of the patterns
    xpath_query query("//pattern");
    xpath_node_set patterns = requestSub.select_nodes(query);

    // Loop through them and perform the variable substitution
    xpath_node_set::const_iterator pattern = patterns.begin();
    for (pattern; pattern != patterns.end(); pattern++) {
        xml_node node = pattern->node();
        substituteVariables(node);
    } 
}

/**
 * @brief Substitute variables with their appropriate value in a pattern
 *
 * @param pattern The pattern to substitue variables in
 */
void AbstractRunnable::substituteVariables(xml_node &pattern) {
    // Regex finds either ~varname or ~~varname, matching varname as a
    // subexpressions
    boost::regex regex("~{1,2}(\\S+)");
    // Regex filter calls replaceVariable every time the regex is matched
    boost::iostreams::regex_filter filter(regex, boost::bind(&AbstractRunnable::replaceVariable, this, _1));

    // Create the filtering stream, which processes the pattern
    boost::iostreams::filtering_istream filteringStream(filter);
    std::stringstream patternStream(pattern.child_value());

    // Push the pattern to the filtering stream, which does the processing
    filteringStream.push(patternStream);

    // Get the processed stream from the filteringStream
    std::stringstream outStream;
    boost::iostreams::copy(filteringStream, outStream);

    // Put the value into the pattern, ready for sending to the server
    pattern.first_child().set_value(outStream.str().c_str());
}

/**
 * @brief Returns the value of a variable, taking its name from a regex match
 *
 * Looks for the value of the variable within the parent conditionals
 * precondition and action
 *
 * @see AbstractRunnable::substituteVariables
 * @param match A regex match containing the name of the variable as its first
 * subexpression
 * @return The value of the variable
 */
std::string AbstractRunnable::replaceVariable(const boost::cmatch& match) {
    // match[1] is the first matched subexpression, i.e. the name of the
    // variable
    std::string matchString(match[1].first, match[1].second);

    LOG("Substituting variable " << matchString, INFO);

    std::string value;
    // Try to find the value within the precondition
    value = conditional->precondition.getVariable(matchString);

    if (value.empty()) {
        // Try to find the value within the action
        value = conditional->action.getVariable(matchString);
    }

    if (value.empty()) {
        LOG("Variable " << matchString << " not found", WARNING);
    }
    else {
        LOG("Replacing " << matchString << " with " << value, INFO);
    }
    return(value);
}

/**
 * @brief Run a function contained within an external library
 *
 * @param requestSub The request containing the run command
 * @return Whether the function was called sucessfully
 */
bool AbstractRunnable::run(xml_node requestSub) {
    std::string libraryName = requestSub.attribute("library").value();
    if (libraryName == "") {
        LOG("No library name given", ERROR);
        return false;
    }

    std::string functionName = requestSub.attribute("function").value();
    if (functionName == "") {
        LOG("No function name given", ERROR);
        return false;
    }

    std::string arg = requestSub.child("pattern").child_value();

    std::string responseVariable = requestSub.attribute("variable").value();

    std::string returnValue = conditional->call(libraryName, functionName, arg);

    if (responseVariable != "") {
        addVariable(responseVariable, returnValue);
    }

    return(true);
}

/**
 * @brief Print a report message using the Logger
 * 
 * @param requestSub The request containing the message to report
 */
void AbstractRunnable::report(xml_node requestSub) {
    xml_node pattern = requestSub.child("pattern");
    for (pattern; pattern; pattern = requestSub.next_sibling("pattern")) {
        std::string message = pattern.child_value();
        boost::algorithm::trim(message);
        LOG(message, REPORT);
    }
}


