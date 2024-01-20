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
 * @file LnBlackboard.cpp
 * @brief Central knowledge store and communication backbone for DARBS
 *
 * The blackboard is realised as an STL::map. Each entry, e.g. partition,
 * consists of a STL::string for the partition name and a STL::string with
 * the contents of the partition. This allows us to have an unspecified
 * number of named partitions.
 */

// Global includes
#include <boost/regex.hpp>
#include <stdexcept>
#include <boost/algorithm/string/trim.hpp>
#include <set>

// Local includes
#include "Blackboard.hpp"
#include "DarbsCmds.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Util.hpp"

// Namespaces
using namespace pugi;

// Defines

// Global variables

// Member functions

/**
 * @brief Default constructor
 */
Blackboard::Blackboard()
{
    createBlackboard();
}

/**
 * @brief Destructor.
 */
Blackboard::~Blackboard()
{
//    delete &blackboard;
}


/**
 * @brief Parses and processes DARBS expressions.
 *
 * Split the expression up into component parts (command, pattern and
 * partition), then calls parse().  Finally formats and returns the output
 * string.
 *
 * @param expression The DARBS expression to process
 * @return The return string from the blackboard
 */
void Blackboard::process(xml_document &requestDoc,
                         xml_document &responseDoc,
                         xml_document &notificationDoc) {
    xml_node request = requestDoc.child("request");
    xml_attribute commandType = request.attribute("type");
    if (commandType.empty()) {
        LOG("No command type given in request", ERROR);
        return;
    }

    // Create the response document
    responseDoc.load("<response></response>");
    xml_node response = responseDoc.child("response");

    // Create the notification document
    notificationDoc.load("<notification></notification>");
    xml_node notification = notificationDoc.child("notification");
    
    if (commandType.value() == DARBS_ADD) {
        add(request, response, notification);
    }
    else if (commandType.value() == DARBS_DELETE) {
        del(request, response, notification);
    }
    else if (commandType.value() == DARBS_REPLACE) {
        replace(request, response, notification);
    }
    else if (commandType.value() == DARBS_CLEAR) {
        clear(request, response, notification);
    }
    else if (commandType.value() == DARBS_GET) {
        get(request, response);
    }
    else if (commandType.value() ==  DARBS_PRESENT) {
        present(request, response);
    }
    else {
        LOG("Unknown request type", ERROR);
    }
}

/**
 * @brief Attempt to add partitions or patterns to the blackboard.
 *
 * Accepts request in a two formats
 * - <partition name="pname" /> Adds an empty partition, and returns true
 *   if the partition does not already exist, or false otherwise
 * - <partition name="pname"><pattern>Some knowledge</pattern></partition> Adds
 *   knowledge to a partition, creating the partition if it does not exist. 
 * Returns true if the data is added succesfully.
 *
 * @param request An xml node containing the partitions to add
 * @param response An xml node which will be filled with values indicating
 * success or failure for each partition or pattern to be added
 */
void Blackboard::add(xml_node &request, xml_node &response,
                     xml_node &notification) {
    std::set<std::string> changedPartitions;
    // Loop through the partitions in the request
    xml_node partition = request.child("partition");
    for (partition; partition; partition = partition.next_sibling("partition")) {
        // Get the name of the partition
        std::string partitionName = partition.attribute("name").value();
        // Find the corresponding partition on the blackboard
        xml_node bbPartition = getPartition(partitionName);

        // Add the partition to the response
        xml_node responsePartition = addPartition(partitionName, response);

        xml_node pattern = partition.child("pattern");
        if (pattern.empty()) {
            // No child patterns, so we just need to add the partition if
            // it doesn't already exist
            if (bbPartition.empty()) {
                addPartition(partitionName, blackboard);
                addValue(DARBS_TRUE, responsePartition);
                changedPartitions.insert(partitionName);
            }
            else {
                // Partition already exists so return FALSE
                addValue(DARBS_FALSE, responsePartition);
            }
        }
        else {
            if (bbPartition.empty()) {
                // Add the partition if it doesn't already exist
                bbPartition = addPartition(partitionName, blackboard);
            }
            for (pattern; pattern; pattern = pattern.next_sibling("pattern")) {
                // Add a pattern node to the response
                xml_node responsePattern = addPattern(responsePartition);

                // Add the pattern to the blackboard
                xml_node bbPattern = addPattern(bbPartition); 
                addValue(pattern.child_value(), bbPattern);

                // Return true
                addValue(DARBS_TRUE, responsePattern);
                changedPartitions.insert(partitionName);
            }
        }
    }

    std::set<std::string>::iterator it = changedPartitions.begin();
    for (it; it!=changedPartitions.end(); it++) {
        addPartition(*it, notification);
    }
}

