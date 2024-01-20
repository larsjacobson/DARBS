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
 * @file TcpClient.hpp 
 * @brief Header for a simple TCP client using boost::asio
 */
#ifndef __TCPCLIENT_HPP
#define __TCPCLIENT_HPP

// Global includes
#include <string>
#include <boost/asio.hpp>
#include <boost/signals.hpp>
#include <boost/thread.hpp>

// Local includes 

// Defines 

// Global variables 
const char ETB = (char) 23;

// Class definitions


/**
 * @brief A simple TCP client using boost::asio

 * This class provides the means for communication via socket and TCP/IP with
 * the DARBS blackboard. It connects to the server, and listens for messages,
 * signalling when it receives data.
 */
class TcpClient {
public:
    // constructor
    TcpClient(boost::asio::io_service& ioService);
    TcpClient(boost::asio::io_service& ioService, std::string host, std::string port);

    // destructor
    ~TcpClient();
    bool connect(std::string host, std::string port);
    void read();
    void write(std::string message);

    bool isError();
    void setErrorFlag();

    boost::signal<void(std::string, TcpClient *)> readSignal;
protected:

private:
    void readHandler(const boost::system::error_code& error,
                     std::size_t transferred);
    void writeHandler(const boost::system::error_code& error,
                      std::size_t transferred);
    boost::asio::io_service& ioService;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::streambuf data;
    boost::asio::ip::tcp::socket socket;
    bool error;
    boost::mutex errorMutex;

};

#endif
