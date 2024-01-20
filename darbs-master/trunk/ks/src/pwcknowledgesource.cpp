/***************************************************************************
                          pwcknowledgesource.cpp  -  description
                             -------------------
    begin                : Wed Jun 7 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <cstring>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <stdexcept>

///////////////////////////// local includes /////////////////////////////
#include "pwcknowledgesource.hpp"
#include "pwcvariable.hpp"
#include "pwcpreconditionstatements.hpp"
#include "LnTokenizer.hpp"

/** SIGIO Call back function for processing the Precondition*/
int pwCKnowledgeSource::SIGIOcallbackPrecondition(std::string buffer){
	std::string	sToken, sSubToken, sSubSubToken, sTemp;
//  vector<std::string>::iterator where;
	LnTokenizer SubTokenizer, SubSubTokenizer, Tokenizer(buffer);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	SubTokenizer.setOperators("");
	SubSubTokenizer.setOperators("");
// 	pwCPreconditionStatements* pPCStatement;
	unsigned int i, j ,nIndex=0, nCount=0;
	bool bStatus;
	int nSize;
	pwCVariable *pVar;
	
	sToken = Tokenizer.getNextToken();
	if ( (sToken == "true") ||(sToken == "false") ||(sToken == "error")){
		if (sToken == "true") {
			bStatus = true;
		}
		else {
			bStatus = false;
		}
		nCount=1;
		if ((sToken = Tokenizer.getNextToken())!= LNVOID) {
			sToken = RemoveBrackets(sToken);
			SubTokenizer.setString(sToken);
			nCount = 0; // count the no of token (no. of instances).
			while ((sSubToken = SubTokenizer.getNextToken())!= LNVOID) { // if not empty then do.
				sSubToken = RemoveBrackets(sSubToken);
				nIndex=0; // no of match variables in the statment.
				SubSubTokenizer.setString(sSubToken);
				while ((sSubSubToken = SubSubTokenizer.getNextToken())!= LNVOID) { // if not empty then do.
					// check error.
					if ( nIndex >= (m_vPreConditionStatements.back()).m_vMatchVariables.size() ) {
						std::cout << "Temp variable list for the callback function has wrong no of variable!!!" << std::endl;
						exit(1);
					}
					sTemp = m_vPreConditionStatements.back().m_vMatchVariables[nIndex].GetName();
					// add the current callback value to m_vVariables in the KS object
					pVar = FindVariable(sTemp, &m_vVariables);
					if (pVar != NULL) {
						pVar->m_vsData.push_back(sSubSubToken);
						// Added P.Wong on 06-09-2002.
						pVar->m_vnInstantIndex.push_back(nCount);
					}
					else {
						std::cout << "Error at the callback function: variable not found" << std::endl;
						exit (1); // variable not found
					}
					// add the current callback value to m_vVariables in the statement object
					pVar = FindVariable(sTemp, &m_vPreConditionStatements.back().m_vVariables);
					if (pVar != NULL) {
						pVar->m_vsData.push_back(sSubSubToken);
						// Added P.Wong on 06-09-2002.
						pVar->m_vnInstantIndex.push_back(nCount);
					}
					else {
						std::cout << "Error at the callback function: variable not found" << std::endl;
						exit (1); // variable not found
					}
					nIndex++;
				} // while
				nCount++;
			} // while
		} // if
		else if (bStatus == false) {
			// if the outcome is false, put "<Ignore>" to the match variables in current statement.
			nSize = m_vPreConditionStatements.back().m_vMatchVariables.size();
			for (i=0; i< nSize; i++) {
//          m_vPreConditionStatements.back().m_vMatchVariables[i].m_vsData.push_back("<Ignore>");
				sTemp = m_vPreConditionStatements.back().m_vMatchVariables[i].GetName();
				pVar = FindVariable(sTemp, &m_vPreConditionStatements.back().m_vVariables);
				if (pVar != NULL) {
					pVar->m_vsData.push_back("<Ignore>");
					// Added P.Wong on 06-09-2002.
					pVar->m_vnInstantIndex.push_back(0);
				}
				else {
					std::cout << "Error at the callback function: variable not found" << std::endl;
					exit (1); // variable not found
  			}
			}
		}
		// when the callback result is true but no data associate with it, clear the
		// the Match variable and run time variable lists in that statement.
		if ( (bStatus==true) && (nIndex==0) && (nCount==0) ){
			m_vPreConditionStatements.back().m_vMatchVariables.clear();
			m_vPreConditionStatements.back().m_vVariables.clear();
		}
		// store the insert varibale's values and the statement's boolean values.
		m_vPreConditionStatements.back().StoreCallbackResults(nCount, bStatus);
	} // if
	else {
		if (m_nDebug >= 1)
			std::cout << "callback for Action: " << buffer << std::endl;
	}	
	return 0;
}

/** SIGIO Call back function for processing the Action*/
void pwCKnowledgeSource::SIGIOcallbackAction(std::string buffer){
	if (m_nDebug >= 1)
		std::cout << "callback for Action: " << buffer << std::endl;
}

pwCKnowledgeSource::pwCKnowledgeSource(){
	m_pClient = NULL;
	m_sKSFileDirectory = "./";
	m_sKSFileName = "KS.dkf";
	m_bReady = false;
	m_bKSFired = true;
	m_bKSRestart = false;
	m_nDebug = 1; // 0 - no debug info, 1 - moderate debug info, 2 - all.
	m_vVariables.clear();
	m_vsPartitions.clear();
}

pwCKnowledgeSource::~pwCKnowledgeSource(){
}

/** Load the knowledge file */
int pwCKnowledgeSource::LoadKnowledgeFile(){

	char ch;				// Character read from the file.	
	char buffer[65536]; // file buffer, max size 64K.
	std::string sTemp;
	int index = 0;
	FILE *ptr;			// Pointer to the file. FILE is a structure  defined in <stdio.h>

	// Open the file
	// std::cout << "file to open: " << m_sKSFileName.c_str() << std::endl;
	sTemp = m_sKSFileDirectory + m_sKSFileName;
	ptr = fopen(sTemp.c_str(),"r");
	if (ptr == NULL) {
		std::cout << "error openning KS file" << std::endl;
		exit (1); //  errors.
	}
	// Read one character at a time, checking
	// for the End of File. EOF is defined
	// in <stdio.h>  as -1 			
	m_sKSString.erase();
	while ((ch = fgetc(ptr)) != EOF)
	{
			buffer[index] = ch;
			index++;
	}
	buffer[index] = 0; // add null terminating character to the std::string.
	m_sKSString = buffer;
	if (m_nDebug >= 2)
		std::cout << "loaded KS string :" << std::endl << m_sKSString << std::endl;
	fclose(ptr);			// Close the file.

	return 0; // no error
}

