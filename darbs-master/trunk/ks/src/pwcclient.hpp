/***************************************************************************
                          pwcclient.hpp  -  description
                             -------------------
    begin                : Thu Jun 8 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCCLIENT_H
#define PWCCLIENT_H

///////////////////////////// global includes /////////////////////////////
#include <string>
#include <vector>
#include <boost/asio.hpp>

///////////////////////////// local includes /////////////////////////////
#include "pwcknowledgesource.hpp"
#include "LnSharedObjectLoader.hpp"
#include "TcpClient.hpp"

/**class that provides facilities for client applications.
  *@author Patrick Wong
  */

class pwCKnowledgeSource;

class pwCClient {
public: 
	pwCClient(int argc, char *argv[]);
	~pwCClient();
  /** Display the command line options */
  virtual void DisplayHelpInfo();
  /** function to make a tcp connection */
  virtual int TcpConnect();
  /** Initialise the DARBS client */
  virtual int Initialise(int argc, char *argv[]);
  /** clean up the allocated memory before the object is deleted. */
  virtual void DeleteContent();
  /** start proceeding the knowledge source. */
  virtual int StartKS();
  /** create the KS object according to the KS file. */
  virtual int CreateKS(pwCKnowledgeSource *ks);
  /** a local funcation for displaying message to the output. */
  virtual void DisplayText(std::string sMessage);
	/** A local function for rule files, ask the user a question and request an answer. */
  virtual std::string GetTextInput(std::string sPrompt);

  virtual void readCallback(std::string buffer, TcpClient *client);
 protected: // Protected attributes
  /** number of parameters */
  int m_nArgc;
  /** the list of parameters */
  std::vector<std::string> m_vArgv;
 public: // Public attributes
  /** The knowledge sourse that associated with this client. */
  pwCKnowledgeSource* m_pKS;
	/** object that handles the client side tcp communication. */
  boost::asio::io_service ioService; // Needs to be before client so it's initialized
  TcpClient client;
  /** object that handles access of shared library. */
  LnSharedObjectLoader m_soLoader;
  /** the IP address for the BB server */
  std::string m_sIP_Address;
};

#endif
