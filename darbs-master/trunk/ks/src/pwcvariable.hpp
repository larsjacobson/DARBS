/***************************************************************************
                          pwcvariable.hpp  -  description
                             -------------------
    begin                : Thu Sep 7 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCVARIABLE_H
#define PWCVARIABLE_H

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>

///////////////////////////// local includes /////////////////////////////

/**objects for containing the dynamic rule variables
  *@author Patrick Wong
  */

class pwCVariable {
public: 
	pwCVariable();
	pwCVariable(std::string sName);
	~pwCVariable();
  /** return the number of the elements in data */
  int GetSize();
  /** return variable name */
  std::string GetName();
  /** set variable name */
  void SetName(std::string sName);
  void clear();
  /** Print the contents of the variable */
  int PrintData();
protected: // Protected attributes
  /** Variable name */
  std::string m_sName;
public: // Public attributes
  /** a list of string data of the named variable */
  std::vector<std::string> m_vsData;
  /** the ID of the statement that contains the variable. */
  std::vector<int> m_vnStatementID;
  /** For insert variables, this can be used to store the instantiation index of the match variable that induce the data values to this insert variable.
It is used to test whether a instantiation is valid at the elimination process of the CreateInstantiation(). */
  std::vector<int> m_vnInstantIndex;
};

#endif