/** function that extracts the KS variables. */
int pwCKnowledgeSource::CheckKeyWords(){
	LnTokenizer Tokenizer(m_sKSString);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string	Token, sTemp;
	while ((Token = Tokenizer.getNextToken())!= LNVOID) { // if not empty then do.
		if (m_nDebug >= 2)
			std::cout << "Token: " << Token << std::endl;
		if (Token.substr(0,2) == "/*") {
			// Token is a comment, simply ignore it.
		}
		else if (Token == "KS") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sKSName = sTemp;
		}
		else if (Token == "KS_TYPE") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sKSType = sTemp;
		}
		else if (Token == "INFERENCE_MODE") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sInferenceMode = sTemp;
		}
		else if (Token == "RULES") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			if ( (sTemp[0] == '[') && (sTemp[sTemp.length()-1] == ']') ){
				m_sRuleString = sTemp.substr(1,(sTemp.length())-2); // remove the square bracket.
			}
			else {
				m_sRuleString = sTemp;
			}
		}
		else if (Token == "FIRABILITY_FLAG") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			m_sFirabilityFlag = sTemp;
		}
		else if (Token == "IF") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			if ( (sTemp[0] == '[') && (sTemp[sTemp.length()-1] == ']') ){
				m_sPreconditionString = sTemp.substr(1,(sTemp.length())-2); // remove the square bracket.
			}
			else {
				m_sPreconditionString = sTemp;
			}
		}
		else if (Token == "THEN") {
			// Ignore this Token if it is a comment.
			do {
				sTemp = Tokenizer.getNextToken();
			} while (sTemp.substr(0,2) == "/*");
			if ( (sTemp[0] == '[') && (sTemp[sTemp.length()-1] == ']') ){
				m_sActionString = sTemp.substr(1,(sTemp.length())-2); // remove the square bracket.
			}
			else {
				m_sActionString = sTemp;
			}
		}
		else if (Token == "END") {
			return 0;
		}
		else {
			std::cout << "Tonken error. Check the KS file." << std::endl;
			exit (1); // unknown tonken error.
		}
	} // while
	std::cout << "WARNING: the key word END is missing" << std::endl;
	return 2; // key word "END" is missing.
}

/** this function print the KS variable on the screen. */
int pwCKnowledgeSource::PrintKS(){
	if (m_nDebug >= 2) {
  	std::cout << "KS = " << m_sKSName << std::endl;
  	std::cout << "KS_TYPE = " << m_sKSType << std::endl;
  	std::cout << "INFERENCE_MODE = " << m_sInferenceMode << std::endl;
  	std::cout << "RULES = " << m_sRuleString << std::endl;
  	std::cout << "FIRABILITY = " << m_sFirabilityFlag << std::endl;
  	std::cout << "PRECONDITION = " << m_sPreconditionString << std::endl;
  	std::cout << "ACTION = " << m_sActionString << std::endl;
  }
	return 0;
}

/** This function initialise the KS object such as load the KS files */
int pwCKnowledgeSource::Initialise(){
	// Load the KS files, create rule objects and initialise rule objects.
	if (!LoadKnowledgeFile()) { // no error
		CheckKeyWords();
		if ( m_nDebug >= 2)
			PrintKS();
	}
	else {
		std::cout << "File fail to open" << std::endl;
	}
	return 0;
}

/** This function set the KS file name. */
int pwCKnowledgeSource::SetKSFileName(std::string filename){
	m_sKSFileName = filename;
	return 0;
}

/** // Inference engine */
int pwCKnowledgeSource::Inference(){
	return 0;
}

