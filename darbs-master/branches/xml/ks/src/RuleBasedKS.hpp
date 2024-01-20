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
 * @file RuleBasedKS.hpp
 * @brief Header file for a rule-based Knowledge Source
 */

#ifndef __RULE_BASED_KS_HPP
#define __RULE_BASED_KS_HPP

// Global includes
#include <string>

// Local includes
#include "pugixml.hpp"
#include "KnowledgeSource.hpp"
#include "TcpClient.hpp"

// Namespace
using namespace pugi;

// Defines

// Type definitions

// Global variables
class Rule;

/**
 * @brief A rule based Knowledge Source.
 *
 * Rule based Knowledge Sources consist of a precondition, an action and any
 * number of ordered rules.  If the precondition evaluates true, each of the
 * rules are evaluated in order, and finally the actions are evaluated.
 */
class RuleBasedKS : public KnowledgeSource
{
public:

    // construction
    RuleBasedKS(TcpClient &client, boost::filesystem::path path);

    // destruction
    ~RuleBasedKS();
    
    // public access
    void run();

    void parse(xml_document &ksDoc);

    void notifyInfo();
    void notifyRules();

protected:

private:
    std::vector<Rule *> rules;
};


#endif
