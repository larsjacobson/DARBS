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
 * @file TcpConnection.hpp 
 * @brief Header for a single TCP connection
 */

#ifndef __TCPCONNECTION_HPP
#define __TCPCONNECTION_HPP

// Global includes
#include <string>
#include <boost/asio.hpp>
#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

// Local includes

// Defines

// Global variables

const char ETB = (char) 23;

// Class definitions


/**
 * @brief A TCP connection allowing aynchronous communication with a client
 *
 * This class allows asynchronous communication with a client over a
 * TCP/IP socket.  It reads data from the socket until an ETB character
 * is encountered, at which point it signals that a block of data has
 * been read.  At any point data can be written to the client.
 */
class TcpConnection : public boost::enable_shared_from_this<TcpConnection> {
public:
    typedef boost::shared_ptr<TcpConnection> Pointer;

    static Pointer create(boost::asio::io_service& io_service);

    // destructor
    ~TcpConnection();

    void write(std::string message);
    void read();
    boost::asio::ip::tcp::socket& getSocket();
    boost::signal<void(std::string, TcpConnection::Pointer)> readSignal;
    boost::signal<void(const boost::system::error_code& error,
                       TcpConnection::Pointer)> errorSignal;
protected:

private:
    // Constructor
    TcpConnection(boost::asio::io_service& io_service);

    void readHandler(const boost::system::error_code& error,
                     std::size_t transferred);
    void writeHandler(const boost::system::error_code& error,
                      std::size_t transferred);
    boost::asio::streambuf data;
    boost::asio::ip::tcp::socket socket;
};


#endif
