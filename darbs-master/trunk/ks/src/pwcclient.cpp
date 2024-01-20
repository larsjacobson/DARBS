/***************************************************************************
                          pwcclient.cpp  -  description
                             -------------------
    begin                : Thu Jun 8 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

///////////////////////////// global includes /////////////////////////////
#include <iostream>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

///////////////////////////// local includes /////////////////////////////
#include "pwcknowledgesource.hpp"
#include "pwcclient.hpp"
#include "pwcruleks.hpp"
#include "pwcproceduralks.hpp"

///////////////////////////// global functions ////////////////////////////

// check if the given message is a "partition changed".
std::string CB_CheckPartitionChanged(std::string sData) {
	int nSize = sData.length();
	std::string sTemp1, sTemp2, sResult;
	
	sResult.erase();
	if (nSize > 17) {
		sTemp1 = sData.substr(0,9);
		sTemp2 = sData.substr(nSize-8);
		if ( (sTemp1=="partition") && (sTemp2=="changed!") ) {
			sResult = sData.substr(10, nSize-sTemp1.length()-sTemp2.length()-2);
		}
	}
	return sResult;
}

/** Client read callback function */
void pwCClient::readCallback(std::string buffer, TcpClient* client){
	std::string sResult;
	sResult = CB_CheckPartitionChanged(buffer);
	if ( !sResult.empty() ){ // broadcast message
		if (m_pKS->CheckPartitionList(sResult) ) {
			if (m_pKS->m_nDebug >= 1) {
				std::cout << "Changing of " << sResult << "has been identified" << std::endl;
			}
			m_pKS->m_bKSRestart = true;
		}
	}
	else { // KS's own call back message
		m_pKS->m_sCallBackString = buffer;
		m_pKS->m_bReady = true;
	}
	if (m_pKS->m_nDebug >= 1)
		std::cout << "callback: " << buffer << std::endl;
}

pwCClient::pwCClient(int argc, char *argv[]) : client(ioService) {
	Initialise(argc, argv);
}

pwCClient::~pwCClient(){
	DeleteContent();
	m_vArgv.clear();
	// close tcp connection.
	// m_TcpClient.sendData("quit");
	delete &client;

}

/** Display the command line options */
void pwCClient::DisplayHelpInfo(){
	std::cout << "ARBS's parameters are:" << std::endl;
	std::cout << "--help, display this information." << std::endl;
	std::cout << "-KS=filename, where filename is the KS's file name." << std::endl;
	std::cout << "-IP=ip_address, where ip_address is the address for the BB server." << std::endl;
	std::cout << "-debug= 0, no debug information." << std::endl;
	std::cout << "-debug= 1, basic debug information." << std::endl;
	std::cout << "-debug= 2, details debug information." << std::endl;
	exit(1);
}

/** function to make a tcp connection */
int pwCClient::TcpConnect(){
	std::string message;
	int nResult;
	// establish tcp connection to the server.
	client.connect(m_sIP_Address, "9734");
	client.readSignal.connect(boost::bind(&pwCClient::readCallback, this,
                                           _1, _2));
    client.read();
    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService));
}

/** Initialise the DARBS client */
int pwCClient::Initialise(int argc, char *argv[]){
	int i;
	bool bFlag;
	std::string sTemp;
	pwCKnowledgeSource ksTemp;
	
	m_pKS = NULL;
	m_nArgc = argc;
	m_vArgv.clear();
	m_vArgv.push_back(argv[0]);
	bFlag = false;
	if (argc > 1) {
		for (i=1; i<argc; i++) {
			sTemp = argv[i];
			m_vArgv.push_back(sTemp);
			if ( (sTemp == "-h") || (sTemp == "--help") ){
				DisplayHelpInfo();
			}
			else if (sTemp == "-debug=0") {
				ksTemp.m_nDebug = 0;
			}
			else if (sTemp == "-debug=1") {
				ksTemp.m_nDebug = 1;
			}
			else if (sTemp == "-debug=2") {
				ksTemp.m_nDebug = 2;
			}
			else if (sTemp.substr(0,4) == "-KS=") {
				sTemp.erase(0,4);
				ksTemp.m_sKSFileName = sTemp;
				bFlag = true;
				if (ksTemp.m_nDebug >= 1) {
					std::cout << "KS File Name = " << sTemp << std::endl;
				}
			}
			else if (sTemp.substr(0,4) == "-IP=") {
				sTemp.erase(0,4);
				m_sIP_Address = sTemp;
				if (ksTemp.m_nDebug >= 1) {
					std::cout << "IP Address is " << sTemp << std::endl;
				}
			}
			else {
				DisplayHelpInfo();
			} // else
		} // for
	} // if
	else {
		DisplayHelpInfo();
	}
	if (bFlag == true) {
		if (ksTemp.m_nDebug >= 1) {
			std::cout << "ARBS starting..." << std::endl;
		}
		//load the KS files and create KS and rule objects.
		ksTemp.Initialise();
		// create the KS object according to the KS file.
		CreateKS(&ksTemp);
		// setup the TCP connection.
		TcpConnect();
		// start the KS inference
		StartKS();
		if (ksTemp.m_nDebug >= 1) {
			std::cout << "DARBS client exit successfully." << std::endl;
		}
	}
	else {
		DisplayHelpInfo();
	}
  return 0;
}

/** clean up the allocated memory before the object is deleted. */
void pwCClient::DeleteContent(){
	if (m_pKS != NULL) {
		delete m_pKS;
		m_pKS = NULL;
	}
}

/** start proceeding the knowledge source. */
int pwCClient::StartKS(){
	m_pKS->Initialise();
	// start or restart the KS
	m_pKS->m_bKSRestart = false;
	while (m_pKS->m_sFirabilityFlag == "true") {
		m_pKS->Inference();
	 	sleep(1);
	}
	return 0;
}

/** Create the KS object according to the KS file. */
int pwCClient::CreateKS(pwCKnowledgeSource *ks){
	if (ks->m_sKSType == "rule_based_KS") {
		m_pKS = new pwCRuleKS;
	}
	else if (ks->m_sKSType == "procedural_KS"){
		m_pKS = new pwCProceduralKS;
	}
	else {
		std::cout << "Error: The KS_TYPE in KS file is wrong!" << std::endl;
		exit(1);
	}
	m_pKS->m_pClient = this;
	m_pKS->CopyKeyWords(ks);
	return 0;
}

/** A local function for rule files, ask the user a question and request an answer. */
std::string pwCClient::GetTextInput(std::string sPrompt){
	std::string sAnswer;
	
	std::cout << sPrompt << std::endl;
	getline(std::cin,sAnswer);
	return sAnswer;
}

/** a local funcation for displaying message to the output. */
void pwCClient::DisplayText(std::string sMessage){
	std::cout << sMessage << std::endl;
}
