/***************************************************************************
                          pwcproceduralks.cpp  -  description
                             -------------------
    begin                : Thu Mar 22 2001
    copyright            : (C) 2001 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/
#include <iostream>
#include <cstdlib>

///////////////////////////// local includes /////////////////////////////
#include "pwcproceduralks.hpp"

pwCProceduralKS::pwCProceduralKS(){
}
pwCProceduralKS::~pwCProceduralKS(){
}
/** This function initialise the procedural KS object */
int pwCProceduralKS::Initialise(){
	return 0;
}
/** Inference engine */
int pwCProceduralKS::Inference(){
	int i, nInstantiations;
	bool bShow = false;
	
	m_nStatementID = 0;
	nInstantiations = CreateInstantiations(m_sPreconditionString);	
	// Firing each instantiations
	for (i=0; i<nInstantiations; i++) {
    /* m_nStatementIndex should be set before this EvaluatePrecondition() is called. */
    m_nStatementIndex = 0;
		m_bKSFired = EvaluatePrecondition(m_sPreconditionString ,i);
		if ( m_nDebug >= 1){
			std::cout << "Fire flag = " << m_bKSFired << std::endl;
		}
		// check for the KS restarting signal.
		if (m_bKSRestart) {
			DisplayText("Tasks interruped. KS is now restarted.");
			m_bKSRestart = false;
			break;
		}
		if (m_bKSFired) {
			bShow = true;
			if ( EvaluateAction(m_sActionString, i) ) {
				std::cout << "error doing action" << std::endl;
				exit (1);
			}
		}
	}
	if (bShow) {
	 	DisplayText("Tasks Completed. KS is now suspended.");
	 	bShow = false;
	}
	return 0;
}
