//////////////////////////////////////////////////////////////////////////
//
// File: client.cpp
//
//////////////////////////////////////////////////////////////////////////
//
// $Id: darbs_terminal.cpp,v 1.1 2001/04/06 16:17:11 kcpw2 Exp $
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis: 
//
// test application for the LnClient class
// 
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2000/06/30: Lars Nolle: initial implementation
//
//////////////////////////////////////////////////////////////////////////

///////////////////////////// global includes ////////////////////////////
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <boost/thread.hpp>

///////////////////////////// local includes /////////////////////////////
#include "TcpClient.hpp"

///////////////////////////// defines ////////////////////////////////////

///////////////////////////// global variables ///////////////////////////

///////////////////////////// member functions ///////////////////////////

void readCallback(std::string message, TcpClient *client);

int main(int argc, char* argv[]) {   
    std::string message;

    std::string host = "127.0.0.1";
    std::string port = "9734";
    if (argc == 2) {
        host = argv[1];
    }
    if (argc == 3) {
        host = argv[1];
        port = argv[2];
    }

    boost::asio::io_service ioService;
    TcpClient client(ioService, host, port);

    client.readSignal.connect(boost::bind(readCallback, _1, _2));

    std::cout<<"Simple TCP/IP Terminal"<<std::endl;
    std::cout<<"======================"<<std::endl;

    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService));


    while( message != "quit")
    {

	    getline(std::cin, message);
	    client.write(message);
    }          
    std::cout<<"stop client"<<std::endl;
    delete &client;
    t.join();
}

void readCallback(std::string message, TcpClient *client) {
    std::cout << message << std::endl;
}

// Prevent multiple definitions of _tls_used
extern "C" void tss_cleanup_implemented(void) {}


