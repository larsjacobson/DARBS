/***************************************************************************
                          pwcruleks.cpp  -  description
                             -------------------
    begin                : Wed Mar 21 2001
    copyright            : (C) 2001 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <stdexcept>

///////////////////////////// local includes /////////////////////////////
#include "pwcruleks.hpp"
#include "LnTokenizer.hpp"

pwCRuleKS::pwCRuleKS(){
}

pwCRuleKS::~pwCRuleKS(){
	// remove dynamically allocated memories for rule objects.
	m_vListOfRules.clear();
}

/** This function initialise the KS object */
int pwCRuleKS::Initialise(){
	CreateRules();
	return 0;
}

/** the function which starts the rule firing procedure. */
int pwCRuleKS::FireRules(){
	int i, nStatementID;
	bool bFired=false;
	
	if (m_nDebug >= 1) {
		std::cout << "KS firing rules... " << std::endl;
	}
	
	nStatementID = m_nStatementID;
  // edited by p.wong on 02-08-2002
  // the while loop is not necessary because the rules in the KS should be fired only once.
	// do {
		m_nStatementID = nStatementID;
		bFired = false;
		for (i=0; i<m_nNoOfRules; i++) {
			m_vListOfRules[i].FireRule();
			bFired = ( bFired || m_vListOfRules[i].m_bFired );
		}
		if (m_nDebug >= 1) {
			std::cout << "a rule fired? " << bFired << std::endl;
		}
	// } while (bFired);
	if (m_nDebug >= 1) {
		std::cout << "KS done firing rules... " << std::endl;
	}
	return 0; // no error
}

/** get no. of rules. */
int pwCRuleKS::GetNoOfRules(){
	return m_nNoOfRules;
}

/** Function that creates rule objects */
int pwCRuleKS::CreateRules(){
	LnTokenizer Tokenizer(m_sRuleString);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string	Token;
	pwCRule Rule;
	m_nNoOfRules = 0;
	
	if ( m_nDebug >= 1){
		std::cout << "Start creating rules" << std::endl;
	}
	while ((Token = Tokenizer.getNextToken())!= LNVOID) { // if not empty then do.
		if ( m_nDebug >= 2){
			std::cout << "Token = " << Token << std::endl;
		}
		// Ignore comments and the keyword AND.
		if ( (Token != "AND") && (Token.substr(0,2) != "/*") ) {
			Rule.SetRuleFileName(Token);
			Rule.SetRuleDirectoryName(m_sKSFileDirectory);
			if ( Rule.LoadRuleFile() ) {
				std::cout << "Error reading rule file!" << std::endl;
				exit (1);
			}
			Rule.CheckKeyWords();
			if ( m_nDebug >= 2)
				Rule.PrintRule();
			Rule.m_pKS = this;
			m_vListOfRules.insert(m_vListOfRules.end(), Rule);
			m_nNoOfRules++;
		}
	} // while
	if ( m_nDebug >= 1){
		std::cout << "Done creating rules" << std::endl;
	}
	return 0;
}

/** the actions provoke when the KS is activated. */
int pwCRuleKS::DoAction(int nInstantiations){
	int nResult;
	// fire all the rules corresponding to this KS.
	nResult = FireRules();
	pwCKnowledgeSource::DoAction(nInstantiations);
	// check for the KS restarting signal.
	if (m_bKSRestart) {
		return 1;
	}
	return nResult;
}

/** Inference engine */
int pwCRuleKS::Inference(){
	int i, nInstantiations;
	bool bShow = false;
	
	m_nStatementID = 0;
	nInstantiations = CreateInstantiations(m_sPreconditionString);	
	// Firing each instantiations
	for (i=0; i<nInstantiations; i++) {
    // m_nStatementIndex should be set before this EvaluatePrecondition() is called.
    m_nStatementIndex = 0;
		m_bKSFired = EvaluatePrecondition(m_sPreconditionString,i);
		if ( m_nDebug >= 1){
			std::cout << "Fire flag = " << m_bKSFired << std::endl;
		}
		// check for the KS restarting signal.
		if (m_bKSRestart) {
			DisplayText("Tasks interruped. KS is now restarted.");
			m_bKSRestart = false;
			break;
		}
		if (m_bKSFired) {
			bShow = true;
			FireRules();
			if ( EvaluateAction(m_sActionString, i) ) {
				std::cout << "error doing action" << std::endl;
				exit (1);
			}
		}
	}
	if (bShow) {
		DisplayText("Tasks Completed. KS is now suspended.");
		bShow = false;
	}
	return 0;
}

