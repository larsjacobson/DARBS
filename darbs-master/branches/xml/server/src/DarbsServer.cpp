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
 * @file DarbsServer.cpp
 * @brief The main DARBS server application
 */

// Global includes

#include <iostream>
#include <cstdlib>
#include <boost/bind.hpp>

//Local includes

#include "DarbsServer.hpp"
#include "Logger.hpp"
#include "DarbsCmds.hpp"
#include "Util.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Callback function to be called when data is read from a client.
 *
 * Passes data read om the client to the blackboard for processing.  If the
 * blackboard changes a partition, a message is broadcast to the other clients,
 * and a confirmation message is sent to the original client.  Otherwise the
 * return value from the blackboard is sent to the client.
 *
 * @param data STL string containing the new data
 * @param connection Pointer to the client's connection 
 */
void DARBSserver::readCallback(std::string data,
                               TcpConnection::Pointer connection) {
    // what have we got?
    LOG("Received request: "<< data, INFO);
    xml_document requestDoc;
    requestDoc.load(data.c_str());
    xml_document responseDoc;
    xml_document notificationDoc;


    // if 'quit received, close port and quit program
    if( data == "quit" )
    {
        delete &server;
    	exit(0);
    }

    // otherwise send data to blackboard for parsing and processing
	blackboard.process(requestDoc, responseDoc, notificationDoc);

    // Send the response to the requesting client
    connection->write(docToString(responseDoc));

    // If necessary notify the other clients about partition changes
    if (!notificationDoc.child("notification").child("partition").empty()) {
        server->broadcast(docToString(notificationDoc), connection);
    }
}

/**
 * @brief Constructor and main loop for the DARBSserver class.
 *
 * Creates the TCP server and starts it running, attaching a callback for
 * when new data arrives.  Blocks until exit.
 */
DARBSserver::DARBSserver(int argc, char *argv[]) {
    boost::program_options::options_description description("Options");
    description.add_options()
        ("help", "Show this help message")
        ("port,p",
         boost::program_options::value<int>()->default_value(9734),
         "Port number to listen on")
        ("log-level,l",
         boost::program_options::value<int>()->default_value(WARNING));

    boost::program_options::command_line_parser parser(argc, argv);
    parser = parser.options(description);
    boost::program_options::store(parser.run(), options);
    boost::program_options::notify(options);

    if (options.count("help")) {
        std::cout << "Usage: blackboard [options]" << std::endl;
        std::cout << description << std::endl;
        exit(0);
    } 

    Logger::getInstance()->setLevel(options["log-level"].as<int>());

    boost::asio::io_service ioService;
    server = new TcpServer(ioService, options["port"].as<int>());
    server->readSignal.connect(boost::bind(&DARBSserver::readCallback, this,
                                           _1, _2));

    LOG("DARBS Blackboard", REPORT);
    LOG("Listening on port " << options["port"].as<int>(), REPORT);
    // Start the main loop
    ioService.run();

    // close DARBS port
    delete &server;
}

/**
 * @brief Empty destructor for the DARBSserver class.
 */
DARBSserver::~DARBSserver()
{
    //do something useful here!
}


int main(int argc, char *argv[]) {
    DARBSserver(argc, argv);
}

