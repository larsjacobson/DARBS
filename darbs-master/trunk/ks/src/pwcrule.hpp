/***************************************************************************
                          pwcrule.hpp  -  description
                             -------------------
    begin                : Wed Jun 7 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCRULE_H
#define PWCRULE_H

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>

///////////////////////////// local includes /////////////////////////////
#include "pwcknowledgesource.hpp"

class pwCKnowledgeSource;

/**The Class for Rules
  *@author Patrick Wong
  */

class pwCRule {
public: 
	pwCRule();
	~pwCRule();
  /** return the rule string */
  std::string GetRuleFileName();
  /** Set the rule directory name */
  int SetRuleDirectoryName(std::string str);
  /** Set the rule file name. */
  int SetRuleFileName(std::string str);
  /** function that extract the rule variables */
  int CheckKeyWords();
  /** Load rule file into the object. */
  int LoadRuleFile();
  /** this function print the rule variables on the screen. */
  int PrintRule();
  /** function that fires the rule */
  int FireRule();
protected: // Protected attributes
  /** The string that contains the explainations. */
  std::string m_sExplainString;
  /** The string that contians the actions. */
  std::string m_sActionString;
  /** The string that constains the preconditions. */
  std::string m_sPreconditionString;
  /** The Rule name. */
  std::string m_sRuleName;
  /** The directory name of the rule file. */
  std::string m_sRuleFileDirectory;
  /** The Rule file name. */
  std::string m_sRuleFileName;
public:
  /** The knowledge source that contains the rule. */
  pwCKnowledgeSource* m_pKS;
  /** a list of pointers to the prior rules that is for building the dependency graph. */
  std::vector<pwCRule*> m_vPirorRules;
  /** a list of pointers to the post rules that is for building the dependency graph. */
  std::vector<pwCRule*> m_vPostRules;
  /** a list of pointers to the weak prior rules that is for building the dependency graph. */
  std::vector<pwCRule*> m_vWeakPirorRules;
  /** a list of pointers to the weak post rules that is for building the dependency graph. */
  std::vector<pwCRule*> m_vWeakPostRules;
  /** a flag indicates whether the rule has fired. */
  bool m_bFired;
  /** The rule string. */
  std::string m_sRuleString;
};

#endif
