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
 * @file AbstractConditional.hpp
 * @brief Header file for a conditional base class
 */

#ifndef __ABSTRACT_CONDITIONAL_HPP
#define __ABSTRACT_CONDITIONAL_HPP

// Global includes
#include <string>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

// Local includes
#include "pugixml.hpp"

#include "TcpClient.hpp"
#include "ClientTypes.hpp"
#include "Precondition.hpp"
#include "Action.hpp"


// Namespace
using namespace pugi;

// Defines

// Type definitions

// Global variables

// Class definitions

/**
 * @brief An abstract class for conditionals (i.e. a precondition/action pair)
 *
 * The AbstractConditional class is used as a base for Knowledge Sources and
 * Rules.  In essence it contains the definitions and common code needed for
 * evaluating a Precondition and calling Actions if it evaluates true.  Also 
 * contains much of the code needed for bidirectional communication with the
 * server.
 */
class AbstractConditional {
public:

    // construction
    AbstractConditional(TcpClient &client, boost::filesystem::path path);

    // destruction
    virtual ~AbstractConditional();
    
    // public access
    virtual void run() {};

    virtual void parse(xml_document &ksDoc);

    void readCallback(std::string buffer, TcpClient *client);

    virtual xml_node sendRequest(const xml_node &request);

    virtual std::string call(std::string libraryName, std::string functionName,
                             std::string arg) {};

    virtual void handleNotification(xml_node &notification) {};

    std::string getName();
    int getTimesFired();

    Precondition precondition;
    Action action;

protected:
    std::string name;
    TcpClient &client;

    boost::signals::connection readConnection;
    boost::mutex readMutex;
    boost::condition_variable readCondition;
    bool haveResponse;
    xml_document responseDoc;
    xml_document storedResponseDoc;
    xml_node response;
    xml_node notification;

    int timesFired;
    boost::filesystem::path path;
private:
};


#endif
