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
 * @file Client.cpp
 * @brief A command line client for running Knowledge Source
 */

// Global includes 
#include <iostream>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

// Local includes
#include "pugixml.hpp"
#include "Client.hpp"
#include "ProceduralKS.hpp"
#include "RuleBasedKS.hpp"
#include "Logger.hpp"

// Namespaces
using namespace pugi;

// Defines

// Global variables

// Member functions

/**
 * @brief Constructor
 *
 * @param argc Number of command line argument
 * @param Command line arguments
 */
Client::Client(int argc, char *argv[]):client(ioService), ioThread(NULL) {
    boost::program_options::options_description visibleDescription("Options");
    visibleDescription.add_options()
        ("help", "Show this help message")
        ("ip-address,i",
         boost::program_options::value<std::string>()->default_value("127.0.0.1"),
         "IP Address to connect to")
        ("port,p",
         boost::program_options::value<std::string>()->default_value("9734"),
         "Port number to connect to")
        ("log-level,l",
         boost::program_options::value<int>()->default_value(WARNING));

    boost::program_options::options_description hiddenDescription("Hidden");
    hiddenDescription.add_options()
        ("agent-file", boost::program_options::value<std::string>(),
         "Agent file");

    boost::program_options::options_description cliDescription;
    cliDescription.add(visibleDescription).add(hiddenDescription);

    boost::program_options::positional_options_description positionalDescription;
    positionalDescription.add("agent-file", -1);

    boost::program_options::command_line_parser parser(argc, argv);
    parser = parser.options(cliDescription);
    parser = parser.positional(positionalDescription);
    boost::program_options::store(parser.run(), options);
    boost::program_options::notify(options);

    if (options.count("help") || !options.count("agent-file")) {
        std::cout << "Usage: agentclient [options] file.dkf" << std::endl;
        std::cout << visibleDescription << std::endl;
        return;
    }

    Logger::getInstance()->setLevel(options["log-level"].as<int>());

    if (!connect()) {
        return;
    }
    
    if (!createKS()) {
        return;
    }

    ks->run();
}

/**
 * @brief Destructor
 */
Client::~Client() {
    LOG("Stopping IO service", INFO);
    ioService.stop();
    if (ioThread) {
        LOG("Joining IO thread", INFO);
        ioThread->join();
    }
}

/**
 * @brief Read in the KS file and create an appropriate KnowledgeSource object
 */
bool Client::createKS() {
    std::string fileName = options["agent-file"].as<std::string>();

    boost::filesystem::path path = fileName;
    if (!path.is_complete()) {
        // Relative path, so add the filename to the path we are running from
        boost::filesystem::path initialPath = boost::filesystem::initial_path();
        path = initialPath / path;
    }

    xml_document ksDoc;
    xml_parse_result parseResult = ksDoc.load_file(path.string().c_str());
    if (parseResult) {
        std::string ksType = ksDoc.child("agent").attribute("type").value();
        if (ksType == "procedural") {
            ks = new ProceduralKS(client, path);
        }
        else if (ksType == "rule-based") {
            ks = new RuleBasedKS(client, path);
        }
        else {
            // Dunno the type
            LOG("Unknown KS type", ERROR);
            return false;
        }
        ks->parse(ksDoc);
    }
    else {
        LOG("Syntax error in KS file - " << parseResult.description(), ERROR);
        return false;
    }
    return true;
}

/**
 * @brief Connect to the blackboard
 */
bool Client::connect() {
    // establish tcp connection to the server.
    std::string ip = options["ip-address"].as<std::string>();
    std::string port = options["port"].as<std::string>();
    bool connected = client.connect(ip, port);
    if (connected) {
        client.read();
        ioThread = new boost::thread(boost::bind(&boost::asio::io_service::run,
                                     &ioService));
    }
    return(connected);
}

/**
 * @brief Main function
 */
int main(int argc, char *argv[]) {
    Client(argc, argv);
}
