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
 * @file Logger.cpp
 * @brief A simple multi-level logger
 *
 * Logging is generally done using the LOG function defined in the header
 */

// Global includes
#include <iostream>

// Local includes
#include "Logger.hpp"

// Namespaces

// Defines

// Global variables

// Member functions

/**
 * @brief Empty constructor
 */
Logger::Logger() {
    level = WARNING;
}

/**
 * @brief Return the logger, creating it if necessary
 */
Logger *Logger::getInstance() {
    if (instance == 0) {
        instance = new Logger();
    }
    return(instance);
}

void Logger::log(const std::ostringstream& oss, Level messageLevel) {
    std::string type;
    if (messageLevel == REPORT) {
        type = "REPORT: ";
    }
    else if (messageLevel == INFO) {
        type = "INFO: ";
    }
    else if (messageLevel == WARNING) {
        type = "WARNING: ";
    }
    else if (messageLevel == ERROR) {
        type = "ERROR: ";
    }
    std::cout << type << oss.str() << std::endl;
}

void Logger::setLevel(Level level) {
    this->level = level;
}

void Logger::setLevel(int level) {
    this->level = (Level) level;
}

Level Logger::getLevel() {
    return(level);
}

Logger *Logger::instance=0;


