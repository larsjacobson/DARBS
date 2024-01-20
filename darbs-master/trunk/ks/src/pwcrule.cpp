/***************************************************************************
                          pwcrule.cpp  -  description
                             -------------------
    begin                : Wed Jun 7 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <iostream>
#include <stdio.h>

///////////////////////////// local includes /////////////////////////////
#include "LnTokenizer.hpp"
#include "pwcrule.hpp"

pwCRule::pwCRule(){
	m_bFired = false;
	m_sRuleFileDirectory = "./";
}
pwCRule::~pwCRule(){
}
/** return the rule string */
std::string pwCRule::GetRuleFileName(){
 return m_sRuleFileName;
}
/** Set the rule file name. */
int pwCRule::SetRuleDirectoryName(std::string str){
	m_sRuleFileDirectory = str;
	return 0; // no error.
}
/** Set the rule file name. */
int pwCRule::SetRuleFileName(std::string str){
	m_sRuleFileName = str;
	// std::cout << "RuleName = " << m_sRuleName << std::endl;
	return 0; // no error.
}
/** function that extract the rule variables */
int pwCRule::CheckKeyWords(){
	LnTokenizer Tokenizer(m_sRuleString);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string	Token, sTemp;
	while ((Token = Tokenizer.getNextToken())!= LNVOID) { // if not empty then do.
		if (Token.substr(0,2) == "/*") {
			// Token is a comment, simply ignore it.
		}
		else if (Token == "RULE") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sRuleName = sTemp;
		}
		else if (Token == "IF") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sPreconditionString = sTemp.substr(1,(sTemp.length())-2);
		}
		else if (Token == "THEN") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sActionString = sTemp.substr(1,(sTemp.length())-2);
		}
		else if (Token == "BECAUSE") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sExplainString = sTemp.substr(1,(sTemp.length())-2);
		}
		else if (Token == "END") {
			return 0;
		}
		else {
			std::cout << "Tonken error. Check the rule file." << std::endl;
			return 1; // unknown tonken error.
		}
	} // while
	std::cout << "WARNING: the key word END is missing" << std::endl;
	return 2; // key word "END" is missing.
}
/** Load rule file into the object. */
int pwCRule::LoadRuleFile(){
  char ch;				/* Character read from the file.	*/
	char buffer[65536]; // file buffer, max size 64K.
	int index = 0;
  FILE *ptr;			/* Pointer to the file. FILE is a structure  defined in <stdio.h>	*/
	std::string sTemp;
	
	/* Open the file */
	sTemp = m_sRuleFileDirectory + m_sRuleFileName + ".drf";
	std::cout << "file openned: " << sTemp.c_str() << std::endl;
  ptr = fopen(sTemp.c_str(),"r");
  if (ptr == NULL) return 1; // errors.
	/* Read one character at a time, checking
	for the End of File. EOF is defined
	in <stdio.h>  as -1 			*/
	m_sRuleString.erase();
  while ((ch = fgetc(ptr)) != EOF)
  {
			buffer[index] = ch;
			index++;
  }
  buffer[index] = 0; // add null terminating character to the string.
	m_sRuleString = buffer;

  fclose(ptr);			/* Close the file.			*/
	return 0;
}
/** this function print the rule variables on the screen. 

 */
int pwCRule::PrintRule(){
	std::cout << "RULE = " << m_sRuleName << std::endl;
	std::cout << "PRECONDITION = " << m_sPreconditionString << std::endl;
	std::cout << "ACTION = " << m_sActionString << std::endl;
	std::cout << "EXPLAINATION = " << m_sExplainString << std::endl;
	return 0;
}
/** function that fires the rule */
int pwCRule::FireRule(){
	int i, nInstantiations;
	bool bKSFired;
	std::string sTemp, sMessage;
	if (m_pKS != NULL) {
		if ( m_pKS->m_nDebug >= 1){
			std::cout << "Start firing rule..." << std::endl;
			std::cout << "Rule to be investigated: " << m_sRuleName << std::endl;
		}
		nInstantiations = m_pKS->CreateInstantiations(m_sPreconditionString);	
		m_bFired = false;
		for (i=0; i<nInstantiations; i++) {
      // m_nStatementIndex should be set before this EvaluatePrecondition() is called.
      m_pKS->m_nStatementIndex = 0;
			bKSFired = m_pKS->EvaluatePrecondition(m_sPreconditionString,i);
			if (bKSFired) {
				m_pKS->EvaluateAction(m_sActionString, i);
 				sMessage = m_sRuleName + " has been fired because :";
				m_pKS->DisplayText(sMessage);
// 				std::cout << m_sRuleName << " has been fired because :" << std::endl;
				sTemp = m_pKS->EvaluateActionContent(m_sExplainString, i);
//				std::cout << sTemp << std::endl << std::endl;
				sTemp = sTemp + "\n";
    		m_pKS->DisplayText(sTemp);
 			}
 			m_bFired = (m_bFired || bKSFired);
			if ( m_pKS->m_nDebug >= 1){
				std::cout << "is any instant of the rule fired?" << m_bFired << std::endl;
			}
 		} // for i
 	}
 	else {
 		std::cout << "fatal error : Rule is not linked to KS";
 		return 1;
 	}
	if ( m_pKS->m_nDebug >= 1){
		std::cout << "Done firing rule." << std::endl;
	}
	return 0; // no error.
}