/** generate instantiations according to the given precondition string. */
/* It first calls PreEvaluatePrecondition() to obtain all the match variables
from the BB, fill the insert variables and stored them before the
EvaluatePrecondition function is called.
It then creates all possible instantiations and elimates the invalid ones.
The invalid instantiations are those with same variables in different statements,
but with different values. A common variable list is used to contain these variables.
*/
int pwCKnowledgeSource::CreateInstantiations(std::string sPrecondition){
	unsigned int i, j, k, l, m, n, nCount, nTempCount, nIndex, nInstantiations;
	bool bStatus, bCountFlag, bAddFlag, bAddVariable, bAddInstantiation;
	std::string sVarName, sVarData, sData;
	std::vector<int> vnStatementID;
	std::vector<int> vnInstantSize;
	std::vector<int> vnInstantIndex;
	// A variable name list containing variables that are exist in 2 or more statements.
	std::vector<pwCVariable> vCommonVariable;
	std::vector<pwCVariable> vTempVariables;
	std::vector<std::string> vsTemp;
	std::vector<bool> vbInstantConditions;
	pwCVariable TempVar;
	pwCInstantiation TempInstant;
	int nSize, nTemp, nInstantIndex;
		
	if ( m_nDebug >= 1){
		std::cout << "Start creating instantiations... " << std::endl;
	}
	m_vInstantiations.clear();
	m_vPreConditionStatements.clear();
	m_vVariables.clear();
	m_vsPartitions.clear();
	/* This obtains all the match variables from the BB and stored them before the EvaluatePrecondition function is called. */
	PreEvaluatePrecondition(sPrecondition);
	if ( m_nDebug >= 1){
		std::cout << "size of m_vPreConditionStatements = " << m_vPreConditionStatements.size() << std::endl;
	}
	// p.wong 23-10-2001, change the method for creating the instantiations.
	// calculate all possible instantiations and create a list of common variables that are exist in 2 or more statements.
	vnInstantSize.clear();
	TempInstant.clear();
	nCount = 1;
	nInstantiations = 0;
	
	// Add the variable objects which appear at Statement objects to the temporary Instantiation object
	// An Instantiation object contains all the variables and their values at this instantiation.
	// loop through all the statements
	for (i=0; i<m_vPreConditionStatements.size(); i++) {
		// loop through all the variable in this statement
		for (j=0; j<m_vPreConditionStatements[i].m_vVariables.size(); j++) {
			if ( m_nDebug >= 1){
				m_vPreConditionStatements[i].m_vVariables[j].PrintData();
			}
			TempVar.clear();
			sVarName = m_vPreConditionStatements[i].m_vVariables[j].GetName();
			TempVar.SetName(sVarName);
			// Add the current variable to the Instant Variables list if that variable is not exist in the list.
			bAddFlag = true;
			// check the current variable name with those in the instantiation variable list.
			for (l=0; l<TempInstant.m_vVariables.size(); l++) {
				if ( sVarName == TempInstant.m_vVariables[l].GetName() ) {
					// add the variable name to the instantiation variable list if it is not already exist.
					bAddFlag = false;
					break;
				}
			} // for l
			if (bAddFlag) {
				TempInstant.m_vVariables.push_back(TempVar);
			}
		} // for j
	} // for i
	
	// calculate the number of possible instantiations and store in nCount.
	for (i=0; i<m_vPreConditionStatements.size(); i++) {
		nTempCount = m_vPreConditionStatements[i].m_vbCallbackConditions.size();
		vnInstantSize.push_back(nTempCount);
		nCount *= nTempCount;
	} // for i
	
	// create a common variable list containing variables that are exist in 2 or more statements.
	for (i=0; i<m_vPreConditionStatements.size()-1; i++) {
		// put all the variable names in the current statement to a temp list.
		vsTemp.clear();
		for (j=0; j<m_vPreConditionStatements[i].m_vVariables.size(); j++) {
			sVarName = m_vPreConditionStatements[i].m_vVariables[j].GetName();
			vsTemp.push_back(sVarName);
		}
		// This temp list will be used for comparing variable names with the subsequent statements.
		for (j=i+1; j<m_vPreConditionStatements.size(); j++) {
			// loop through all the variable in this statement
			for (k=0; k<m_vPreConditionStatements[j].m_vVariables.size(); k++) {
				sVarName = m_vPreConditionStatements[j].m_vVariables[k].GetName();
				// check the current variable name with those in the temp name list.
				for (l=0; l<vsTemp.size(); l++) {
					if (sVarName == vsTemp[l]) {
						// add the variable name to the common variable list if it is not already exist.
						bAddFlag = true;
						// loop through all the variable in the common variable list.
						for (m=0; m<vCommonVariable.size(); m++) {
							if ( sVarName == vCommonVariable[m].GetName() ){
								bAddFlag = false;
								break;
							}
						} // for m
						if (bAddFlag) {
							// add the variable to the common variable list.
							TempVar.clear();
							TempVar.SetName(sVarName);
							vCommonVariable.push_back(TempVar);
						}
					} // if
				} // for l
			} // for k
		} // for j
	} // for i

	// Create the instantiation.
	// create a list of variables storing the its corresponding values for instantiations.
	vnInstantIndex.clear();
	// initialise the instantiation index for each statement.
	for (i=0; i<m_vPreConditionStatements.size(); i++) {
		vnInstantIndex.push_back(0);
	}
	
	nIndex = 0;
	sData.erase();
	// loop through all the possible instantiations
	for (i=0; i<nCount; i++) {
		// initialise variables
		bAddInstantiation = true;
		vTempVariables.clear();
		vbInstantConditions.clear();
		// clear the data in the TempInstant list, but keep the variable names.
		for (j=0; j<TempInstant.m_vVariables.size(); j++){
			TempInstant.m_vVariables[j].m_vsData.clear();
		}
		TempInstant.m_vConditions.clear();
		for (j=0; j<vCommonVariable.size(); j++){
			vCommonVariable[j].m_vsData.clear();
			vCommonVariable[j].m_vnInstantIndex.clear();
		} // for j
		
		// loop through all the statements.
		for (j=0; j<m_vPreConditionStatements.size(); j++) {
			// it is the instantiation index for this statement.
			n = vnInstantIndex[j];
			// Get the boolean result for this instantiation for this statement.
			bStatus = m_vPreConditionStatements[j].m_vbCallbackConditions[n];
			// loop through all the variables in that statement
			for (k=0; k<m_vPreConditionStatements[j].m_vVariables.size(); k++) {
				TempVar.clear();
				sVarName = m_vPreConditionStatements[j].m_vVariables[k].GetName();
				sVarData = m_vPreConditionStatements[j].m_vVariables[k].m_vsData[n];
				// Added by P.Wong on 06-09-2002.
				nInstantIndex = m_vPreConditionStatements[j].m_vVariables[k].m_vnInstantIndex[n];
				TempVar.SetName(sVarName);
				TempVar.m_vsData.push_back(sVarData);
				// check validation for every possible instantiations.
				// loop through the common variable list containing variables that are exist in 2 or more statements.
				for (l=0; l<vCommonVariable.size(); l++){
					if (vCommonVariable[l].GetName() == sVarName) {
						if ( vCommonVariable[l].m_vsData.empty() ){
							vCommonVariable[l].m_vsData.push_back(sVarData);
							// Added by P.Wong on 06-09-2002.
							vCommonVariable[l].m_vnInstantIndex.push_back(nInstantIndex);
							break;
						}
						else {
							if ( (vCommonVariable[l].m_vsData[0] != sVarData) ||
								(vCommonVariable[l].m_vnInstantIndex[0] != nInstantIndex) ) {
								bAddInstantiation = false;
								break;
							}
						} //else
					} // if
				} // for l
				if (!bAddInstantiation) {
					break;
				}
				else {
					// Add the current instantiation to the temp Variables list.
					bAddVariable = true;
					for (l=0; l<vTempVariables.size(); l++) {
						// check the current variable name with those in the temp variable list.
						if ( sVarName == vTempVariables[l].GetName() ) {
							// add the variable name to the instantiation variable list if it is not already exist.
							bAddVariable = false;
							break;
						}
					} // for l
					if (bAddVariable) {
						// add the variable to the temp Variables list.
							vTempVariables.push_back(TempVar);
					}
				} // else
			} // for k
			if (!bAddInstantiation) {
				break;
			}
			else {
				vbInstantConditions.push_back(bStatus);
			}
		} // for j
		if (bAddInstantiation) {
			// Temp variable list should contain the same number of variables and in the same order as the Instantiation variable list.
			nSize = TempInstant.m_vVariables.size();
			nTemp = vTempVariables.size();
			if ( nSize != nTemp ) {
				std::cout << "Wrong length of the Instantiation list" << std::endl;
				exit(1);
			}
			for (j=0; j<TempInstant.m_vVariables.size(); j++){
				if (TempInstant.m_vVariables[j].GetName() == vTempVariables[j].GetName() ){
					sData = vTempVariables[j].m_vsData[0];
					TempInstant.m_vVariables[j].m_vsData.push_back(sData);
				}
				else {
					std::cout << "Wrong order of variables in the Instantiation list" << std::endl;
					exit(1);
				}
			} // for j

			// Add the callback result to the Condition list.
			for (j=0; j<vbInstantConditions.size(); j++){
				TempInstant.m_vConditions.push_back(vbInstantConditions[j]);
			}
			// add the current instantiation to the instantiation list.
			m_vInstantiations.push_back(TempInstant);
			nInstantiations++;
		} // if
		
		// Adjust the instantiation index.
		bCountFlag = true;
		// the size of vnInstantIndex and vnInstantSize is the number of Statements.
		for (j=0; j<vnInstantIndex.size(); j++){
			nTempCount = vnInstantIndex[j];
			if (bCountFlag) {
				nTempCount++;
			}
			if (nTempCount >= vnInstantSize[j]){
				nTempCount = 0;
				bCountFlag = true;
			}
			else {
				bCountFlag = false;
			}
			vnInstantIndex[j] = nTempCount;
		} // for j
	} // for i
	
	return nInstantiations;
}

/** it obtains all the match variables from the BB and stored them before the
EvaluatePrecondition function is called. */
int pwCKnowledgeSource::PreEvaluatePrecondition(std::string sToken){
	LnTokenizer Tokenizer(sToken);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	pwCPreconditionStatements PCStatement;
	std::string	sSubToken, sMessage, sTemp;
	
	while ( ((sSubToken = Tokenizer.getNextToken()) != LNVOID) ){
		if ( m_nDebug >= 2){
			std::cout << "PreEvaluatePrecondition: token = " << sSubToken << std::endl;
		}
		if ( (sSubToken != "AND") && (sSubToken != "OR") && (sSubToken.substr(0,2) != "/*") ) {
			sSubToken = RemoveBrackets(sSubToken);
			// check if sMessage is a compound token.
			if ( (sSubToken.find("AND") < sSubToken.length()) ||
					(sSubToken.find("OR") < sSubToken.length()) ) {
				PreEvaluatePrecondition(sSubToken);
			}
			else {
				// create a Statement object to store the variables and logical values.
				PCStatement.SetID(m_nStatementID);
				PCStatement.m_pKS = this;
				m_vPreConditionStatements.push_back(PCStatement);
				SendTCPData(sSubToken, 0, 0);
				m_nStatementID++;
			} // else
		} // if
	} // while
	return 0;
}

