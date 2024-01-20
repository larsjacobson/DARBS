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
 * @file TcpClient.cpp
 * @brief A simple TCP client using boost::asio
 */

// Global includes
#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <cstdlib>

// Local includes
#include "TcpClient.hpp"
#include "Logger.hpp"

// Namespaces

// Defines

// Global variables

// Member functions

/**
 * @brief Constructor
 * 
 * @param ioService The io_service to use for the client's communication
 */
TcpClient::TcpClient(boost::asio::io_service& ioService)
    : ioService(ioService), socket(ioService), resolver(ioService) {
    error = false;
}
    

/**
 * @brief Constructor
 * 
 * @param ioService The io_service to use for the client's communication
 * @param host The IP address to connect to
 * @param port The port number to connect to
 */
TcpClient::TcpClient(boost::asio::io_service& ioService, std::string host, std::string port) : ioService(ioService), socket(ioService), resolver(ioService) {
    connect(host, port);
    read();
}

/**
 * @brief Destructor
 */
TcpClient::~TcpClient() {
    socket.close();
}

/**
 * @brief Connect to a given host
 *
 * @param host The IP address of the host
 * @param The port number to connect to
 * @return Whether connecting was sucessful
 */
bool TcpClient::connect(std::string host, std::string port) {
    LOG("Connecting to " << host << ":" << port, INFO);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && it != end) {
        socket.close();
        socket.connect(*it++, error);
    }
    if (error) {
        LOG("Could not connect to DARBS server", ERROR);
        return(false);
    }
    return(true);
}

/**
 * @brief Read from the socket until an ETB occurs
 */
void TcpClient::read() {
    boost::asio::async_read_until(socket, data, ETB,
                                  boost::bind(&TcpClient::readHandler, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
}


/**
 * @brief Sends a signal if data was received, or if an error occurred
 *
 * Callback from the read function
 *
 * @param error An error code if the read was unsucessful
 * @param transferred The number of bytes transferred
 */
void TcpClient::readHandler(const boost::system::error_code& error,
                            std::size_t transferred) {
    if (error) {
        LOG(error.message(), ERROR);
        setErrorFlag();
        // Need to send the read signal so we don't end up stuck waiting for
        // the response
        readSignal("<error />", this);
    }
    else {
        // Convert from streambuf to string
        std::string sData;
        std::istream isData(&data);
        std::getline(isData, sData, ETB);
        readSignal(sData, this);
        read();
    }
}

/**
 * @brief Write a string to the server
 * 
 * @param message String to send to the server
 */
void TcpClient::write(std::string message) {
    boost::asio::async_write(socket, boost::asio::buffer(message + ETB),
                             boost::bind(&TcpClient::writeHandler, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

/**
 * @brief Write callback - exits if there was an error writing
 *
 * @param error Error number
 * @param transferred The number of bytes transferred
 */
void TcpClient::writeHandler(const boost::system::error_code& error_code,
                             std::size_t transferred) {
    if (error_code) {
        LOG(error_code.message(), ERROR);
        setErrorFlag();
    }
}

bool TcpClient::isError() {
    boost::mutex::scoped_lock lock(errorMutex);
    return error;
}

void TcpClient::setErrorFlag() {
    boost::mutex::scoped_lock lock(errorMutex);
    error = true;
}

