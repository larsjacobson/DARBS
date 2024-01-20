/***************************************************************************
                          pwcvariable.cpp  -  description
                             -------------------
    begin                : Thu Sep 7 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

///////////////////////////// local includes /////////////////////////////
#include "pwcvariable.hpp"

pwCVariable::pwCVariable(){
}
/** constructor: initialise the variable name */
pwCVariable::pwCVariable(std::string sName){
	m_sName = sName;
}
pwCVariable::~pwCVariable(){
}
/** return the number of the elements in data */
int pwCVariable::GetSize(){
	return m_vsData.size();
}
/** return variable name */
std::string pwCVariable::GetName(){
	return m_sName;
}
/** set variable name */
void pwCVariable::SetName(std::string sName){
	m_sName = sName;
}
/** clear the content of the variable object */
void pwCVariable::clear(){
	m_sName.erase();
	m_vsData.clear();
	m_vnStatementID.clear();
}
/** Print the contents of the variable */
int pwCVariable::PrintData(){
	int i;
	std::cout << "Variable " << m_sName << " contains: ";
	for (i=0; i<m_vsData.size(); i++) {
		std::cout << m_vsData[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Statement IDs" << ": ";
	for (i=0; i<m_vnStatementID.size(); i++) {
		std::cout << m_vnStatementID[i] << " ";
	}
	std::cout << std::endl;
	
	return 0;
}
