/***************************************************************************
                          pwcruleks.hpp  -  description
                             -------------------
    begin                : Wed Mar 21 2001
    copyright            : (C) 2001 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCRULEKS_HPP
#define PWCRULEKS_HPP

#include "pwcknowledgesource.hpp"

/** class for rule-baesd knowledge sources.
  *@author Patrick Wong
  */
class pwCRule;

class pwCRuleKS : public pwCKnowledgeSource  {
public: 
	pwCRuleKS();
	~pwCRuleKS();
  /** This function initialises the KS object. */
  int Initialise();
  /** get no. of rules. */
  int GetNoOfRules();
  /** Function that creates rule objects */
  int CreateRules();
  /** the function which starts the rule firing procedure. */
  int FireRules();
  /** the actions provoke when the KS is fired. */
  int DoAction(int nInstantiations);
  /** Inference engine */
  int Inference();
protected: // Protected attributes
  /** No. of rules in this KS */
	int m_nNoOfRules;
  /**   a pointer to the rule that is being processed. */
  pwCRule* m_pCurrentRule;
  /** A vector containing a list of rules for this KS. */
public: // public attributes
  std::vector<pwCRule> m_vListOfRules;
};

#endif