/**
 * @brief Deletes partitions or patterns from the blackboard
 *
 * Accepts request contents in the following forms:
 * - <partition name="pname" /> Deletes a partition.  Returns true if the
 *   partition is found, false otherwise
 * - <partition name="pname"><pattern>Some knowledge</pattern></partition>
 *   Deletes knowledge from a partition.  Returns true if a matching pattern is
 *   found, false otherwise.
 * - <partition name="?"><pattern>Some knowledge</pattern></partition> Deletes
 *   knowledge from any partion on the blackboard.  Returns true if a matching
 *   pattern is found, false otherwise.
 * When deleting knowledge the request can contain an optional limit attribute,
 * which limits the number of patterns deleted from a single partition.
 *
 * @param request An xml node containing the partitions and patterns to delete
 * @param response An xml node containing the success of the various requests
 * @todo Split function in half - that indents wayyy to far
 * @smell? When deleting from the entire blackboard it will delete up
 * to the limit on each partition, which seems like it's probably undesirable
 * behaviour.  Bit of an edge case though, and the fix isn't super simple.
 * Document it and move on methinks...
 */
void Blackboard::del(xml_node &request, xml_node &response,
                     xml_node &notification) {
    std::set<std::string> changedPartitions;
    xml_node partition = request.child("partition");
    for (partition; partition; partition = partition.next_sibling("partition")) {
        // Get the name of the partition
        std::string partitionName = partition.attribute("name").value();

        // Add the partition to the response
        xml_node responsePartition = addPartition(partitionName, response);

        if (partitionName != "?") {
            // We are only searching a specific partition

            // Find the corresponding partition on the blackboard
            xml_node bbPartition = getPartition(partitionName);

            xml_node pattern = partition.child("pattern");
            if (pattern.empty()) {
                // No patterns provided
                if (!bbPartition.empty()) {
                    // Partition exists, so delete it and return true
                    deletePartition(bbPartition);
                    addValue(DARBS_TRUE, responsePartition);
                    changedPartitions.insert(partitionName);
                }
                else {
                    // Partition doesn't exist, so return false
                    addValue(DARBS_FALSE, responsePartition);
                }
            }
            else {
                // Patterns provided
                // Grab the limit from the request (or 0 if it doesn't exist)
                int limit = request.attribute("limit").as_int();
                
                for (pattern; pattern;
                     pattern = pattern.next_sibling("pattern")) {
                    // Add a pattern to the response
                    xml_node responsePattern = addPattern(responsePartition);

                    if (deletePatternFromPartition(pattern, bbPartition,
                                                   limit)) {
                        // Pattern was found and deleted, so return true
                        addValue(DARBS_TRUE, responsePattern);
                        changedPartitions.insert(partitionName);
                    }
                    else {
                        // Pattern wasn't found, so return false
                        addValue(DARBS_FALSE, responsePattern);
                    }
                }
            }
        }
        else {
            // No partition name give, so search on the entire blackboard
            // Grab the limit from the request (or 0 if it doesn't exist)
            int limit = request.attribute("limit").as_int();

            xml_node pattern = partition.child("pattern");
            if (pattern.empty()) {
                // This shouldn't happen if the XML makes sense
                LOG("No pattern provided provided with ? as the partition name",
                    ERROR);
                addValue(DARBS_FALSE, responsePartition);
                continue;
            }
                
            bool patternMatched = false;
            for (pattern; pattern;
                 pattern = pattern.next_sibling("pattern")) {
                // Add a pattern to the response
                xml_node responsePattern = addPattern(responsePartition);

                xml_node bbPartition = blackboard.child("partition");
                for (bbPartition; bbPartition;
                     bbPartition = bbPartition.next_sibling("partition")) {
                    if (deletePatternFromPartition(pattern, bbPartition,
                                                   limit)) {
                        // Pattern was found and deleted, so return true
                        patternMatched = true;
                        std::string pn = bbPartition.attribute("name").value();
                        changedPartitions.insert(pn);
                    }
                }
                addValue(patternMatched ? DARBS_TRUE : DARBS_FALSE,
                         responsePattern);
            }
        }
    }

    std::set<std::string>::iterator it = changedPartitions.begin();
    for (it; it!=changedPartitions.end(); it++) {
        addPartition(*it, notification);
    }
}

