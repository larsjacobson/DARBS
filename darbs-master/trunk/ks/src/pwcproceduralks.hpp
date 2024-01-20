/***************************************************************************
                          pwcproceduralks.hpp  -  description
                             -------------------
    begin                : Thu Mar 22 2001
    copyright            : (C) 2001 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCPROCEDURALKS_H
#define PWCPROCEDURALKS_H

///////////////////////////// local includes /////////////////////////////
#include "pwcknowledgesource.hpp"

/**Procedural Knowledge source
  *@author Patrick Wong
  */

class pwCProceduralKS : public pwCKnowledgeSource  {
public: 
	pwCProceduralKS();
	~pwCProceduralKS();
  /** This function initialises the procedural KS object. */
  int Initialise();
  /** Inference engine */
  int Inference();
};

#endif
