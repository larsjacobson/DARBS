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
 * @file DarbsServer.hpp
 * @brief Definition of DARBServer class
 */

#ifndef __DARBS_SERVER_HPP
#define __DARBS_SERVER_HPP

//Global includes

#include <string>
#include <boost/program_options.hpp>

// Local includes 

#include "Blackboard.hpp"
#include "TcpConnection.hpp"
#include "TcpServer.hpp"

// Defines 

// Global variables

// Member classes

/**
 * @brief The main server component of darbs
 *
 * Consists of a TCP/IP server object and a Blackboard object. A member
 * function acts as a callback that is activated by the server object if new
 * data has arrived.
 */
class DARBSserver
{
public:
    // constructor
    DARBSserver(int argc, char *argv[]);

    // destructor
    ~DARBSserver();
protected:

private:
    // TCP/IP server object
    TcpServer *server;
    boost::asio::io_service ioService;

    boost::program_options::variables_map options;

    // blackboard object
    Blackboard blackboard;

    void readCallback(std::string data, TcpConnection::Pointer connection);
};

#endif