/**
 * @brief Find and replace patterns on the blackboard
 * 
 * Accepts request contents in the following forms:
 * - <partition name="pname">
 *      <pattern>Find pattern</pattern>
 *      <pattern>Replace pattern</pattern>
 *   </partition>
 *   Replaces knowledge on a partition.  Returns true if a pattern matching the
 *   find pattern is found, false otherwise.
 * - <partition name="?">
 *      <pattern>Find pattern</pattern>
 *      <pattern>Replace pattern</pattern>
 *   </partition>
 *   Replaces knowledge from any partion on the blackboard.  Returns true if a
 *  pattern matching the find pattern is found, false otherwise.
 * 
 * When replacing knowledge the request can contain an optional limit attribute,
 * which limits the number of patterns replaced from a single partition.
 *
 * @param request An xml node containing the partitions and patterns to replace
 * @param response An xml node containing the success of the various requests
 */
void Blackboard::replace(xml_node &request, xml_node &response,
                         xml_node &notification) {
    std::set<std::string> changedPartitions;
    xml_node partition = request.child("partition");
    for (partition; partition; partition = partition.next_sibling("partition")) {
        // Get the name of the partition
        std::string partitionName = partition.attribute("name").value();

        // Add the partition to the response
        xml_node responsePartition = addPartition(partitionName, response);

        if (partitionName != "?") {
            // We are only searching a specific partition

            // Find the corresponding partition on the blackboard
            xml_node bbPartition = getPartition(partitionName);

            xml_node findPattern = partition.child("pattern");
            xml_node replacePattern = findPattern.next_sibling("pattern");

            if (findPattern.empty() || replacePattern.empty()) {
                // This shouldn't happen if the XML makes sense
                LOG("Replace request doesn't have enough patterns", ERROR);
                addValue(DARBS_FALSE, responsePartition);
                continue; 
            }
            // Grab the limit from the request (or 0 if it doesn't exist)
            int limit = request.attribute("limit").as_int();

            // Patterns provided
            for (findPattern; findPattern;
                findPattern = replacePattern.next_sibling("pattern")) {
                // Increment the replace pattern
                replacePattern = findPattern.next_sibling("pattern");

                if (replacePattern.empty()) {
                    LOG("Odd number of patterns in replace request", ERROR);
                    break;
                }

                // Add a pattern to the response
                xml_node responsePattern = addPattern(responsePartition);

                if (replacePatternOnPartition(findPattern, replacePattern,
                                              bbPartition, limit)) {
                        // Pattern was found and replaced, so return true
                    addValue(DARBS_TRUE, responsePattern);
                    changedPartitions.insert(partitionName);
                }
                else {
                    // Pattern wasn't found, so return false
                    addValue(DARBS_FALSE, responsePattern);
                }
            }
        }
        else {
            // No partition name give, so search on the entire blackboard
            xml_node findPattern = partition.child("pattern");
            xml_node replacePattern = findPattern.next_sibling("pattern");
            if (findPattern.empty() || replacePattern.empty()) {
                // This shouldn't happen if the XML makes sense
                LOG("Replace request doesn't have enough patterns", ERROR);
                addValue(DARBS_FALSE, responsePartition);
                continue;
            }
            
            // Grab the limit from the request (or 0 if it doesn't exist)
            int limit = request.attribute("limit").as_int();

            for (findPattern; findPattern;
                 findPattern = replacePattern.next_sibling("pattern")) {
                // Increment the replace pattern
                replacePattern = findPattern.next_sibling("pattern");

                if (replacePattern.empty()) {
                    LOG("Odd number of patterns in replace request", ERROR);
                    break;
                }

                // Add a pattern to the response
                xml_node responsePattern = addPattern(responsePartition);

                bool patternMatched = false;

                xml_node bbPartition = blackboard.child("partition");
                for (bbPartition; bbPartition;
                     bbPartition = bbPartition.next_sibling("partition")) {
                    if (replacePatternOnPartition(findPattern, replacePattern,
                                                  bbPartition, limit)) {
                        // Pattern was found, so return true
                        patternMatched = true;
                        std::string pn = bbPartition.attribute("name").value();
                        changedPartitions.insert(pn);
                    }
                }
                addValue(patternMatched ? DARBS_TRUE : DARBS_FALSE,
                         responsePattern);
            }
        }
    }

    std::set<std::string>::iterator it = changedPartitions.begin();
    for (it; it!=changedPartitions.end(); it++) {
        addPartition(*it, notification);
    }
}

