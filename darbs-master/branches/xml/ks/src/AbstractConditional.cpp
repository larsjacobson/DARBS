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
 * @file AbstractConditional.cpp
 */

// Global includes
#include <iostream>
#include <boost/bind.hpp>

//Local includes

#include "AbstractConditional.hpp"
#include "Logger.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
AbstractConditional::AbstractConditional(TcpClient &client,
                                         boost::filesystem::path path)
    :precondition(this), action(this), client(client), path(path),
     timesFired(0)  {
    readConnection = client.readSignal.connect(boost::bind(                                                                 &AbstractConditional::readCallback,
                                            this, _1, _2));
    name = path.filename();
    name = name.substr(0, name.rfind("."));
}

/**
 * @brief Destructor.
 */
AbstractConditional::~AbstractConditional() {
}

/**
 * @brief Parse anxml document containing the most basic type of conditional
 *
 * @param acDoc The xml document to parse
 *
 * @todo Check all the attributes exist, give defaults if possible
 */
void AbstractConditional::parse(xml_document &acDoc) {
    xml_node ac = *(acDoc.begin());

    xml_node preconditionNode = ac.child("precondition");
    for (preconditionNode; preconditionNode;
         preconditionNode = preconditionNode.next_sibling("precondition")) {
        precondition.addChild(preconditionNode);
    }

    xml_node actionNode = ac.child("action");
    for (actionNode; actionNode;
         actionNode = actionNode.next_sibling("action")) {
        action.addChild(actionNode);
    }
}

/**
 * @brief A callback used when data is received for the blackboard
 *
 * Loads the data received from the blackboard into a response, and notifies
 * the thread which is waiting for the response that it has arrived
 *
 * @param buffer A string containing the response obtained from the blackboard
 * @param client The TcpClient which got the data
 */
void AbstractConditional::readCallback(std::string buffer, TcpClient *client) {
    LOG("Received response: " << buffer, INFO);
    responseDoc.load(buffer.c_str());

    std::string responseType = responseDoc.first_child().name();
    if (responseType == "notification") {
        notification = responseDoc.first_child();
        handleNotification(notification);
        return;
    }

    // Signal to sendRequest that we have the data, and it can stop waiting
    // Need to use a copy of the response, so it isn't overwritten if a
    // notification comes while readCondition is being signalled
    response = responseDoc.first_child();
    // Remove the old response
    storedResponseDoc.remove_child(storedResponseDoc.first_child());
    storedResponseDoc.append_copy(response);
    response = storedResponseDoc.first_child();
    boost::lock_guard<boost::mutex> lock(readMutex);
    haveResponse = true;
    readCondition.notify_one();
}

/**
 * @brief Send a request to the blackboard, blocking until we have a response
 * 
 * @param request The request to send
 * @return An xml node containing the response
 */
xml_node AbstractConditional::sendRequest(const xml_node &request) {
    haveResponse = false;
    // Convert the xml_node to a string
    xml_document requestDoc;
    requestDoc.append_copy(request);
    std::stringstream outStringStream;
    xml_writer_stream streamWriter(outStringStream);
    requestDoc.save(streamWriter);
    // Write it to the client
    if (!client.isError()) {
        LOG("Sending request: " << outStringStream.str(), INFO);
        client.write(outStringStream.str());
        // Wait for the response
        boost::unique_lock<boost::mutex> lock(readMutex);
        while (!haveResponse) {
            readCondition.wait(lock);
        }
    }
    else {
        responseDoc.load("<error />");
        response = responseDoc.first_child();
    }
    return (response);
}

std::string AbstractConditional::getName() {
    return(name);
}

int AbstractConditional::getTimesFired() {
    return(timesFired);
}
