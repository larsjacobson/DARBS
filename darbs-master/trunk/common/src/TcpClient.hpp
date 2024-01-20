/////////////////////////////////////////////////////////////////////////
//
// File: TcpClient.hpp
//
//////////////////////////////////////////////////////////////////////////
//
// $Id: LnTcpServer.hpp,v 1.1 2001/04/06 15:46:57 kcpw2 Exp $
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis: 
//
// Defininition file for TcpClient file
// 
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2010/02/11: Joe Turner: Initial implementation
//
//////////////////////////////////////////////////////////////////////////
#ifndef __TCPCLIENT_HPP
#define __TCPCLIENT_HPP

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <boost/asio.hpp>
#include <boost/signals.hpp>

///////////////////////////// local includes /////////////////////////////

///////////////////////////// defines ////////////////////////////////////

///////////////////////////// global variables ///////////////////////////
const char ETB = (char) 23;
///////////////////////////// class definitions //////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// TcpClient
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis:  
//
// This class provides the means for communication via socket and TCP/IP with
// the DARBS blackboard. It connects to the server, and listens for messages,
// signalling when it receives data.
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2010/02/05: Joe Turner: initial implementation
//
//////////////////////////////////////////////////////////////////////////

class TcpClient
{
public:
    // constructor
    TcpClient(boost::asio::io_service& ioService);
    TcpClient(boost::asio::io_service& ioService, std::string host, std::string port);

    // destructor
    ~TcpClient();
    void connect(std::string host, std::string port);
    void read();
    void write(std::string message);
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
};

#endif