/**
 * @brief Find patterns on the blackboard
 * 
 * Accepts request contents in the following forms:
 * - <partition name="pname" /> Returns true if a partition with a matching
 *   name is found, false otherwise.
 * - <partition name="pname"><pattern>Some knowledge</pattern></partition>
 *   Finds knowledge on a partition.  Returns true if a pattern matching the
 *   pattern is found, false otherwise.
 * - <partition name="?"><pattern>Find pattern</pattern></partition>
 *   Finds knowledge from any partition on the blackboard.  Returns true if a
 *  pattern matching the pattern is found, false otherwise.
 * 
 * @param request An xml node containing the partitions and patterns to find
 * @param response An xml node containing the success of the various requests
 */
void Blackboard::present(xml_node &request, xml_node &response) {
    xml_node partition = request.child("partition");
    for (partition; partition; partition = partition.next_sibling("partition")) {
        // Get the name of the partition
        std::string partitionName = partition.attribute("name").value();

        // Add the partition to the response
        xml_node responsePartition = addPartition(partitionName, response);

        if (partitionName != "?") {
            // We are only searching a specific partition

            // Find the corresponding partition on the blackboard
            xml_node bbPartition = getPartition(partitionName);

            xml_node pattern = partition.child("pattern");
            if (pattern.empty()) {
                // No patterns provided
                if (!bbPartition.empty()) {
                    // Partition exists, so return true
                    addValue(DARBS_TRUE, responsePartition);
                }
                else {
                    // Partition doesn't exist, so return false
                    addValue(DARBS_FALSE, responsePartition);
                }
            }
            else {
                // Patterns provided
                for (pattern; pattern;
                     pattern = pattern.next_sibling("pattern")) {
                    // Add a pattern to the response
                    xml_node responsePattern = addPattern(responsePartition);

                    if (patternOnPartition(pattern, bbPartition)) {
                        // Pattern was found, so return true
                        addValue(DARBS_TRUE, responsePattern);
                    }
                    else {
                        // Pattern wasn't found, so return false
                        addValue(DARBS_FALSE, responsePattern);
                    }
                }
            }
        }
        else {
            // No partition name give, so search on the entire blackboard
            xml_node pattern = partition.child("pattern");
            if (pattern.empty()) {
                // This shouldn't happen if the XML makes sense
                LOG("No pattern provided provided with ? as the partition name",
                    ERROR);
                addValue(DARBS_FALSE, responsePartition);
                continue;
            }


            for (pattern; pattern;
                 pattern = pattern.next_sibling("pattern")) {
                // Add a pattern to the response

                xml_node responsePattern = addPattern(responsePartition);

                bool patternMatched = false;

                xml_node bbPartition = blackboard.child("partition");
                for (bbPartition; bbPartition;
                     bbPartition = bbPartition.next_sibling("partition")) {
                    if (patternOnPartition(pattern, bbPartition)) {
                        // Pattern was found, so return true
                        patternMatched = true;
                    }
                }
                addValue(patternMatched ? DARBS_TRUE : DARBS_FALSE,
                         responsePattern);
            }
        }
    }
}