/** a recursive function that evaluates the KS and rules precondition */
/* m_nStatementIndex should be set before this function is called. */
bool pwCKnowledgeSource::EvaluatePrecondition(std::string sToken, int nInstantiationIndex){
	// variables
	LnTokenizer Tokenizer(sToken);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string	sSubToken, sMessage, sTemp, sKey;
	bool bResult, bTemp;
	std::vector<std::string> vsAND_OR;
	std::vector<bool> vbConditions;
	unsigned int nIndex;

	vbConditions.clear();
	vsAND_OR.clear();
	// check KS precondition.
	// setup callback function for precondition.
	//	m_TcpClient.setCallback(SIGIO, SIGIOcallbackPrecondition, this);
	if (m_nDebug >= 1) {
		std::cout << "Start Evaluating Precondition..." << std::endl;
	}
	while ( ((sSubToken = Tokenizer.getNextToken()) != LNVOID) ){
		// check for the KS restarting signal.
		if (m_bKSRestart) {
			return false;
		}
		if ( (sSubToken != "AND") && (sSubToken != "OR") && (sSubToken.substr(0,2) != "/*") ) {
			sSubToken = RemoveBrackets(sSubToken);
			// check if sMessage is a compound token.
			if ( (sSubToken.find("AND") < sSubToken.length()) ||
				(sSubToken.find("OR") < sSubToken.length()) ) {
				bTemp = EvaluatePrecondition(sSubToken, nInstantiationIndex);
				vbConditions.push_back(bTemp);
			}
			else {
				bTemp = m_vInstantiations[nInstantiationIndex].m_vConditions[m_nStatementIndex];
				vbConditions.push_back(bTemp);
        m_nStatementIndex++;
			}
		}
		else {
			if (sSubToken.substr(0,2) != "/*") {
				// The sSubToken is either "AND" or "OR".
				vsAND_OR.push_back(sSubToken);
			}
		}
	} // while
	// Evalulating the preconditions
	nIndex = vbConditions.size();
	if (m_nDebug >= 2) {
		std::cout << "size of the condition list:" << nIndex << std::endl;
	}
	bResult = vbConditions[0];
	if (m_nDebug >= 2) {
		std::cout << "boolean state of precondition statement 0 " << "=" << bResult << std::endl;
	}
	for (unsigned int i=1; i<nIndex; i++) {
		bTemp = vbConditions[i];
		if (m_nDebug >= 2) {
			std::cout << "boolean state of precondition statement " << i << " =" << bTemp << std::endl;
		}
		if (vsAND_OR[i-1] == "AND") {
			bResult = (bResult && bTemp);
		}
		else {
			bResult = (bResult || bTemp);
		}
	}
	if (m_nDebug >= 1) {
		std::cout << "Precondition = " << bResult << std::endl;
		std::cout << "Done Evaluating Precondition..." << std::endl;
	}
	return bResult;
}

/** the actions provoke when the KS is activated. */
int pwCKnowledgeSource::DoAction(int nInstantiations){
	int i;
	// evaluate the action string for this KS.
	for (i=0; i<nInstantiations; i++) {
		// check for the KS restarting signal.
		if (m_bKSRestart) {
			return 1;
		}
		EvaluateAction(m_sActionString, i);
	}
	return 0;
}

/** function evaluate the action string for both KS and rules. */
int pwCKnowledgeSource::EvaluateAction(std::string sAction, int InstantIndex){
	LnTokenizer MainTokenizer(sAction);
	LnTokenizer SubTokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	MainTokenizer.setOperators("");
	SubTokenizer.setOperators("");
	std::string	MainToken, SubToken, Message, sKey, sTemp;
	int nDebug;
	
	nDebug = m_nDebug;
	if (m_nDebug >= 1) {
		std::cout << "Start evaluating action string..." << std::endl;
	}
	while ( (MainToken = MainTokenizer.getNextToken()) != LNVOID ){
		// check for the KS restarting signal.
		if (m_bKSRestart) {
			return 1;
		}
		if ((MainToken != "AND") && (MainToken.substr(0,2) != "/*")) {
			Message = RemoveBrackets(MainToken);
			SubTokenizer.setString(Message);
			SubToken = SubTokenizer.getNextToken();
			if (m_nDebug >= 2)
				std::cout << "Token = " << SubToken << std::endl;
			if ( IsBBKeyWord(0, SubToken) ) {
				if ( SendTCPData(Message, 1, InstantIndex) ) {
					std::cout << "error sending TCP data" << std::endl;
					exit (1);
				}
			}
			else if (SubToken == "run_algorithm") {
				if ( RunAlgorithm(SubTokenizer.getNextToken(), 1, InstantIndex) ) {
					std::cout << "error running algorithm" << std::endl;
					exit (1);
				}
			}
			else if (SubToken == "load_file_to_blackboard") {
				if ( LoadFileToBB(SubTokenizer.getNextToken(), 1, InstantIndex) ) {
					std::cout << "error loading file" << std::endl;
					exit (1);
				}
			}
			else if (SubToken == "report") {
				if ( Report(SubTokenizer.getNextToken(), 1, InstantIndex) ) {
					std::cout << "error reporting" << std::endl;
					exit (1);
				}
			}
			else {
				std::cout << "Error : wrong action" << std::endl;
				exit (1); // error
			}
		}
		MainToken.erase();
		Message.erase();
		SubToken.erase();
	} // while
	if (m_nDebug >= 1) {
		std::cout << "Done evaluating action string..." << std::endl;
	}
	return 0;
}

/** check if the given string is in one of the Black Board keyword categories . */
bool pwCKnowledgeSource::IsBBKeyWord(int nCategories, std::string token){
	std::string sKeyWords, MainToken;
 	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
//	unsigned int nResult;	
	switch (nCategories) {
	case 0:
		// all keywords
		sKeyWords = BBALLKEYWORDS;
		break;
	case 1:
		// keywords that require no parameter
		sKeyWords = BBKEYWORDS1;
		break;
	case 2:
		// keywords that only require one parameter
		sKeyWords = BBKEYWORDS2;
		break;
	case 3:
		// keywords that require two parameters
		sKeyWords = BBKEYWORDS3;
		break;
	default:
		// all keywords
		sKeyWords = BBALLKEYWORDS;
		break;
	}
	Tokenizer.setString(sKeyWords);	
	while ( (MainToken = Tokenizer.getNextToken()) != LNVOID ){
		if (MainToken == token) {
			return true;
		}
	}
	return false;
}

/** run an external function to preformance a particular task. */
// For precondition statement, nType=0; For action statement, nType=1;
int pwCKnowledgeSource::RunAlgorithm(std::string sAlgorithm, int nType, int nInstant){
	std::string sToken, sLib, sFunction, sInput, sResultName, sResult;
	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	pwCVariable Var;
	pwCVariable *pTempVar;
	unsigned int i;
	int nResult;

	if ( m_nDebug >= 1) {
		std::cout << "Start running algorithms..." << std::endl;
	}
	sAlgorithm = RemoveBrackets(sAlgorithm);
	Tokenizer.setString(sAlgorithm);
	// extract share library
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sLib = sToken;
	if ( m_nDebug >= 1)
		std::cout << "Lib: " << sLib << std::endl;
	// extract function name
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sFunction = sToken;
	if ( m_nDebug >= 1)
		std::cout << "Function: " << sFunction << std::endl;
	// extract input parameters
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sInput = sToken;
	if ( m_nDebug >= 1)
		std::cout << "Input: " << sInput << std::endl;
	// extract return value
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sResultName = sToken;
	if (sResultName != "Nil") {
		Var.SetName(sResultName);
		if ( m_nDebug >= 1) std::cout << "Variable name: " << sResultName << std::endl;
	}
	else {
		Var.clear();
	}
	sInput = RemoveBrackets(sInput);
	if (nType == 0) {
		nResult = EvaluateBBCommandContent(sInput);
		if (nResult != 0) {
			std::cout << "error evaluating the BB command content" << std::endl;
			exit (1);
		}
	}
	else if (nType == 1) {
		EvaluateActionContent(sInput, nInstant);
	}
	if ( m_nDebug >= 1)
		std::cout << "size of BB Command Contents list = " << m_vsBBCommandContents.size() << std::endl;
	for (i=0; i<(m_vsBBCommandContents.size()); i++) {
		// check for the KS restarting signal.
		if (m_bKSRestart) {
			return 1;
		}
		m_sCurrentCommandContent = m_vsBBCommandContents[i];
		if ( m_nDebug >= 1)
			std::cout << "sContent = " << m_sCurrentCommandContent << std::endl;
	 	if (sLib == "LOCAL") {
	 		if (sFunction == "GetTextInput") {
	 			sResult = GetTextInput(m_sCurrentCommandContent);
	 		}
	 		else if (sFunction == "SetVariable"){
	 			sResult = m_sCurrentCommandContent;
	 		}
	 		else if (sFunction == "IntegerAdder") {
	 			sResult = IntegerAdder(m_sCurrentCommandContent);
	 		}
	 		else if (sFunction == "CountToken") {
	 			sResult = CountToken(m_sCurrentCommandContent);
	 		}
	 		else if (sFunction == "StringMerge") {
	 			sResult = StringMerge(m_sCurrentCommandContent);
	 		}
	 		else if (sFunction == "StringListFormer") {
	 			sResult = StringListFormer(m_sCurrentCommandContent);
	 		}
	 		else if (sFunction == "GetFormedStringList") {
	 			sResult = GetFormedStringList(m_sCurrentCommandContent);
	 		}
	 		else if (sFunction == "ClearStringList") {
	 			sResult = ClearStringList(m_sCurrentCommandContent);
	 		}
	 		else {
	 			std::cout << "Error at RunAlgorithm()" << std::endl;
	 			std::cout << "no such local function" << std::endl;
	 			exit (1); // error
	 		}
	 	}
	 	else {
			try
			{
				m_pClient->m_soLoader.setDefaultDir(m_sKSFileDirectory);
				m_pClient->m_soLoader.link(sLib);
				m_pClient->m_soLoader.call( sFunction, sResult, RemoveBrackets(m_sCurrentCommandContent) );
//				sResult = m_pClient->m_soLoader.call( sFunction, 1, this );
				m_pClient->m_soLoader.unlink();
			}
			catch( std::runtime_error &error )
			{
				std::cout<<error.what()<<std::endl;
			}
		} // else
		if (sResultName != "Nil") {
			Var.m_vsData.push_back(sResult);
			if ( m_nDebug >= 1) std::cout << "Result: " << sResult << std::endl;
		}
	} // for
	// if the variable is not already exist in the list, add it.
	pTempVar = FindVariable(sToken, &m_vInstantiations[nInstant].m_vVariables);
	if ( pTempVar == NULL) {
		m_vInstantiations[nInstant].m_vVariables.push_back(Var); // Add variable
	}
	else {
		pTempVar->m_vsData = Var.m_vsData;
	}
	if ( m_nDebug >= 1) {
		std::cout << Var.GetName() << " is added to the variable list" << std::endl;
		std::cout << "Done running algorithms..." << std::endl;
	}
//	std::cout << "m_nDebug = " << m_nDebug << std::endl;
	return 0;
}

