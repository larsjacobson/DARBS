/***************************************************************************
                          pwcpreconditionstatements.hpp  -  description
                             -------------------
    begin                : Mon Sep 11 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCPRECONDITIONSTATEMENTS_H
#define PWCPRECONDITIONSTATEMENTS_H


/**it contains the BB commands and the corresponding return values
  *@author Patrick Wong
  */

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>

///////////////////////////// local includes /////////////////////////////
#include "pwcvariable.hpp"
#include "pwcknowledgesource.hpp"

class pwCPreconditionStatements {
public: 
	pwCPreconditionStatements();
	pwCPreconditionStatements(int nID);
	pwCPreconditionStatements(int nID, std::string sBBCommand, std::string sBBContent, std::string sBBPanal);
	~pwCPreconditionStatements();
  /** set callback string */
  int SetCallbackString(std::string sCallbackString);
  /** get the object ID */
  int GetID();
  /** form and return the tcp command */
  std::string GetTCPCommand();
  /** set the identifer */
  void SetID(int nID);
  /** store the insert variable's values and the statement's boolean values. */
  int StoreCallbackResults(int nInstant, bool bStatus);
protected: // Protected attributes
  /** the identifier of the object */
  int m_nID;
  /** BB command */
  std::string m_sBBCommand;
  /** BB content */
  std::string m_sBBContent;
  /** BB Panal */
  std::string m_sBBPanal;
  /** BB callback string */
  std::string m_sCallbackString;
public: // Public attributes
  /** a list of callback conditions. */
  std::vector<bool> m_vbCallbackConditions;
  /** it represents the boolean states of all the combinations of the precondition statements.
	it is used by the EvaluatePrecondition(). */
  std::vector<bool> m_vbCombinedConditions;
  /** a list containing the run time variables in this statement. */
  std::vector<pwCVariable> m_vVariables;
  /** a list containing the insert variables in this statement. */
  std::vector<pwCVariable> m_vInsertVariables;
  /** a list containing the match variables in this statement. */
  std::vector<pwCVariable> m_vMatchVariables;
  /** The index number for the current instantiation. */
  int m_nInstantIndex;
  /** pointer to the knowledge source. */
  pwCKnowledgeSource* m_pKS;
};

#endif
