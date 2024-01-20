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
 * @file TcpServer.hpp 
 * @brief Header for a simple TCP server using boost::asio
 */

#ifndef __TCPSERVER_HPP
#define __TCPSERVER_HPP

// Global includes
#include <string>
#include <set>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

// Local includes
#include "TcpConnection.hpp"

// Defines

// Global variables

// Class definitions


/**
 * @brief A simple TCP server using boost::asio
 *
 * This class provides the means for asyncronous communication via socket
 * and TCP/IP with client applications. It waits for clients to connect to
 * an instance of this class. When the client's connection is accepted, a
 * TcpConnection class is used to handle communication with the client.  
 * The server listens for signals from the TcpConnection indicating data
 * has been received, or an error has occurred, and acts accordingly.
 */
class TcpServer {
public:
    // constructor
    TcpServer(boost::asio::io_service& io_service, int port);

    // destructor
    ~TcpServer();
    void broadcast(std::string message, TcpConnection::Pointer avoidConnection);
    boost::signal<void(std::string, TcpConnection::Pointer)> readSignal;

protected:

private:
    void startAccept();
    void handleAccept(TcpConnection::Pointer conn,
                      const boost::system::error_code& error);
    void readHandler(std::string data, TcpConnection::Pointer connection);
    void errorHandler(const boost::system::error_code& error,
                      TcpConnection::Pointer connection);
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::io_service& ioService;
    std::set<TcpConnection::Pointer> connections;
};

#endif
