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
 * @file Client.hpp 
 * @brief Header file for a command-line Knowledge Source client
 */

#ifndef __CLIENT_HPP_
#define __CLIENT_HPP

// Global includes 
#include <string>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

// Local includes 
#include "TcpClient.hpp"
#include "KnowledgeSource.hpp"

// Defines 

// Global variables

// Class definitions

/**
 * @brief A command line Knowledge Source client
 * 
 * Creates and runs a KS from an xml file describing its characteristics
 */
class Client {
public:
    // Constructor
    Client(int argc, char *argv[]);

    // Destructor
    ~Client();

    bool connect();

    bool createKS();

    void run();


protected:

private:
    boost::asio::io_service ioService;
    boost::thread *ioThread;
    TcpClient client;
    KnowledgeSource *ks;
    boost::program_options::variables_map options;
};

// Prevent multiple definitions of _tls_used
extern "C" void tss_cleanup_implemented(void) {}

#endif