/** print the message to the screen or other distinations. */
// For precondition statement, nType=0; For action statement, nType=1;
int pwCKnowledgeSource::Report(std::string message, int nType, int nInstant){
	std::string sContent, sMessage;
		
	if ( m_nDebug >= 1) {
		std::cout << "Start reporting..." << std::endl;
		std::cout << "Report Content is " << message << std::endl;
	}
	message = RemoveBrackets(message);
	// check for insert variables.
	if (nType == 1) {
		sContent = EvaluateActionContent(message, nInstant);
	}
	sMessage = "Report : " + sContent;
	DisplayText(sMessage);
	if ( m_nDebug >= 1) {
		std::cout << "Done reporting..." << std::endl;
	}
	return 0;
}

/** check the BB commands and match variables before sending the command to BB. */
// For precondition statement, nType=0; For action statement, nType=1;
int pwCKnowledgeSource::SendTCPData(std::string sMessage, int nType, int InstantIndex){
	LnTokenizer Tokenizer(sMessage);
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
// 	pwCPreconditionStatements* pPCStatement;
	std::string sToken, sBBCommand, sBBContent, sBBPanel, sData, sTemp;
	unsigned int i;
	int nResult;
	bool bStatus;

	if ( m_nDebug >= 1) {
		std::cout << "Start preparing TCP data..." << std::endl;
	}
	// extract BB command
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sBBCommand = sToken;
	// check if the precondition command is compare.
	if (sBBCommand == "compare") {
		// extract compare's content
		do {
			sToken = Tokenizer.getNextToken();
		} while (sToken.substr(0,2) == "/*");
		nResult = EvaluateBBCommandContent(sToken);
		if (nResult != 0) {
			std::cout << "error evaluating the BB command content" << std::endl;
			exit (1);
		}
		for (i=0; i<(m_vsBBCommandContents.size()); i++) {
			sBBContent = m_vsBBCommandContents[i];
			bStatus = Compare(sBBContent);
			// there will only be 1 instant.
			m_vPreConditionStatements.back().m_nInstantIndex = i;
			m_vPreConditionStatements.back().StoreCallbackResults(1, bStatus);
		} // for
	}
	// check if the BB keyword requires no parameter.
	else if (IsBBKeyWord(1, sBBCommand)) {
		if ( m_nDebug >= 2) {
			std::cout << "TCP data contains only 1 element" << std::endl;
		}
		DoSendTCPData(sBBCommand);
	}
	else if (IsBBKeyWord(2, sBBCommand)){
		// check if the BB keyword requires 1 parameters.
		// extract BB partition name
		do {
			sToken = Tokenizer.getNextToken();
		} while (sToken.substr(0,2) == "/*");
		sBBPanel = sToken;
		AddToPartitionList(sBBPanel);
		sData = sBBCommand + " " + sBBPanel;
		if ( m_nDebug >= 2) {
			std::cout << "TCP data contains 2 element" << std::endl;
		}
		DoSendTCPData(sData);
	}
	else if (IsBBKeyWord(3, sBBCommand)) {
		// check if the BB keyword requires 2 parameters.
		if (sBBCommand == "on_partition") {
			if (m_sInferenceMode == "MI_Forwardchain") {
				sBBCommand = "ret_all";
			}
			else if (m_sInferenceMode == "SI_Forwardchain") {
				sBBCommand = "ret_first";
			}
		}
		if ( m_nDebug >= 2) {
			std::cout << "BB Command ->" << sBBCommand << std::endl;
		}
		// extract BB content
		do {
			sToken = Tokenizer.getNextToken();
		} while (sToken.substr(0,2) == "/*");
		if (nType == 0) {
			nResult = EvaluateBBCommandContent(sToken);
			if (nResult != 0) {
				std::cout << "error evaluating the BB command content" << std::endl;
				exit (1);
			}
		}
		else if (nType == 1) {
			EvaluateActionContent(sToken, InstantIndex);
		}
		// extract BB panel
		do {
			sToken = Tokenizer.getNextToken();
		} while (sToken.substr(0,2) == "/*");
		sBBPanel = sToken;
		AddToPartitionList(sBBPanel);
		// prepare a storage for the callback values.
		// sending comands to BB.
		for (i=0; i<(m_vsBBCommandContents.size()); i++) {
			sBBContent = m_vsBBCommandContents[i];
			sData = sBBCommand + " " + sBBContent + " " + sBBPanel;
			if ( m_nDebug >= 2) {
				std::cout << "TCP data contains 3 element" << std::endl;
			}
			m_vPreConditionStatements.back().m_nInstantIndex = i;
			DoSendTCPData(sData);
		} // for
	} //else if
	else {	
		std::cout << "unrecognise BB command: " << sBBCommand << std::endl;
		exit (1); // unrecognise BB command.
	}
	if ( m_nDebug >= 1) {
		std::cout << "Done preparing TCP data." << std::endl;
	}
	return 0;
}

/** remove the first pair of square bracket of the given string */
std::string pwCKnowledgeSource::RemoveBrackets(std::string sToken){
	size_t cp;
	if ( m_nDebug >= 2) {
		std::cout << "token is ->" << sToken << std::endl;
	}
	if ( (sToken[0] == '[') && (sToken[sToken.length()-1] == ']') ){
		sToken = sToken.substr(1,(sToken.length())-2); // remove the square bracket.
		// strip leading and tailing white space
		strip( sToken, BOTH, WHITESPACE);
		// check if the token is empty
		cp = sToken.find_first_not_of(WHITESPACE);
		if( cp > sToken.length())
		{
	    sToken.erase();
		}
	}
	if ( m_nDebug >= 2) {
		std::cout << "After removing bracket, token is ->" << sToken << std::endl;
	}
	return sToken;
}