/**
 * @brief Return patterns from the blackboard
 *
 * Accepts request contents in the following forms:
 * - <request type="get" /> Returns the entire contents of the blackboard
 * - <request type="get"><partition name="pname" /></request> Returns the
 *   contents of a single partition
 * - <request type="get">
 *      <partition name="pname">
 *          <pattern>A pattern</pattern>
 *      <partition>
 *   </request>
 *   Returns patterns matching the partition, along with matching wildcards
 *
 * @param request An xml node containing the partitions and patterns to return
 * @param response An xml node containing the result of the various requests
 *
 * @todo Function is stupidly long - split it up some
 */
void Blackboard::get(xml_node &request, xml_node &response) {
    xml_node partition = request.child("partition");
    if (partition.empty()) {
        // Return the entire blackboard
        xml_node bbPartition = blackboard.child("partition");
        for (bbPartition; bbPartition;
             bbPartition = bbPartition.next_sibling("partition")) {
            response.append_copy(bbPartition);
        }
        return;
    }

    for (partition; partition; partition = partition.next_sibling("partition")) {
        // Get the name of the partition
        std::string partitionName = partition.attribute("name").value();

        // Add the partition to the response
        xml_node responsePartition = addPartition(partitionName, response);

        if (partitionName != "?") {
            // We are only searching a specific partition

            // Find the corresponding partition on the blackboard
            xml_node bbPartition = getPartition(partitionName);

            xml_node pattern = partition.child("pattern");
            if (pattern.empty()) {
                // No patterns provided
                if (!bbPartition.empty()) {
                    // Return the entire partition
                    response.remove_child(responsePartition);
                    response.append_copy(bbPartition);
                }
                else {
                    addValue(DARBS_FALSE, responsePartition);
                }
            }
            else {
                // Patterns provided
                int limit = request.attribute("limit").as_int();

                for (pattern; pattern;
                     pattern = pattern.next_sibling("pattern")) {
                    // Add a pattern to the response
                    xml_node responsePattern = addPattern(responsePartition);

                    MultiVariableMap matches;
                    if (getMatchesOnPartition(pattern, bbPartition, matches,
                                              limit)) {
                        // Add matches to the response partition
                        MultiVariableMap::iterator it = matches.begin();
                        for (it; it != matches.end(); it++) {
                           xml_node responseKey = getKey(it->first,
                                                         responsePattern);
                            std::vector<std::string>::iterator jt = it->second.begin();
                            for (jt; jt != it->second.end(); jt++) {
                                xml_node responseMatch = addMatch(responseKey);
                                addValue(*jt, responseMatch);
                            }
                        }
                    }
                    else {
                        addValue(DARBS_FALSE, responsePattern);
                    }
                }
            }
        }
        else {
            xml_node pattern = partition.child("pattern");
            if (pattern.empty()) {
                // This shouldn't happen if the XML makes sense
                LOG("No pattern provided provided with ? as the partition name",
                    ERROR);
                addValue(DARBS_FALSE, responsePartition);
                continue;
            }

            int limit = request.attribute("limit").as_int();

            for (pattern; pattern;
                 pattern = pattern.next_sibling("pattern")) {
                // Add a pattern to the response
                xml_node responsePattern = addPattern(responsePartition);

                bool haveMatch;

                xml_node bbPartition = blackboard.child("partition");
                for (bbPartition; bbPartition;
                     bbPartition = bbPartition.next_sibling("partition")) {
                    MultiVariableMap matches;
                    if (getMatchesOnPartition(pattern, bbPartition, matches,
                                              limit)) {
                        haveMatch = true;
                        MultiVariableMap::iterator it = matches.begin();
                        for (it; it != matches.end(); it++) {
                            xml_node responseKey = getKey(it->first,
                                                          responsePattern);
                            std::vector<std::string>::iterator jt = it->second.begin();
                            for (jt; jt != it->second.end(); jt++) {
                                xml_node responseMatch = addMatch(responseKey);
                                addValue(*jt, responseMatch);
                            }
                        }
                    }
                }
                if (!haveMatch) {
                    addValue(DARBS_FALSE, responsePattern);
                }
            }
        }        
    }
}

