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
 * @file ProceduralKS.cpp
 */

// Global includes
#include <iostream>
#include <boost/thread/thread.hpp>

//Local includes

#include "ProceduralKS.hpp"
#include "Logger.hpp"
#include "Util.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
ProceduralKS::ProceduralKS(TcpClient &client, boost::filesystem::path path)
    : KnowledgeSource(client, path) {
}

/**
 * @brief Destructor.
 */
ProceduralKS::~ProceduralKS() {
}

/**
 * @brief The procedural KS's main loop
 */
void ProceduralKS::run() {
    notifyExistance();
    notifyInfo();
    bool result;
    bool haveIncremented;
    bool canPause;
    while (!client.isError()) {
        result = false;
        haveIncremented = true;
        canPause = true;
        while (haveIncremented) {
            LOG("Evaluating preconditions", INFO);
            result = precondition.evaluate();
            LOG("Preconditions evaluated " << result, INFO);

            if (result) {
                LOG("Evaluating actions", INFO);
                action.evaluate();
                canPause = false;
                timesFired += 1;
                notifyTimesFired();
            }
    
            LOG("Incrementing variables", INFO);
            haveIncremented = precondition.incrementVariables();
        }
        precondition.reset();
        action.reset();

        if (canPause) {
            LOG("Pausing", INFO);
            setState(PAUSED);
            notifyState();
        }

        do {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
            watchState(); // Make sure we respond to state change patterns
        } while (getState() == PAUSED || getState() == STOPPED);

        if (canPause) {
            // We have paused, but woken up.  Notify!
            notifyState();
        }
    }
    LOG("Exiting KS main loop", INFO);
}

/**
 * @brief Add patterns to the blackboard containing info about the KS
 */
void ProceduralKS::notifyInfo() {
    xml_document requestDoc;
    xml_node request = addRequest("add", requestDoc);
    xml_node partition = addPartition("_ks_" + name + "_info", request);
    xml_node typePattern = addPattern(partition);
    addValue("Procedural Knowledge Source", typePattern);
    xml_node firedPattern = addPattern(partition);
    addValue("Fired " + itos(timesFired) + " times", firedPattern);
    xml_node response = sendRequest(request); 
}