/** remove the first pair of <> bracket of the given string */
std::string pwCKnowledgeSource::RemovePointedBrackets(std::string sToken){
	size_t cp;
	if ( m_nDebug >= 2) {
		std::cout << "token is ->" << sToken << std::endl;
	}
	if ( (sToken[0] == '<') && (sToken[sToken.length()-1] == '>') ){
		sToken = sToken.substr(1,(sToken.length())-2); // remove the square bracket.
		// strip leading and tailing white space
		strip( sToken, BOTH, WHITESPACE);
		// check if the token is empty
		cp = sToken.find_first_not_of(WHITESPACE);
		if( cp > sToken.length())
		{
	    sToken.erase();
		}
	}
	if ( m_nDebug >= 2) {
		std::cout << "After removing bracket, token is ->" << sToken << std::endl;
	}
	return sToken;
}

/** it put the match variables (?a or ??a) in the KS variable list, m_vVariables.
and replace the (~a or ~~a) with the insert variables. */
int pwCKnowledgeSource::EvaluateBBCommandContent(std::string sContent){
	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string sToken, sTemp, sData;
	unsigned int i, j, k, nCount, nTempCount, nVarSize, nIndex;
	pwCVariable TempVar, *pVar;
//	vector<pwCVariable> vInsertVars;
	std::vector<int> vStatementID, vnCount;
	bool bCountFlag;
		
	(m_vPreConditionStatements.back()).m_vMatchVariables.clear(); 	
	m_vsBBCommandContents.clear();
	// identify the match and insert variables.
	sTemp = RemoveBrackets(sContent);
	if ( m_nDebug >= 1) {
		std::cout << "Starting evaluate BB Command Content..." << std::endl;
		std::cout << "BB Command Content is " << sContent << std::endl;
	}
	Tokenizer.setString(sTemp);
	while ( ((sToken = Tokenizer.getNextToken()) != LNVOID) ){
		if ( m_nDebug >= 2) {
			std::cout << "Token = " << sToken << std::endl;
		}
		if (sToken[0] == '?') {
			if (sToken[1] == '?') {
				sTemp =sToken.substr(2,(sToken.length())-1);
			}
			else {
				sTemp =sToken.substr(1,(sToken.length())-1);
			}
			// if the variable is not already exist in the list, add it.
			pVar = FindVariable(sTemp, &m_vVariables);
			TempVar.clear();
			TempVar.SetName(sTemp);
			TempVar.m_vnStatementID.push_back( m_vPreConditionStatements.back().GetID() );
			if ( pVar == NULL) {
				m_vVariables.push_back(TempVar); // Add variable
			}
			else {
				pVar->clear();
				pVar->SetName(sTemp);
				pVar->m_vnStatementID.push_back( m_vPreConditionStatements.back().GetID() );
			}
			// put the variable in a statement variable list for the callback function.
			// the variable's values will be filled in at the SIGIOCallbackPrecondition function.
			(m_vPreConditionStatements.back()).m_vMatchVariables.push_back(TempVar);
			(m_vPreConditionStatements.back()).m_vVariables.push_back(TempVar);
		}
		else if (sToken[0] == '~') {
			if (sToken[1] == '~') {
				sTemp =sToken.substr(2,(sToken.length())-1);
			}
			else {
				sTemp =sToken.substr(1,(sToken.length())-1);
			}
			pVar = FindVariable(sTemp, &m_vVariables);
			TempVar.clear();
			TempVar.SetName(sTemp);
			if (pVar != NULL) {
				// add the current statement ID to the variable.
				pVar->m_vnStatementID.push_back( m_vPreConditionStatements.back().GetID() );
				m_vPreConditionStatements.back().m_vInsertVariables.push_back(*pVar);
				m_vPreConditionStatements.back().m_vVariables.push_back(TempVar);
			}
			else {
				std::cout << "Error evaluating command content: variable not found" << std::endl;
				exit (1); // variable not found
			}
		}
	} // while
	// if there is insert variable in the BB command content string.
	nCount = 1;
	nVarSize = m_vPreConditionStatements.back().m_vInsertVariables.size();
	if ( m_nDebug >= 1) {
		std::cout << "No of insert variables in this command content is " << nVarSize << std::endl;
	}
	if (nVarSize > 0){
		// find the insert variable name in the list, extract the variable value.
		// calculate how many instantiations are.
		vStatementID.clear();
		for (i=0; i<nVarSize; i++) {
			TempVar = m_vPreConditionStatements.back().m_vInsertVariables[i];
			bCountFlag = true;
			// check if that variable is in the same statement as those variables that are already counted.
			// or check if the same variable appear on another statement in which the variable has already counted.
			// in both case, don't count that for this instantiation.
			for (j=0; j<vStatementID.size(); j++) {
				for (k=0; k<TempVar.m_vnStatementID.size(); k++) {
					if ( TempVar.m_vnStatementID[k] == vStatementID[j] ){
						bCountFlag = false;
						break;
					}
				} // for k
				if (bCountFlag == false) {
					break;
				}
			} //for j
			if (bCountFlag) {
				nCount *= TempVar.m_vsData.size();
				for (k=0; k<TempVar.m_vnStatementID.size(); k++) {
					vStatementID.push_back( TempVar.m_vnStatementID[k] );
				}
			}
		} // for i
		if ( m_nDebug >= 1) {
			std::cout << "total instantiations in this command content is " << nCount << std::endl;
		}
		// generate primary instantiations.
		for (i=0; i<nCount; i++) {
			m_vsBBCommandContents.push_back(sContent);
		}
		// replace the insert variables with the appropriate values.
		nCount = 1;
		vStatementID.clear(); vnCount.clear();
		for (i=0;	i<nVarSize; i++) {
			// check error.
			if (i >= m_vPreConditionStatements.back().m_vInsertVariables.size() ) {
				std::cout << "Error evaluating command content: variable not found" << std::endl;
				exit (1);
			}
			TempVar = m_vPreConditionStatements.back().m_vInsertVariables[i];
			if (&TempVar != NULL) {
				// check if that variable is in the same statement as those variables that are already checked.
				bCountFlag = true;
				for (j=0; j<vStatementID.size(); j++) {
					for (k=0; k<TempVar.m_vnStatementID.size(); k++) {
						if ( TempVar.m_vnStatementID[k] == vStatementID[j] ){
							nTempCount = nCount;
							nCount = vnCount[j];
							bCountFlag = false;
							break;
						}
					}
					if (bCountFlag == false) {
						break;
					}
				}
				if (bCountFlag) {
					for (k=0; k<TempVar.m_vnStatementID.size(); k++) {
						vStatementID.push_back( TempVar.m_vnStatementID[k] );
					}
					vnCount.push_back( nCount );
				}
				sTemp = TempVar.GetName();
//				// put the variable in a statement variable list for the callback function.
//				(m_vPreConditionStatements.back()).m_vVariables.push_back(TempVar);
//				(m_vPreConditionStatements.back()).m_vVariables.back().m_vsData.clear();
				k=0; nIndex=0;
				for (j=0; j<(m_vsBBCommandContents.size()); j++) {
					Tokenizer.setString( RemoveBrackets(m_vsBBCommandContents[j]) );
					// replacing
					sData.erase();
					while ( ((sToken = Tokenizer.getNextToken()) != LNVOID) ){
						if ( (sToken == ("~"+sTemp)) || (sToken == ("~~"+sTemp)) ){
							sData += ( (TempVar.m_vsData[k]) + " ");
//							// put the variable in a statement variable list for the callback function.
//							(m_vPreConditionStatements.back()).m_vVariables.back().m_vsData.push_back(TempVar.m_vsData[k]);
						}
						else {
							sData += (sToken + " ");
						}
					} // while
					strip(sData, BOTH, WHITESPACE);
					m_vsBBCommandContents[j] = "[" + sData + "]";
					nIndex++;
					if (nIndex >= nCount) {
						k++;
						if ( k >= TempVar.m_vsData.size() ) {
							k=0;
						}
						nIndex=0;
					}
				} // for j
				if (bCountFlag) {
					nCount *= TempVar.m_vsData.size();
				}
				else {
					// restore to the previous count.
					nCount = nTempCount;
				}
			} // if
			else {
				std::cout << "Error evaluating command content: variable not found" << std::endl;
				exit (1);
			}
		} // for
	} // if
	else {
		m_vsBBCommandContents.push_back(sContent);
	}
	if ( m_nDebug >= 1) {
		std::cout << "Done evaluate BB Command Content." << std::endl;
	}
	return 0;
}

