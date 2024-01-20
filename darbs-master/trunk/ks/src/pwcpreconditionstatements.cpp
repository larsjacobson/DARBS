/***************************************************************************
                          pwcpreconditionstatements.cpp  -  description
                             -------------------
    begin                : Mon Sep 11 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>

///////////////////////////// local includes /////////////////////////////
#include "pwcpreconditionstatements.hpp"
#include "pwcvariable.hpp"

pwCPreconditionStatements::pwCPreconditionStatements(){
}

pwCPreconditionStatements::pwCPreconditionStatements(int nID){
	m_nID = nID;
}

pwCPreconditionStatements::pwCPreconditionStatements(int nID, std::string sBBCommand, std::string sBBContent, std::string sBBPanal){
	m_nID = nID;
	m_sBBCommand = sBBCommand;
	m_sBBContent = sBBContent;
	m_sBBPanal = sBBPanal;
}

pwCPreconditionStatements::~pwCPreconditionStatements(){
}

/** get the object ID */
int pwCPreconditionStatements::GetID(){
	return m_nID;
}

/** set callback string */
int pwCPreconditionStatements::SetCallbackString(std::string sCallbackString){
	m_sCallbackString = sCallbackString;
}

/** form and return the tcp command */
std::string pwCPreconditionStatements::GetTCPCommand(){
	return (m_sBBCommand + " " + m_sBBContent + " " + m_sBBPanal);
}

/** set the identifer */
void pwCPreconditionStatements::SetID(int nID){
	m_nID = nID;
}

/** store the insert variable's values and the statement's boolean values. */
int pwCPreconditionStatements::StoreCallbackResults(int nInstant, bool bStatus){
	int i,j, nSize;
	std::string sName, sData;
	pwCVariable *pVar, TempVar;	
	// add the insert variable and its instantiations to the variable list.
	nSize = m_vInsertVariables.size();
	for (i=0; i<nSize; i++) {
		sName = m_vInsertVariables[i].GetName();
		sData = m_vInsertVariables[i].m_vsData[m_nInstantIndex];
		pVar = m_pKS->FindVariable(sName, &m_vVariables);
		if (pVar == NULL) {
			TempVar.clear();
			TempVar.SetName( sName );
			// added by P.Wong on 6th September 2002.
			// m_vnInstantIndex contains a Instantiation index of each data in m_vsData.
			TempVar.m_vnInstantIndex.clear();
			for (j=0; j<nInstant; j++) {
				// save variable's instantiation.
				TempVar.m_vsData.push_back(sData);
				TempVar.m_vnInstantIndex.push_back(m_nInstantIndex);
			}
			m_vVariables.push_back(TempVar);
		}
		else {
			for (j=0; j<nInstant; j++) {
				// save variable's instantiation.
				pVar->m_vsData.push_back(sData);
				pVar->m_vnInstantIndex.push_back(m_nInstantIndex);
			}
		}
	}
	// save boolean values.
	for (j=0; j<nInstant; j++) {
		m_vbCallbackConditions.push_back(bStatus);
	}
	return 0;
}
