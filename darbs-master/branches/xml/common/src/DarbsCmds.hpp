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
 * @file DarbsCmds.hpp
 * @brief Header file containing the DARBS command definitions
 */
#ifndef __DARBS_CMDS_HPP
#define __DARBS_CMDS_HPP

// Global includes

// Local includes

// Defines

// DARBS response
const std::string DARBS_ERROR = "error";
const std::string DARBS_PARTITION_NOT_FOUND = "partition not found!";
const std::string DARBS_NO_PARTITION_SPECIFIED = "$nps";
const std::string DARBS_COMMAND_NOT_FOUND = "command not found";

const std::string DARBS_CONFIRM = "confirm";
const std::string DARBS_FALSE = "false";
const std::string DARBS_TRUE = "true";
const std::string DARBS_PARTITION_CHANGED = "partition_changed";

// DARBS commands

// manage partitions
const std::string DARBS_CLEAR = "clear";

// change contents of partition
const std::string DARBS_ADD = "add";
const std::string DARBS_DELETE = "delete";
const std::string DARBS_REPLACE = "replace";

// check and return patterns
const std::string DARBS_PRESENT = "present";
const std::string DARBS_GET = "get";

// KS commands
const std::string DARBS_RUN = "run";
const std::string DARBS_COMPARE = "compare";
const std::string DARBS_REPORT = "report";

// Type definitions

// Global variables

// Class definitions


#endif