/**
 * @brief Clear partitions, or the whole blackboard
 * 
 * Accepts request contents in the following forms:
 * - No contents Clears the entire blackboard, and returns true if sucessful
 * - <partition name="pname" /> Clears the contents of a partition, returning
 *   true if the partition is found, false otherwise.
 * 
 * @param request An xml node containing the partitions to clear
 * @param response An xml node containing the success of the various requests
 * @todo Check pugixml to see whether we can clean up the allocated memory here
 */
void Blackboard::clear(xml_node &request, xml_node &response,
                       xml_node &notification) {
    std::set<std::string> changedPartitions;
    xml_node partition = request.child("partition");
    if (partition.empty()) {
        // No partitions given, so recreate the entire blackboard
        // Have a feeling this leaks all memory allocated for the nodes so far,
        // but that may be unavoidable.
        xml_node bbPartition = blackboard.child("partition");
        for (bbPartition; bbPartition;
             bbPartition=bbPartition.next_sibling("partition")) {
            std::string pn = bbPartition.attribute("name").value();
            changedPartitions.insert(pn);
        }
        createBlackboard();
        addValue(DARBS_TRUE, response);
    }

    for (partition; partition; partition = partition.next_sibling("partition")) {
        // Get the name of the partition
        std::string partitionName = partition.attribute("name").value();
        // Find the corresponding partition on the blackboard
        xml_node bbPartition = getPartition(partitionName);

        // Add the partition to the response
        xml_node responsePartition = addPartition(partitionName, response);


        if (!bbPartition.empty()) {
            // Remove all patterns on the partition
            xml_node pattern = bbPartition.child("pattern");
            for (pattern; pattern; pattern = pattern.next_sibling("pattern")) {
                bbPartition.remove_child(pattern);
            }
            addValue(DARBS_TRUE, responsePartition);
            changedPartitions.insert(partitionName);
        }
        else {
            addValue(DARBS_FALSE, responsePartition);
        }
    }

    std::set<std::string>::iterator it = changedPartitions.begin();
    for (it; it!=changedPartitions.end(); it++) {
        addPartition(*it, notification);
    }
}

/**
 * @brief Clears the blackboard and creates new empty partitions.
 */
void Blackboard::createBlackboard()
{
    blackboardDoc.load("<blackboard></blackboard>");
    blackboard = blackboardDoc.child("blackboard");
}

/**
 * @brief Finds a partition with the matching name on the blackboard
 * @param name The name of the partition
 */
xml_node Blackboard::getPartition(std::string name) {
    return(blackboard.find_child_by_attribute("partition", "name",
           name.c_str()));
}

/**
 * @brief Gets a key in a pattern, creating it if it doesn't exist
 * @param name The name of the key
 * @param parent The parent pattern to add the key to
 */
xml_node Blackboard::getKey(std::string name, xml_node &parent) {
    xml_node key = parent.find_child_by_attribute("key", "name", name.c_str());
    if (key.empty()) {
        key = parent.append_child();
        key.set_name("key");
        key.append_attribute("name").set_value(name.c_str());
    }
    return(key);
}

