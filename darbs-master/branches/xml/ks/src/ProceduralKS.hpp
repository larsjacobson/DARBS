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
 * @file ProceduralKS.hpp
 * @brief Header file for a procedural Knowledge Source
 */

#ifndef __PROCEDURAL_KS_HPP
#define __PROCEDURAL_KS_HPP

// Global includes
#include <string>
#include <boost/filesystem.hpp>

// Local includes
#include "pugixml.hpp"
#include "KnowledgeSource.hpp"
#include "TcpClient.hpp"

// Namespace
using namespace pugi;

// Defines

// Type definitions

// Global variables

/**
 * @brief A simple procedural Knowledge Source
 *
 * Procedural Knowledge Sources are simple precondition/action pairs which
 * contain a run command for executing functions contained in an external
 * shared library
 */
class ProceduralKS : public KnowledgeSource
{
public:

    // construction
    ProceduralKS(TcpClient &client, boost::filesystem::path path);

    // destruction
    ~ProceduralKS();
    
    // public access
    void run();

    void notifyInfo();

protected:

private:
};


#endif
