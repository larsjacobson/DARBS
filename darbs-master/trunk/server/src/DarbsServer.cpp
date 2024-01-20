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

// Namespaces

// Defines 

// Global variables

std::string confirmation = "okay";

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
void DARBSserver::readCallback(std::string data, TcpConnection *connection) {
    // what have we got?
    std::cout<<"server received   : "<<data<<std::endl;

    // if 'quit received, close port and quit program
    if( data == "quit" )
    {
	    std::cout<<"Bye bye ..."<<std::endl;
        delete &server;
    	exit(0);
    }

    // otherwise send data to blackboard for parsing and processing
	std::string retval = blackboard.process(data);

    // put answer on screen ...
    std::cout<<"answer from board : "<<retval<<std::endl;

    // if a partition has changed, inform the clients
    if( retval.find("partition") != -1 ) {
	// inform all clients, but the requesting one
    	server->broadcast(retval, connection);

        // send confirmation to requesting client
	    connection->write(confirmation);
    }
    else // otherwise sent answer to requesting client
    {
	    connection->write(retval);
    }
}

/**
 * @brief Constructor and main loop for the DARBSserver class.
 *
 * Creates the TCP server and starts it running, attaching a callback for
 * when new data arrives.  Blocks until exit.
 */
DARBSserver::DARBSserver() { 
    // clear screen
    std::cout<<"\x1B[2J";

    // message for user  
    std::cout<<"DARBS Blackboard Server"<<std::endl;

    // TODO: Print IP and port of server

    boost::asio::io_service ioService;
    server = new TcpServer(ioService, 9734);
    server->readSignal.connect(boost::bind(&DARBSserver::readCallback, this,
                                           _1, _2));
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

