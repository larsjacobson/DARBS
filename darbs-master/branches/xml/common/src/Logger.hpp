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
 * @file Logger.hpp 
 * @brief Header for a simple logger class
 */

#ifndef __LOGGER_HPP
#define __LOGGER_HPP

// Global includes 

// Local includes 
#include <sstream>

// Defines 
#define LOG(text, level) \
{Logger *logger = Logger::getInstance(); std::ostringstream oss; \
 if (level <= logger->getLevel()) logger->log((std::ostringstream&)(oss<<text), level);};

#ifdef ERROR
#undef ERROR
#endif
// Global variables
enum Level {REPORT, ERROR, WARNING, INFO};


// Class definitions

/**
 * @brief A simple logger
 */
class Logger {
public:
    static Logger *getInstance();

    void log(const std::ostringstream &oss, Level level);

    void setLevel(Level level);

    void setLevel(int level);

    Level getLevel();
protected:
    Logger();
private:
    static Logger *instance;
    Level level;
};

#endif