/**
 * @brief Add a match to a key node
 * @param parent The parent key to add the match to
 */
xml_node Blackboard::addMatch(xml_node &parent) {
    xml_node match = parent.append_child();
    match.set_name("match");
    return(match);
}

/**
 * @brief Removes a partition from the blackboard
 * @param partition An xml node containing the partition
 */
void Blackboard::deletePartition(const xml_node &partition) {
    blackboard.remove_child(partition);
}

/**
 * @brief Deletes matches to a pattern from a partition
 * @param pattern An xml node containing the pattern
 * @param partition An xml node containing the partition
 * @param limit The maximum number of patterns to delete
 * @return Whether any patterns were deleted
 */
bool Blackboard::deletePatternFromPartition(const xml_node &pattern,
                                            xml_node &partition, int limit) {
    int count = 0; // How many patterns have been deleted

    xml_node bbPattern = partition.child("pattern");
    for (bbPattern; bbPattern; bbPattern = bbPattern.next_sibling("pattern")) {
        if (isInstance(bbPattern.child_value(), pattern.child_value())) {
            partition.remove_child(bbPattern);
            count += 1;
            if (count == limit && limit != 0) {
                break;
            }
        }
    }
    return(count > 0);
}

/**
 * @brief Replaces matches to a pattern from a partition
 * @param findPattern An xml node containing the pattern to find
 * @param replacePattern An xml node containing the pattern to replace matches
 * with
 * @param partition An xml node containing the partition
 * @param limit The maximum number of patterns to replace
 * @return Whether any patterns were replaced
 */
bool Blackboard::replacePatternOnPartition(const xml_node &findPattern,
                                           const xml_node &replacePattern,
                                           xml_node &partition, int limit) {
    int count = 0; // How many patterns have been replaced

    xml_node bbPattern = partition.child("pattern");
    for (bbPattern; bbPattern; bbPattern = bbPattern.next_sibling("pattern")) {
        if (isInstance(bbPattern.child_value(), findPattern.child_value())) {
            // Remove the existing PCDATA/CDATA
            bbPattern.remove_child(bbPattern.first_child());
            bbPattern.append_child(node_pcdata).set_value(replacePattern.child_value());
            count += 1;
            if (count == limit && limit != 0) {
                break;
            }
        }
    }
    return(count > 0);
}
            
/**
 * @brief Checks whether a matching pattern exists on a partition
 * @param pattern An xml node containing the pattern to find
 * @param partition An xml node containing the partition
 * @return Whether the pattern was found
 */
