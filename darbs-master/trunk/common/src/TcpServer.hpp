/////////////////////////////////////////////////////////////////////////
//
// File: TcpServer.hpp 
//
//////////////////////////////////////////////////////////////////////////
//
// $Id: LnTcpServer.hpp,v 1.1 2001/04/06 15:46:57 kcpw2 Exp $
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis: 
//
// Defininition file for TcpServer file
// 
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2010/02/03: Joe Turner: Initial implementation
//
//////////////////////////////////////////////////////////////////////////
#ifndef __TCPSERVER_HPP
#define __TCPSERVER_HPP

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

///////////////////////////// local includes /////////////////////////////
#include "TcpConnection.hpp"

///////////////////////////// defines ////////////////////////////////////

///////////////////////////// global variables ///////////////////////////

///////////////////////////// class definitions //////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// TcpServer
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis:  
//
// This class provides the means for asyncron communication via socket
// and TCP/IP with client applications. It waits for clients to connect to
// an instance of this class. When the client's connection is accepted, a
// TcpConnection class is used to handle communication with the client.  
// The server listens for signals from the TcpConnection indicating data
// has been received, or an error has occurred, and acts accordingly.
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2010/02/05: Joe Turner: initial implementation
//
//////////////////////////////////////////////////////////////////////////

class TcpServer
{
public:
    // constructor
    TcpServer(boost::asio::io_service& io_service, int port);

    // destructor
    ~TcpServer();
    void broadcast(std::string message, TcpConnection *avoidConnection=0);
    boost::signal<void(std::string, TcpConnection*)> readSignal;

protected:

private:
    void startAccept();
    void handleAccept(TcpConnection* conn,
                      const boost::system::error_code& error);
    void readHandler(std::string data, TcpConnection *connection);
    void errorHandler(const boost::system::error_code& error,
                      TcpConnection *connection);
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::io_service& ioService;
    std::vector<TcpConnection*> connections;
};

#endif
