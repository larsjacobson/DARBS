//////////////////////////////////////////////////////////////////////////
//
// File: TcpConnection.hpp 
//
//////////////////////////////////////////////////////////////////////////
//
// $Id: LnTcpServer.hpp,v 1.1 2001/04/06 15:46:57 kcpw2 Exp $
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis: 
//
// Defininition file for TcpConnection
// 
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2010/02/03: Joe Turner: Initial implementation
//
//////////////////////////////////////////////////////////////////////////
#ifndef __TCPCONNECTION_HPP
#define __TCPCONNECTION_HPP

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <boost/asio.hpp>
#include <boost/signal.hpp>

///////////////////////////// local includes /////////////////////////////

///////////////////////////// defines ////////////////////////////////////

///////////////////////////// global variables ///////////////////////////

const char ETB = (char) 23;

///////////////////////////// class definitions //////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// TcpConnection
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis:  
//
// This class allows asynchronous communication with a client over a
// TCP/IP socket.  It reads data from the socket until an ETB character
// is encountered, at which point it signals that a block of data has
// been read.  At any point data can be written to the client.
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2000/07/07: Lars Nolle: initial implementation
//
//////////////////////////////////////////////////////////////////////////
class TcpConnection {
public:
    // Constructor
    TcpConnection(boost::asio::io_service& io_service);
    // destructor
    ~TcpConnection();

    void write(std::string message);
    void read();
    boost::asio::ip::tcp::socket& getSocket();
    boost::signal<void(std::string, TcpConnection*)> readSignal;
    boost::signal<void(const boost::system::error_code& error,
                       TcpConnection*)> errorSignal;
protected:

private:
    void readHandler(const boost::system::error_code& error,
                     std::size_t transferred);
    void writeHandler(const boost::system::error_code& error,
                      std::size_t transferred);
    boost::asio::streambuf data;
    boost::asio::ip::tcp::socket socket;
};


#endif
