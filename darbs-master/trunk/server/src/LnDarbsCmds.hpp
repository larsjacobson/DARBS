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
 * @file LnDarbsCmds.hpp
 * @brief Header file containing the DARBS command definitions
 */
#ifndef __LN_DARBS_CMDS_HPP
#define __LN_DARBS_CMDS_HPP

// Global includes

// Local includes

// Defines

// DARBS response
#define DARBS_ERROR                  "error"
#define DARBS_PARTITION_NOT_FOUND    "partition not found!"
#define DARBS_NO_PARTITION_SPECIFIED "$nps"
#define DARBS_COMMAND_NOT_FOUND      "command not found"

#define DARBS_CONFIRM                "confirm"
#define DARBS_FALSE                  "false"
#define DARBS_TRUE                   "true"
#define DARBS_PARTITION_CHANGED      "partition_changed"

// DARBS commands

// manage partitions
#define DARBS_DELETE_PARTITION     "dpar"
#define DARBS_CREATE_PARTITION     "add_partitions"
#define DARBS_CLEAR_PARTITION      "clr_partition"
#define DARBS_CLEAR_BOARD          "clr_board"

// change contents of partition
#define DARBS_ADD_TO_PARTITION     "add"
#define DARBS_DELETE_FIRST         "del_first"
#define DARBS_DELETE_ALL           "del_all"
#define DARBS_REPLACE_FIRST        "replace_pattern"
#define DARBS_REPLACE_ALL          "ramp"
#define DARBS_REPLACE_PARTITION    "repp"

// check and return patterns
#define DARBS_PATTERN_ON_PARTITION     "on_partition"
#define DARBS_PATTERN_ON_BOARD         "on_blackboard"
#define DARBS_GET_FIRST                "ret_first"
#define DARBS_GET_ALL                  "ret_all"
#define DARBS_GET_CONTENTS             "get_contents"
#define DARBS_GET_PARTITIONS           "get_partitions"
#define DARBS_PARTITION_EXIST          "partition_exist"
#define DARBS_PARTITION_NOT_EXIST      "partition_not_exist"
#define DARBS_PATTERN_NOT_ON_PARTITION "not_on_partition"
#define DARBS_PATTERN_ON_BLACKBOARD    "ponb"
#define DARBS_NOT_ON_BLACKBOARD        "not_on_blackboard"

// Type definitions

// Global variables

// Class definitions


#endif __LN_DARBS_CMDS
