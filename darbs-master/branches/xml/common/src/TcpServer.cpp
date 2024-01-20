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
 * @file TcpServer.cpp
 * @brief A simple TCP server using boost::asio
 */


// Global includes
#include <string>
#include <iostream>
#include <boost/bind.hpp>

// Local includes
#include "TcpServer.hpp"
#include "Logger.hpp"

// Namespaces

// Defines

// Global variables

// Member functions


/**
 * @brief Constructor
 * 
 * Creates the socket manager (ioService) and connection acceptor, and begins
 * accepting connections.
 * 
 * @param ioService Socket IO facilitator
 * @param port The port number to bind to
 */
TcpServer::TcpServer(boost::asio::io_service& ioService, int port)
    : ioService(ioService),
      acceptor(ioService,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)) {
    LOG("Listening on localhost:" << port, INFO);
    startAccept();
}

/**
 * @brief Destructor which destroys any active connections.
 */
TcpServer::~TcpServer() {
    delete &connections;
}

/**
 * @brief Start listening for new connections from clients
 */
void TcpServer::startAccept() {
    TcpConnection::Pointer conn = TcpConnection::create(ioService);
    acceptor.async_accept(conn->getSocket(),
                          boost::bind(&TcpServer::handleAccept, this, conn,
                                      boost::asio::placeholders::error));
}

/**
 * @brief Accept a new connection request, and start reading from the conenction
 *
 * @param conn Pointer to the connection
 * @param Error code if the accept failed
 * @todo Do something with the error rather than just returning
 */
void TcpServer::handleAccept(TcpConnection::Pointer conn,
                             const boost::system::error_code& error) {
    if (error) {
        LOG(error.message(), ERROR);
        return;
    }
    conn->read();
    conn->readSignal.connect(boost::bind(&TcpServer::readHandler, this,
                                         _1, _2));
    conn->errorSignal.connect(boost::bind(&TcpServer::errorHandler, this,
                                          _1, _2));
    connections.insert(conn);
    startAccept();
}

/**
 * @brief Send a message to all connected clients
 *
 * Does not send the message to the connection supplied as avoidConnection
 * (which is generally the client which caused the broadcast in the first place)
 * @param message The message to send to connected clients
 * @param avoidConnection A connection which will not be broadcast to
 */
void TcpServer::broadcast(std::string message,
                          TcpConnection::Pointer avoidConnection) {
    std::set<TcpConnection::Pointer>::iterator i;
    for (i=connections.begin(); i!=connections.end(); i++) {
        if (*i != avoidConnection) {
            (*i)->write(message);
        }
    }
}

/**
 * @brief Broadcasts read signals received from all clients as a single signal
 *
 * @param data Data read from a client
 * @param connection The connection which received the data
 */
void TcpServer::readHandler(std::string data,
                            TcpConnection::Pointer connection) {
    readSignal(data, connection);
}

/**
 * @brief Deletes client connections when an error signal is received
 *
 * @param error The error code
 * @param connection The connection over which the error occurred
 * @todo Check what the error is and handle it more gracefully
 */
void TcpServer::errorHandler(const boost::system::error_code& error,
                             TcpConnection::Pointer connection) {
    connections.erase(connection);
}