/** find and evaluate the insert variables of the report content */
std::string pwCKnowledgeSource::EvaluateActionContent(std::string sContent, int InstantIndex){
	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string sToken, sTemp, sData;
	pwCVariable *pVar;
		
	if ( m_nDebug >= 1) {
		std::cout << "Starting evaluate action content..." << std::endl;
		std::cout << "action content is " << sContent << std::endl;
	}
	m_vsBBCommandContents.clear();
	// replace the insert variables.
	sData.erase();
	sTemp = RemoveBrackets(sContent);
	Tokenizer.setString(sTemp);
	while ( ((sToken = Tokenizer.getNextToken()) != LNVOID) ){
		if ( m_nDebug >= 2) {
			std::cout << "Token = " << sToken << std::endl;
		}
		if (sToken[0] == '~') {
			if (sToken[1] == '~') {
				sTemp =sToken.substr(2,(sToken.length())-1);
			}
			else {
				sTemp =sToken.substr(1,(sToken.length())-1);
			}
			if ( m_nDebug >= 1) {
				int nSize = m_vInstantiations[InstantIndex].m_vVariables.size();
				for (int i=0; i < nSize; i++) {
					m_vInstantiations[InstantIndex].m_vVariables[i].PrintData();
				}
			}
			pVar = FindVariable(sTemp, &m_vInstantiations[InstantIndex].m_vVariables);
			if (pVar != NULL) {
				sToken = pVar->m_vsData[0];
			}
			else {
				std::cout << "Error at EvaluateActionContent()" << std::endl;
				std::cout << "variable not found" << std::endl;
				exit (1); // variable not found
			}
		}
		sData += (sToken + " ");
	} // while
	strip(sData, BOTH, WHITESPACE);
	sData = "[" + sData + "]";
	m_vsBBCommandContents.push_back(sData);
	return sData;
}

/** check if the given variable is exist in the variable list. */
bool pwCKnowledgeSource::IsVariableExist(std::string sVar){
	unsigned int i;
	std::string sTemp;
	
	for (i=0; i<(m_vVariables.size()); i++) {
		if (m_vVariables[i].GetName() == sTemp) {
			return true;
		}
	} // for
	return false;
}
/** find the given variable and return it's reference. */
pwCVariable* pwCKnowledgeSource::FindVariable(std::string sName, std::vector<pwCVariable>* pvVar){
	std::vector<pwCVariable>::size_type i;
	pwCVariable* pVariable;
	if ( m_nDebug >= 1) {
		std::cout << "Start finding variables..." << std::endl;
		std::cout << "No of Variables = " << pvVar->size() << std::endl;
		for (i=0; i<pvVar->size(); i++) {
			pvVar->operator[](i).PrintData();
		}
	}
	for (i=0; i<pvVar->size(); i++) {
		pVariable = &pvVar->operator[](i); // somehow, I can't use the at(i) function.
		if ( m_nDebug >= 2)
			std::cout << "pvVar[" << i << "] = " << pVariable->GetName() << std::endl;
		if (pVariable->GetName() == sName) {
			if ( m_nDebug >= 2) {
				std::cout << "found " << sName << std::endl;
				std::cout << "Done finding variables." << std::endl;
			}
			return pVariable;
		}
	} // for
	if ( m_nDebug >= 1) {
		std::cout << "Variables not found" << std::endl;
		std::cout << "Done finding variables." << std::endl;
	}
	return NULL;
}
/** A local function for rule files, ask the user a question and request an answer. */
std::string pwCKnowledgeSource::GetTextInput(std::string sPrompt){
	return m_pClient->GetTextInput(sPrompt);
}
/** a local function which adds the 2 given numeric values together. */
std::string pwCKnowledgeSource::IntegerAdder(std::string sIntegerValues){
	int nInput1, nInput2, nSum;
	char sInput[100], sResult[50];

	sIntegerValues = RemoveBrackets(sIntegerValues);
	strcpy(sInput, sIntegerValues.c_str() );
	sscanf(sInput, "%i %i", &nInput1, &nInput2);
	if ( m_nDebug >= 1) {
		std::cout << "sInput:" << sInput << std::endl;
		std::cout << "int1= " << nInput1 << std::endl;
		std::cout << "int2= " << nInput2 << std::endl;
	}
	nSum = nInput1 + nInput2;
	sprintf(sResult,"%i", nSum);
	if ( m_nDebug >= 1)
		std::cout << "Sum = " << sResult << std::endl;
	return sResult;
}
/** Merge a number of string together */
std::string pwCKnowledgeSource::StringMerge(std::string sMergingStrings){
	std::string sToken, sMergedString;
	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	sMergingStrings = RemoveBrackets(sMergingStrings);
	Tokenizer.setString(sMergingStrings);
	sMergedString.erase();
	while ( (sToken = Tokenizer.getNextToken()) != LNVOID ){
		sMergedString += sToken;
	}
	return sMergedString;
}

/** Count how many token is in the given list */
std::string pwCKnowledgeSource::CountToken(std::string sList){
	int nResult;
	char str[100];
	LnTokenizer Tokenizer;

	Tokenizer.setOperators("");
	Tokenizer.setString(sList);
	nResult = Tokenizer.getNumberOfTokens();
	sprintf(str, "%d", nResult);
	return str;
}

/** add the given string element to m_vsTemp */
std::string pwCKnowledgeSource::StringListFormer(std::string sElement){
	sElement = RemoveBrackets(sElement);
	m_vsExternal.push_back(sElement);
	return sElement;
}

/** clear the temp list for the external function. */
std::string pwCKnowledgeSource::ClearStringList(std::string sElement){
	m_vsExternal.clear();
	return sElement;
}

/** Create a string that contains all the elements stored in the external string list. The elements are separated by a space. */
std::string pwCKnowledgeSource::GetFormedStringList(std::string sStr){
	int i, nSize;
	std::string sTemp, sResult;
	nSize = m_vsExternal.size();
	sTemp = m_vsExternal[0];
	sResult += sTemp;
	for (i=1; i<nSize; i++) {
		sTemp = m_vsExternal[i];
		sResult += ("," + sTemp);
	}
	return sResult;
}

