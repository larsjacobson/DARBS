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
 * @file KnowledgeSource.hpp
 * @brief Header file for a Knowledge Source
 */

#ifndef __KNOWLEDGE_SOURCE_HPP
#define __KNOWLEDGE_SOURCE_HPP

// Global includes
#include <string>
#include <map>
#include <set>
#include <boost/filesystem.hpp>

// Local includes
#include "TcpClient.hpp"
#include "ClientTypes.hpp"
#include "AbstractConditional.hpp"

// Namespace

// Defines

// Type definitions

// Global variables
enum State {RUNNING, PAUSED, STOPPED};

// Class definitions

/**
 * @brief A knowledge source
 */
class KnowledgeSource : public AbstractConditional {
public:

    // construction
    KnowledgeSource(TcpClient &client, boost::filesystem::path path);

    // destruction
    virtual ~KnowledgeSource();
    
    // public access
    std::string call(std::string libraryName, std::string function,
                     std::string arg);

    virtual void handleNotification(xml_node &notification);

    virtual void notifyExistance();
    virtual void notifyInfo() {};
    virtual void notifyState();
    virtual void notifyTimesFired();

    void setState(State s);
    State getState();

    void setNeedStateCheck(bool check);
    bool getNeedStateCheck();

    State requestState();
    void watchState();

protected:
    LibraryMap libraries;
    State state;
    bool needStateCheck;

    boost::mutex stateMutex;
    std::set<std::string> changedPartitions;
private:

};


#endif