bool Blackboard::patternOnPartition(const xml_node &pattern,
                                    const xml_node &partition) {
    xml_node bbPattern = partition.child("pattern");
    for (bbPattern; bbPattern; bbPattern = bbPattern.next_sibling("pattern")) {
        if (isInstance(bbPattern.child_value(), pattern.child_value())) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Get matching patterns from a partition on the blackboard
 *
 * @param pattern An xml node containing the pattern to find
 * @param partition An xml node containing the partition to search
 * @param matches A map where matches to wildcards are placed
 * @param limit The maximum number of patterns to match
 */
bool Blackboard::getMatchesOnPartition(const xml_node &pattern,
                                       const xml_node &partition,
                                       MultiVariableMap &matches,
                                       int limit) {
    int count = 0;
    xml_node bbPattern = partition.child("pattern");
    for (bbPattern; bbPattern; bbPattern = bbPattern.next_sibling("pattern")) {
        VariableMap singleMatches;
        if (isInstance(bbPattern.child_value(), pattern.child_value(),
                       &singleMatches)) {
            VariableMap::iterator it = singleMatches.begin();
            for (it; it != singleMatches.end(); it++) {
                matches[it->first].push_back(it->second);
            }
            count += 1;
            if (count == limit && limit > 1) {
                break;
            }
        }
    }
    return(count > 0);
}
            
/**  
 * @brief Checks whether a string matches a pattern
 *
 * Works through each token in the expression and matches it against the corresponding pattern token.
 * The pattern can contain the following wildcards,  with the matching values returned as a list from
 * the function.
 * Wildcards supported are:
 * - ? - match a word and return matches
 * - ?? - match one or more and return matches
 * - == - match one or more words, but don't return them.
 *
 * @param expression The string to check
 * @param pattern The pattern to match against
 * @return Whether the string matches the pattern
 *
 * @todo If ever a function needed some test cases...
 */
bool Blackboard::isInstance(std::string pattern, std::string expression,
                            VariableMap *matches) {
    boost::algorithm::trim(expression);
    boost::algorithm::trim(pattern);
    // Trying to do it with regex
    // First we convert the DARBS wildcard syntax to regex syntax, using
    // yet more regex
    // Regex is horrible, but here goes...
    // Replace ??name with (?<name>.*?)
    // .*? matches zero or more of any character, but is non-greedy
    // so doesn't munch through more of the pattern than it needs to.  The
    // extra parentheses make it a subexpression so we can grab the matched
    // value later.  The ?<name> gives the subexpression a name so we can
    // access it more simply
    boost::regex doubleQMMatcher("\\?\\?(\\S+)");
    if (matches) {
        MatchResults matchResults;
        std::string::const_iterator start = expression.begin();
        std::string::const_iterator end = expression.end();
        while (regex_search(start, end, matchResults, doubleQMMatcher)) {
            std::string match(matchResults[0].first + 2, // Skip the ??
                              matchResults[0].second);
            LOG("Found wildcard ??" << match, INFO);
            (*matches)[match.c_str()] = "";
            start = matchResults[0].second;
        }
    }
    expression = boost::regex_replace(expression, doubleQMMatcher,
                                       std::string("(?<$1>.*?)"));
    // Replace ?name with (<?name>\S*)
    // \S* matches zero or more non-whitespace characters.  The extra
    // parentheses make it a subexpression so we can grab the matched value
    // later.  The ?<name> gives the subexpression a name so we can
    // access it more simply.
    // Also note the (?![<\\)]) in the regex - (?!EXPRESSION) is a negative
    // forward lookahead, which makes sure that the next character does not
    // match expression.  We look for the '<' and '\' characters, which follow
    // the '?' characters created in the previous replace.
    boost::regex singleQMMatcher("\\?(?![<\\)])(\\S+)");
    if (matches) {
        MatchResults matchResults;
        std::string::const_iterator start = expression.begin();
        std::string::const_iterator end = expression.end();
        while (regex_search(start, end, matchResults, singleQMMatcher)) {
            std::string match(matchResults[0].first + 1, // Skip the ?
                              matchResults[0].second);
            LOG("Found wildcard ?" << match, INFO);
            (*matches)[match.c_str()] = "";
            start = matchResults[0].second;
        }
    }
    expression = boost::regex_replace(expression, singleQMMatcher,
                                      std::string("(?<$1>\\\\S*)"));
    // Replace == with .*?
    // .*? matches zero or more of any character non-greedily.  This is
    // basically used to munch through anything which we are not looking
    // to return
    expression = boost::regex_replace(expression, boost::regex("=="), std::string(".*?"));

    // We need to match the whole pattern, so add the start and end markers
    expression = "^" + expression + "$";

    // Now we use our newly constructed regex to match the variables in
    // the pattern
    boost::regex re(expression);
    MatchResults matchResults;
    bool haveMatch = boost::regex_match(pattern, matchResults, re);
    if (haveMatch && matches && matches->size()) {
        if (matchResults.size() - 1 != matches->size()) {
            LOG("More matches than variables", WARNING);
        }
        VariableMap::iterator it = matches->begin();
        for (it; it != matches->end(); it++) {
            std::string match(matchResults[it->first.c_str()].first,
                              matchResults[it->first.c_str()].second);
            it->second = match;
            LOG("Found match: " << it->first << " = " << it->second, INFO);
        }
    }
    return (haveMatch);
}

