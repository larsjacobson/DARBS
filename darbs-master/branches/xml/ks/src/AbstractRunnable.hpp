// $Id$

/** 
 * Copyright (C) 2010 Adrian Hopgood et al.
 *
 * Licensed under the Academic Free License version 3.0
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 */

/**
 * @file AbstractRunnable.hpp 
 * @brief An abstract base class for Preconditions and Actions
 */

#ifndef __ABSTRACT_RUNNABLE_HPP
#define __ABSTRACT_RUNNABLE_HPP

// Global includes 

// Local includes 
#include "pugixml.hpp"

// Namespaces
using namespace pugi;

// Defines 

// Global variables

// Class definitions
class AbstractConditional;

/**
 * @brief An abstract base class for Preconditions and Actions.
 * 
 * Contains the functions for variable substitution, request storage etc.
 */
class AbstractRunnable {
public:
    AbstractRunnable(AbstractConditional *conditional);
    ~AbstractRunnable();

    virtual void addChild(xml_node &child) {};

    virtual bool evaluate() {};

    virtual bool evaluateRequest() {};

    virtual std::string getVariable(std::string name) {};

    virtual void addVariable(std::string variableName, std::string value) {};

    virtual void substitutePatterns(xml_node &requestSub);

    virtual void substituteVariables(xml_node &pattern);

    virtual std::string replaceVariable(const boost::cmatch& match);

    virtual void reset() {};

    virtual bool run(xml_node requestSub);

    virtual void report(xml_node requestSub);



protected:

    std::vector<AbstractRunnable *> children;
    xml_document requestDoc;
    xml_node request;
    AbstractConditional *conditional;
private:
};

#endif
