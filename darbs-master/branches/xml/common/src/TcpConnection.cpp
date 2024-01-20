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
 * @file TcpConnection.cpp
 * @brief A TCP connection to a client
 */


// Global includes

#include <iostream>
#include <boost/bind.hpp>

// Local includes

#include "TcpConnection.hpp"

// Namespaces

// Defines

// Global variables

// Member functions

/**
 * @brief Creates a shared pointer to a new instance
 *
 * @param io_service Socket IO facilitator
 */
TcpConnection::Pointer TcpConnection::create(boost::asio::io_service& io_service) {
    return Pointer(new TcpConnection(io_service));
}

/**
 * @brief Constructor
 *
 * Creates a new TcpConnection ready for use by a client
 *
 * @param io_service Socket IO facilitator
 */
TcpConnection::TcpConnection(boost::asio::io_service& io_service)
    : socket(io_service) {
}

/**
 * @brief Destructor
 */
TcpConnection::~TcpConnection() {
}

/**
 * @brief Write a string to a client
 *
 * @param message String to send to the client
 */
void TcpConnection::write(std::string message) {
    boost::asio::async_write(socket, boost::asio::buffer(message + ETB),
                             boost::bind(&TcpConnection::writeHandler,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

/**
 * @brief Read from the socket until an ETB occurs
 */
void TcpConnection::read() {
    boost::asio::async_read_until(socket, data, ETB,
                                  boost::bind(&TcpConnection::readHandler,
                                              shared_from_this(),
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
}

/**
 * @brief Write callback - signals if there was an error whilst writing
 *
 * @param error Error number
 * @param transferred The number of bytes transferred
 */
void TcpConnection::writeHandler(const boost::system::error_code& error,
                  std::size_t transferred) {
    if (error) {
        errorSignal(error, shared_from_this());
    }
}

/**
 * @brief Read callback - signals if data was received or if an error occurred
 *
 * @param error Error number
 * @param transferred The number of bytes transferred
 */
void TcpConnection::readHandler(const boost::system::error_code& error,
                 std::size_t transferred) {
    if (error) {
        errorSignal(error, shared_from_this());
        return;
    }
    // Convert from streambuf to string
    std::string sData;
    std::istream isData(&data);
    std::getline(isData, sData, ETB);
    readSignal(sData, shared_from_this());
    read();
}

/**
 * @brief Return the socket used by the connection
 *
 * @return Reference to the socket used by the connection
 */
boost::asio::ip::tcp::socket& TcpConnection::getSocket() {
      return socket;
}

