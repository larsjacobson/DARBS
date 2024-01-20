/***************************************************************************
                          pwcinstantiation.cpp  -  description
                             -------------------
    begin                : Thu Oct 25 2001
    copyright            : (C) 2001 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#include "pwcinstantiation.hpp"

pwCInstantiation::pwCInstantiation(){
}
pwCInstantiation::~pwCInstantiation(){
}
/** Clear the variable and condition list. */
void pwCInstantiation::clear(){
	m_vVariables.clear();
	m_vConditions.clear();
}