/** do the actual data sending to tcp port and wait for the callback. */
int pwCKnowledgeSource::DoSendTCPData(std::string sMessage){
	
	if ( m_nDebug >= 1) {
		std::cout << "Start sending TCP Data... " << std::endl;
		std::cout << "Data to send to BB: " << sMessage << std::endl;
	}
	m_pClient->client.write(sMessage);
	// put the program to sleep until the callback function is called.
    // FIXME: Should definitely sleep for some, otherwise will be using
    // 100% CPU during this wait
	do {
//		 sleep(1);
		if ( m_nDebug >= 2){
			std::cout << "Callback Ready = " << m_bReady << std::endl;
		}
	} while (!m_bReady);
	m_bReady = false;
	SIGIOcallbackPrecondition(m_sCallBackString);
	if ( m_nDebug >= 1) {
		std::cout << "Done sending TCP Data. " << std::endl;
	}
	return 0;
}

/** load data to blackboard directly */
int pwCKnowledgeSource::LoadFileToBB(std::string sContent, int nType, int nInstant){
	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string sToken, sTemp, sData;
	std::string sFileName, sVariables, sBBDataFormat, sPartition, sResult;
	std::vector<pwCVariable> vVariables;
	pwCVariable VarTemp, *pVarTemp;
	FILE *fptr;
	char sLineData[256];
	int i, nLineCount, nVarCount;
	
	// extract the fields from the content string
	sTemp = RemoveBrackets(sContent);
	Tokenizer.setString(sTemp);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sFileName = RemoveBrackets(sToken);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sVariables = RemoveBrackets(sToken);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sBBDataFormat = RemoveBrackets(sToken);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sPartition = RemoveBrackets(sToken);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sResult = RemoveBrackets(sToken);
	
	// create variables
	Tokenizer.setString(sVariables);
	while ( ((sToken = Tokenizer.getNextToken()) != LNVOID) ){
		VarTemp.SetName(sToken);
		vVariables.push_back(VarTemp);
	}
	
	// load line data from file
	// check for insert variables.
	if (nType == 1) {
		sFileName = EvaluateActionContent(sFileName, nInstant);
	}
	sFileName = RemoveBrackets(sFileName);
	sTemp = m_sKSFileDirectory + sFileName;
	fptr = fopen(sTemp.c_str(),"r");
	if (fptr == NULL) return 1; // errors.
	nLineCount = 0;
	while (feof(fptr) == 0)
	{
		fgets(sLineData, 256, fptr);	/* Read next record			*/
		if (feof(fptr) != 0) break;
		Tokenizer.setString(sLineData);
		Tokenizer.setOperators("+*/");
		nVarCount = 0;
		while ( ((sToken = Tokenizer.getNextToken()) != LNVOID) ){
			vVariables[nVarCount].m_vsData.push_back(sToken);
			if ( m_nDebug >= 2) {
				std::cout << "Current line Data elements: " << vVariables[nVarCount].m_vsData[nLineCount] << std::endl;
			}
			nVarCount++;
		}
		if ((int)vVariables.size() != nVarCount) {
			std::cout << "variable size is wrong, check the line file" << std::endl;
			return 1;
		}
		nLineCount++;
	}
	fclose(fptr);			/* Close the file.			*/
	
	// send line data to BB
	for (i=0; i<nLineCount; i++) {
		Tokenizer.setString(sBBDataFormat);
		sData.erase();
		while ( ((sToken = Tokenizer.getNextToken()) != LNVOID) ){
			if (sToken[0] == '~'){
				sToken = sToken.substr(1,(sToken.length())-1); // remove the "~" symbol
				pVarTemp = FindVariable(sToken, &vVariables);
				if (pVarTemp != NULL) {
					sTemp = pVarTemp->m_vsData[i];
				}
				else {
					std::cout << "Error loading data to blackboard : variable not found" << std::endl;
					exit (1); // variable not found
				}
			}
			else {
				sTemp = sToken;
			}
			sData += (sTemp + " ");
		} // while
		if ( m_nDebug >= 2) {
			std::cout << "Sending data to blackboard : Current line Data: " << sData << std::endl;
		}
		DoSendTCPData("add [" + sData + "]" + sPartition);
	} // for
	return 0;
}
/** copy the key words from the given KS to itself. */
int pwCKnowledgeSource::CopyKeyWords(pwCKnowledgeSource* pKS){
		if (pKS == NULL) {
			std::cout << "Internal error, a null pointer is given!" << std::endl;
			exit(1);
		}
		m_nDebug = pKS->m_nDebug;
		m_sKSFileDirectory = pKS->m_sKSFileDirectory;
		m_sKSFileName = pKS->m_sKSFileName;
		m_sKSString = pKS->m_sKSString;
		m_sKSName = pKS->m_sKSName;
		m_sKSType = pKS->m_sKSType;
		m_sInferenceMode = pKS->m_sInferenceMode;
		m_sRuleString = pKS->m_sRuleString;
		m_sFirabilityFlag = pKS->m_sFirabilityFlag;
		m_sPreconditionString = pKS->m_sPreconditionString;
		m_sActionString = pKS->m_sActionString;
		return 0;
}

/** Add the given partition name (if not exist in the list) to the partition list. */
int pwCKnowledgeSource::AddToPartitionList(std::string sPartition){
	int i;
	bool bFound = false;
	
	for (i=0; i<m_vsPartitions.size(); i++) {
		if (m_vsPartitions[i] == sPartition) {
			bFound = true;
			break;
		}
	}
	if (bFound == false) {
		m_vsPartitions.push_back(sPartition);
	}
	return 0;
}
/** check if the given partition is exist in the partition list. */
bool pwCKnowledgeSource::CheckPartitionList(std::string sPartition){
	int i;
	bool bFound = false;

	for (i=0; i<m_vsPartitions.size(); i++) {
		if (m_vsPartitions[i] == sPartition) {
			bFound = true;
			break;
		}
	}
	return bFound;
}
/** a local funcation for displaying message to the output. */
void pwCKnowledgeSource::DisplayText(std::string sMessage){
	m_pClient->DisplayText(sMessage);
}
/** apply operator to the 2 operands, return true or false. */
bool pwCKnowledgeSource::Compare(std::string sContent){
	LnTokenizer Tokenizer;
	// added by P.Wong on 09-09-2002 to reset operator string
	Tokenizer.setOperators("");
	std::string sToken, sTemp, sOperand1, sOperand2, sOperator;
	float fOperand1, fOperand2;
	char str[100];
	
	// extract the fields from the content string
	sTemp = RemoveBrackets(sContent);
	Tokenizer.setString(sTemp);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sOperand1 = RemoveBrackets(sToken);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sOperator = RemoveBrackets(sToken);
	do {
		sToken = Tokenizer.getNextToken();
	} while (sToken.substr(0,2) == "/*");
	sOperand2 = RemoveBrackets(sToken);
	// do the comparing
	fOperand1 = atof( sOperand1.c_str() );
	fOperand2 = atof( sOperand2.c_str() );
/*	strcpy ( str, sOperand1.c_str() );
	sscanf(str, "%f", &fOperand1);
	strcpy ( str, sOperand2.c_str() );
	sscanf(str, "%f", &fOperand2); */
	if (sOperator == "Equal") {
		return (fOperand1 == fOperand2);
	}
	else if (sOperator == "NotEqual") {
		return (fOperand1 != fOperand2);
	}
	else if (sOperator == "LessThan") {
		return (fOperand1 < fOperand2);
	}
	else if (sOperator == "LessThanOrEqual") {
		return (fOperand1 <= fOperand2);
	}
	else if (sOperator == "GreaterThan") {
		return (fOperand1 > fOperand2);
	}
	else if (sOperator == "GreaterThanOrEqual") {
		return (fOperand1 >= fOperand2);
	}
	else {
		std::cout << "Error at Compare : no such operator" << std::endl;
		exit(1);
	}
}
