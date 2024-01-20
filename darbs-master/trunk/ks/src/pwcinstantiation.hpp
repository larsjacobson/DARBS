/***************************************************************************
                          pwcinstantiation.hpp  -  description
                             -------------------
    begin                : Thu Oct 25 2001
    copyright            : (C) 2001 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCINSTANTIATION_HPP
#define PWCINSTANTIATION_HPP

///////////////////////////// local includes /////////////////////////////
#include "pwcvariable.hpp"

/**It contains a variable list and a boolean condition list.
  *@author Patrick Wong
  */

class pwCInstantiation {
public: 
	pwCInstantiation();
	~pwCInstantiation();
  /** Clear the variable and condition list. */
  void clear();
public: // Public attributes
  /** A variable list containing a set of variables and their values for this instantiation. */
  std::vector<pwCVariable> m_vVariables;
  /** A boolean list containing a set of boolean results from the precondition statements for this instantiation. */
  std::vector<bool> m_vConditions;
};

#endif
