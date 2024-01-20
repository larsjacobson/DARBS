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
 * @file KnowledgeSource.cpp
 */

// Global includes
#include <iostream>

//Local includes
#include "KnowledgeSource.hpp"
#include "Logger.hpp"
#include "Util.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
KnowledgeSource::KnowledgeSource(TcpClient &client,
                                 boost::filesystem::path path)
    :AbstractConditional(client, path) {
}

/**
 * @brief Destructor.
 */
KnowledgeSource::~KnowledgeSource() {
}

/**
 * @brief Call a function contained in an external shared library
 *
 * @param libraryName The name of the library
 * @param functionName The name of the function
 * @param arg An argument to pass to the function
 * @return The return value of the function
 */
std::string KnowledgeSource::call(std::string libraryName,
                                  std::string functionName,
                                  std::string arg) {
    LibraryMap::iterator it = libraries.find(libraryName);
    if (it == libraries.end()) {
        libraries[libraryName] = new SharedObject(libraryName);
        it = libraries.find(libraryName);
    }
    LOG("Calling function " << functionName << " with argument " << arg, INFO);

    std::string returnValue = it->second->call(functionName, arg);

    LOG("Function returned " << returnValue, INFO);
    return(returnValue);
}

/**
 * @brief Set the state of the KS to control whether it runs
 * @param s The state to put the KS into
 */
void KnowledgeSource::setState(State s) {
    boost::mutex::scoped_lock lock(stateMutex);
    state = s;
}

/**
 * @brief Get the current state of the KS
 * @return The state of the KS
 */
State KnowledgeSource::getState() {
    boost::mutex::scoped_lock lock(stateMutex);
    return(state);
}

/**
 * @brief Switches a flag indicating that we may need to change state
 *
 * When the flag is set the KS checks the blackboard at the end of its run loop
 * to see whether there is a state change pattern
 *
 * @param check Whether the flag should be set or not
 */
void KnowledgeSource::setNeedStateCheck(bool check) {
    boost::mutex::scoped_lock lock(stateMutex);
    needStateCheck = check;
}

/**
 * @brief Get the value of a flag showing that we should look for a state change
 * @return The value of the flag
 */
bool KnowledgeSource::getNeedStateCheck() {
    boost::mutex::scoped_lock lock(stateMutex);
    return(needStateCheck);
}

/**
 * @brief Look for a state change message, and change the state if necessary
 *
 * First checks for the state change flag, which is set if the KSs info
 * partition has changed on the blackboard.  If this is set, then requests the
 * value of the state pattern, and changes the state if it has changed
 */
void KnowledgeSource::watchState() {
    if (getNeedStateCheck()) {
        State bbState = (State) requestState();
        if (bbState != state) {
            LOG("External state change: " << state << " to "<< bbState, INFO);
            setState(bbState);
        }
        setNeedStateCheck(false);
    }
}

/**
 * @brief Gets the state of the KS from the blackboard
 *
 * If we should change state the blackboard will have a different value to
 * the one stored in the state variable
 * @return The state on the blackboard
 */
State KnowledgeSource::requestState() {
    xml_document requestDoc;
    xml_node request = addRequest("get", requestDoc);
    xml_node partition = addPartition("_ks_" + name + "_info", request);
    xml_node pattern = addPattern(partition);
    addValue("State is ?state", pattern);
    xml_node response = sendRequest(request);
    // Grab the state from the response
    partition = response.child("partition");
    pattern = partition.child("pattern");
    xml_node key = pattern.child("key");
    xml_node match = key.child("match");
    return((State) stoi(match.child_value()));
}

/**
 * @brief Handle notification messages received from the blackboard
 *
 * Used to check whether due to changes on the blackboard we need to change
 * state or unpause to re-evaluate conditions
 */
void KnowledgeSource::handleNotification(xml_node &notification) {
    // Check to see whether we need to look at our state
    std::string partitionName = "_ks_" + name + "_info";
    xml_node partition = notification.find_child_by_attribute("partition",
                                                              "name",
                                                      partitionName.c_str());
    if (!partition.empty()) {
        setNeedStateCheck(true);
    }


    if (getState() == PAUSED) {
        boost::mutex::scoped_lock lock(stateMutex);
        xml_node partition = notification.child("partition");
        for (partition; partition;
             partition = partition.next_sibling("partition")) {
            std::string partitionName = partition.attribute("name").value();
            if (precondition.partitionWatched(partitionName)) {
                LOG("Unpausing", INFO);
                lock.unlock();
                setState(RUNNING);
                break;
            }
        }
    }

    if (getState() == RUNNING) {    
        boost::mutex::scoped_lock lock(stateMutex);
        xml_node partition = notification.child("partition");
        for (partition; partition;
             partition = partition.next_sibling("partition")) {
            std::string partitionName = partition.attribute("name").value();
            changedPartitions.insert(partitionName);
        }
    }
        
}

/**
 * @brief Add a pattern to the _ks partition indicating that the KS exists
 */
void KnowledgeSource::notifyExistance() {
    xml_document requestDoc;
    xml_node request = addRequest("add", requestDoc);
    xml_node partition = addPartition("_ks", request);
    xml_node pattern = addPattern(partition);
    xml_node value = addValue(name, pattern);

    xml_node response = sendRequest(request);
}

/**
 * @brief Add a pattern to the blackboard indicating the current KS state
 */
void KnowledgeSource::notifyState() {
    xml_document requestDoc;
    xml_node request = addRequest("replace", requestDoc);
    xml_node partition = addPartition("_ks_" + name + "_info", request);
    xml_node findPattern = addPattern(partition);
    addValue("State is ?state", findPattern);
    xml_node replacePattern = addPattern(partition);
    addValue("State is " + itos(state), replacePattern);
    
    xml_node response = sendRequest(request);
}

/**
 * @brief Add a pattern to the blackboard giving the times the KS has fired
 */
void KnowledgeSource::notifyTimesFired() {
    xml_document requestDoc;
    xml_node request = addRequest("replace", requestDoc);
    xml_node partition = addPartition("_ks_" + name + "_info", request);
    xml_node findPattern = addPattern(partition);
    addValue("Fired ?times times", findPattern);
    xml_node replacePattern = addPattern(partition);
    addValue("Fired " + itos(timesFired) + " times", replacePattern);

    xml_node response = sendRequest(request);
}
