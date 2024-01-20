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
 * @file LnBlackboard.cpp
 * @brief Central knowledge store and communication backbone for DARBS
 *
 * The blackboard is realised as an STL::map. Each entry, e.g. partition,
 * consists of a STL::string for the partition name and a STL::string with
 * the contents of the partition. This allows us to have an unspecified
 * number of named partitions.
 */

// Global includes
#include <boost/regex.hpp>
#include <stdexcept>

// Local includes
#include "LnBlackboard.hpp"
#include "LnTokenizer.hpp"

// Defines
#define DELIMITERS      " \n\t,;:"// possible delimiters for string tokens
#define LIST_BEGIN      " [ "     // start of list
#define LIST_END        " ] "     // end of list
#define LIST_SEPERATOR  " "       // seperate elements in list
#define ARBS_EMPTY_LIST "[]"      // empty list

// Global variables

// Member functions



/**
 * @brief Empty default constructor
 */
LnBlackboard::LnBlackboard()
{
    // nothing here yet
}

/**
 * @brief Constructor.  Creates a blackboard with a number of empty partitions.
 *
 * @param partition A list containing the names of the partitions to create
 */
LnBlackboard::LnBlackboard( std::string partitions )
{
    setUpBlackboard( partitions);
}

/**
 * @brief Empty destructor.
 */
LnBlackboard::~LnBlackboard()
{
    // nothing here yet
}


/**
 * @brief Parses and processes DARBS expressions.
 *
 * Split the expression up into component parts (command, pattern and
 * partition), then calls parse().  Finally formats and returns the output
 * string.
 *
 * @param expression The DARBS expression to process
 * @return The return string from the blackboard
 */
std::string LnBlackboard::process( std::string expression )
{
    std::string cmd;
    std::string pattern;
    std::string partition;
    std::string buffer;
    LnTokenizer tokenizer( expression );
		// added by P.Wong on 09-09-2002 to reset operator string
		tokenizer.setOperators("");
    int num;

    // get number of tokens in expression
    num = tokenizer.getNumberOfTokens();
    
    // get first token
    cmd = tokenizer.getNextToken(); 

    switch( num )
    {
    case 1:
	// nothing here yet
	break;
    case 2:
	pattern = tokenizer.getNextToken();
	partition = pattern;
	break;
    case 3:
	pattern = tokenizer.getNextToken();
	partition = tokenizer.getNextToken();
	break;
    default:
	// nothing here yet
	break;
    }

    // return results from parser
    buffer = parse( cmd, pattern, partition);

    if( buffer == ARBS_CONFIRM )
    {
	// pass on partition name for broadcast message
	buffer  = "partition ";
	buffer += partition;
	buffer += " changed!";
    }
    
    // eleminate uneccessery delimeters and return answer
    return compress( buffer );
}

/**
 * @brief Takes a separated DARBS expression, and calls the approriate
 * functions for a particular command.
 * 
 * @param cmd The command to execute
 * @param pattern The pattern to use with the command
 * @param partition The partition to work on
 * @return The result of the command
 */
std::string LnBlackboard::parse( std::string cmd, std::string pattern, std::string partition)
{
    try
    {
	// find command and do appropriate action
	if( cmd == ARBS_SETUP_BLACKBOARD )
	{   
	    // if list of partitions, get rid of the brackets
	    strip( partition, BOTH, " {}[]()");
	    
	    // partition blackboard
	    setUpBlackboard( partition );
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_ADD_PARTITIONS )
	{   
	    // if list of partitions, get rid of the brackets
	    strip( partition, BOTH, " {}[]()");
	    
	    // add partitions to blackboard
	    setUpBlackboard( partition, false );
	 
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_ADD )
	{
	    // add to pattern to partition
	    add( pattern, partition);
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_DEL )
	{
	    // delete first occurence of pattern from partition
	    removeFirstMatch( pattern, partition);
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_DEL_ALL )
	{
	    // delete all matches of pattern from partition
	    removeAllMatches( pattern, partition);
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_CLR )
	{
	    // clear partition
	    clearPartition( partition);
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_CLR_ALL )
	{
	    // clear blackboard
	    clearBlackboard();
	    return ARBS_CONFIRM;
	}
	else if( cmd == ARBS_RET )
	{
	    // return first match
	    return getFirstMatch( pattern, partition );
	}
	else if( cmd == ARBS_RET_ALL )
	{
	    // return all matches
	    return getAllMatches( pattern, partition );
	}
	else if( cmd == ARBS_GET )
	{
	    // get contents of partition
	    return getContents( partition);
	}
	else if( cmd == ARBS_GET_PARTITIONS )
	{
	    // get partition names
	    return getPartitionNames();
	}
	else if( cmd == ARBS_PARTITION_EXIST )
	{
	    // check if partition exists
	    if( checkExistenceOfPartition(pattern) == true )
	    {
		// partition exists
		return ARBS_TRUE;
	    }
	    else
	    {
		// partition does not exist
		return ARBS_FALSE;
	    }
	}
	else if( cmd == ARBS_PARTITION_NOT_EXIST )
	{
	    // check if partition exists
	    if( checkExistenceOfPartition(pattern) == true )
	    {
		// partition exists
		return ARBS_FALSE;
	    }
	    else
	    {
		// partition does not exist
		return ARBS_TRUE;
	    }
	}
	else if( cmd == ARBS_ON_PARTITION )
	{	   
	    // check if pattern is present on partition
	    if(  absentOnPartition( pattern, partition ) == true )
	    {
		// pattern is not on partition
		return ARBS_FALSE;
	    }
	    else
	    {
		// pattern is on partition
		return ARBS_TRUE;
	    }
	}
	else if( cmd == ARBS_NOT_ON_PARTITION )
	{	   
	    // check if pattern is present on partition
	    if(  absentOnPartition( pattern, partition ) == true )
	    {
		// pattern is not on partition
		return ARBS_TRUE;
	    }
	    else
	    {
		// pattern is on partition
		return ARBS_FALSE;
	    }
	}
	else if( cmd == ARBS_ON_BLACKBOARD)
	{
	    // check if pattern is on blackboard
	    if( absentOnBlackboard( pattern ) == true )
	    {
		// pattern is not on blackboard
		return ARBS_FALSE;
	    }
	    else
	    {
		// pattern is on blackboard
		return ARBS_TRUE;
	    }
	}
	else if( cmd == ARBS_NOT_ON_BLACKBOARD)
	{
	    // check if pattern is on blackboard
	    if( absentOnBlackboard( pattern ) == true )
	    {
		// pattern is not on blackboard
		return ARBS_TRUE;
	    }
	    else
	    {
		// pattern is on blackboard
		return ARBS_FALSE;
	    }
	}

	else // if token is not a command report error
	{
	    return ARBS_ERROR;
	}
    }
    catch(  std::exception& ex )
    {
	// exception caught, return error
	return ARBS_ERROR;
    }
}

/**
 * @brief Clears the blackboard and creates new empty partitions.
 *
 * @param partitions A list of partition names to create
 * @exception std::runtime_error No partition names were given
 */
void LnBlackboard::setUpBlackboard( std::string partitions, int clr, std::string pattern )
{
    LnTokenizer tokenizer( partitions );
		// added by P.Wong on 09-09-2002 to reset operator string
		tokenizer.setOperators("");
	std::string buffer;

    // if string contains no names throw exception
    if( partitions.empty() )
    {
	throw std::runtime_error(ARBS_NO_NAMES_DEFINED);
    }

    // if clear flag true clear the blackboard
    if( clr == true )
    {
	clearBlackboard();
    }

    // create partitions
    while( (buffer = tokenizer.getNextToken() ) != LNVOID )
    {
	m_blackboard.insert(value_type(buffer, pattern ));
    }
}

/**
 * @brief Delete all partitions on the blackboard.
 */
void LnBlackboard::clearBlackboard()
{
    m_blackboard.erase( m_blackboard.begin(), m_blackboard.end() );
} 

/**
 * @brief Replaces the contents of a partition with empty string.
 *
 * @param partition The name of the partition to be cleared
 * @exception std::runtime_error The partition was not found
 */
void LnBlackboard::clearPartition( std::string partition )
{
    // Find the partition
    partition_type::iterator p = m_blackboard.find( partition );
    
    // if partition found clear contents of partition
    if (p != m_blackboard.end())
    {
	p->second = "";
    }
    else // throw exception
    {
	 throw std::runtime_error(PARTITION_NOT_FOUND);
    }
}

/**
 * @brief Compiles a list with the names of the partitions
 *
 * @return A string containing the list of partition names enclosed by square
 * brackets
 */
std::string LnBlackboard::getPartitionNames()
{
    std::string names;
    std::string partition;

    // get first partition
    partition_type::iterator p = m_blackboard.begin();   

    // if blackboard return
    if( p == m_blackboard.end() )
    {
	return ARBS_EMPTY_LIST;
    }

    // indicate list begin
    names = "[ ";

    do // for every element in list
    {
	// get partition name
	partition = p->first;

	// add partition name to list
	names += partition;

	// separate names in list
	names += LIST_SEPERATOR;

	// increment iterator
	p++;

    } while( p != m_blackboard.end() );

    // close list
    names +="]";

    // return list
    return names;
}

/**
 * @brief Check if a partition with the given name exists.
 *
 * @param name The name of the partition
 * @return Whether the partition exists
 */
int LnBlackboard::checkExistenceOfPartition( std::string name)
{
    int flag = false;    

    // get first partition
    partition_type::iterator p = m_blackboard.begin();   

    // if blackboard return
    if( p == m_blackboard.end() )
    {
	return flag;
    }

    do // for every element in list
    {
	// compare partition with name
	if( name == p->first )
	{
	    // partition with name name exists
	    flag = true;
	}

	// increment iterator
	p++;

    } while( p != m_blackboard.end() );

    // return result
    return flag;
}

/**
 * @brief Checks whether a pattern is absent from the blackboard.
 *
 * @param pattern: The pattern to search for
 * @return Whether the pattern is absent
 * @exception std::runtime_error If no partitions exist
 */
int LnBlackboard::absentOnBlackboard( std::string pattern)
{
    int flag = true;
    std::string buffer;
    std::string partition;

    // get first partition
    partition_type::iterator p = m_blackboard.begin();

    // if blackboard empty throw exception
    if( p == m_blackboard.end() )
    {
	throw std::runtime_error(PARTITION_NOT_FOUND);
    }

    do // for every element in list
    {
	// get partition name
	partition = p->first;

	// if pattern in partition set flag
	if( !absentOnPartition( pattern, partition ))
	{
	    flag = 0;
	}

	// increment iterator
	p++;

    } while( p != m_blackboard.end() );

    return flag;
}

/**
 * @brief Checks whether a pattern is absent from a partition
 *
 * @param pattern The pattern to search for
 * @param partition The partition to search in
 * @return Whether the pattern is absent
 */
int LnBlackboard::absentOnPartition( std::string pattern, std::string partition )
{
    std::string buffer = getAllMatches( pattern, partition);

    if( buffer == ARBS_FALSE )
    {
	return true;
    }
    else
    {
	return false;
    }
}

/**
 * @brief Returns the first matched variable in a pattern on a partition.
 *
 * Loops through each token on the partition calling IsInstance to check
 * whether they match the pattern.  Returns when a match is found.
 *
 * @param pattern The pattern to search for
 * @param partition The partition to search
 * @return A string containing true or false, followed by any matched variables
 * @exception std::runtime_error If the partition is not found
 */
std::string LnBlackboard::getFirstMatch( std::string pattern, std::string partition )
{   
	std::string buffer;
	std::string retval;
	std::string token;
	// added by P.Wong on 25.01.2002
	std::string sTemp;
	bool bStatus;
	
	retval.erase();
	bStatus = false;

	// Find the partition
	partition_type::iterator p = m_blackboard.find( partition );
    
	// if partition found search for matches
	if (p != m_blackboard.end()) {
		// copy partition to buffer
		buffer = p->second;

		// install tokenizer for buffer
		LnTokenizer tokenizer( buffer);
		// added by P.Wong on 09-09-2002 to reset operator string
		tokenizer.setOperators("");

		// search for pattern in buffer
		while( (token = tokenizer.getNextToken() ) != LNVOID )
		{
			// if token is an instance of pattern return token
			if( ( token = isInstance( token, pattern)) != ARBS_FALSE )
			{
				// changed by P.Wong on 25.01.2002
				bStatus = (bStatus || true);
				sTemp = LIST_BEGIN;
				sTemp += LIST_END;
				if (token != sTemp) {
					retval += token;
				}
				break;
			}
			// added by P.Wong on 25.01.2002
			else {
				bStatus = (bStatus || false);
			}
		}
	}
	else // throw exception
	{
		throw std::runtime_error(PARTITION_NOT_FOUND);
	}
	
	// return matches or false string
	if( !bStatus )
	{
		return ARBS_FALSE;
	}
	else
	{
		// create string with list of matches
		token = retval;
		strip (token, BOTH);
		if ( token.empty() ) {
			token  = ARBS_TRUE;
		}
		else {
			token  = ARBS_TRUE;
			token += LIST_BEGIN;
			token += retval;
			token += LIST_END;
		}
		return token;
	}
}

/**
 * @brief Returns all matched variables of a pattern on a partition
 *
 * Loops through each token on the partition calling IsInstance to check
 * whether they match the pattern, and adding the matched strings to a list.
 * When each token has been checked, the list string is returned.
 *
 * @param pattern The pattern to search for
 * @param partition The partition to search on
 * @return A string containing true or false, followed by a list of the
 * matched variables
 * @exception std::runtime_error If the partition is not found
 */
std::string LnBlackboard::getAllMatches( std::string pattern, std::string partition )
{
	std::string buffer;
	std::string retval;
	std::string token;
	// added by P.Wong on 25.01.2002
	std::string sTemp;
	bool bStatus;

	retval.erase();
	bStatus = false;
	// Find the partition
	partition_type::iterator p = m_blackboard.find( partition );
    
	// if partition found seach for matches
	if (p != m_blackboard.end())
	{
		// copy partition to buffer
		buffer = p->second;

		// install tokenizer for buffer
		LnTokenizer tokenizer( buffer);
		// added by P.Wong on 09-09-2002 to reset operator string
		tokenizer.setOperators("");

		// search for pattern in buffer
		while( (token = tokenizer.getNextToken() ) != LNVOID )
		{
			// if token is an instance of pattern add token to retval
			if( (token = isInstance(token, pattern)) != ARBS_FALSE )
			{
				// changed by P.Wong on 25.01.2002
				bStatus = (bStatus || true);
				sTemp = LIST_BEGIN;
				sTemp += LIST_END;
				if (token != sTemp) {
					retval += token;
				}
			}
			// added by P.Wong on 25.01.2002
			else {
				bStatus = (bStatus || false);
			}
		}
	}
	else // throw exception
	{
		// changed by P.Wong on 21/09/2001.
		//When the partition is not found, instead of return error, return false.
		//	throw runtime_error(PARTITION_NOT_FOUND);
		return ARBS_FALSE;
	}

	// return matches or false string
	if( !bStatus )
	{
		return ARBS_FALSE;
	}
	else
	{
		// create string with list of matches
		token = retval;
		strip (token, BOTH);
		if ( token.empty() ) {
			token  = ARBS_TRUE;
		}
		else {
			token  = ARBS_TRUE;
			token += LIST_BEGIN;
			token += retval;
			token += LIST_END;
		}
		return token;
	}
}

/**  
 * @brief Adds a pattern to a partition.  If the partition does not exist, it
 * is created.
 * @param pattern The pattern to added
 * @param partition The partition where pattern will be added
 */
void LnBlackboard::add( std::string pattern, std::string partition )
{
    // Find the partition
    partition_type::iterator p = m_blackboard.find( partition );
    
    // if partition found add pattern to partition
    if (p != m_blackboard.end())
    {
    // FIXME?  Looks like this could result in a race condition if two KS's
    // try to add to a partition at the same time.
	p->second += pattern;
    }
    else // partition not found
    {
	// create partition and add pattern
	setUpBlackboard( partition, false, pattern);
    }
}

/**
 * @brief Removes the first match of a pattern from a partition.
 *
 * @param pattern The pattern to remove
 * @param pattern The partition to search in
 * @exception std::runtime_error If the partition is not found
 */
void LnBlackboard::removeFirstMatch( std::string pattern, std::string partition)
{
    std::string buffer;
    std::string newpartition;
    std::string token;
    int counter=0;

    // Find the partition
    partition_type::iterator p = m_blackboard.find( partition );
    
    // if partition found search for matches
    if (p != m_blackboard.end())
    {
	// copy partition to buffer
	buffer = p->second;

	// install tokenizer for buffer
	LnTokenizer tokenizer( buffer);
	// added by P.Wong on 09-09-2002 to reset operator string
	tokenizer.setOperators("");

	// search for pattern in buffer
	while( (token = tokenizer.getNextToken() ) != LNVOID )
	{
	    // if token is not an instance of pattern or 
	    // counter > 0 add token to partition
	    if( isInstance( token, pattern) == ARBS_FALSE || counter )
	    {
		newpartition+=token;
	    }
	    else
	    {
		counter++;
	    }
	}	
	// update partition
	p->second = newpartition;
    }
    else // throw exception
    {
	throw std::runtime_error(PARTITION_NOT_FOUND); 
    }
}

/**
 * @brief Removes all instances matching a pattern from a partition.
 *
 * @param pattern The pattern to remove
 * @param pattern The partition to search in
 * @exception std::runtime_error If the partition is not found
 */
void LnBlackboard::removeAllMatches( std::string pattern, std::string partition)
{
    std::string buffer;
    std::string newpartition;
    std::string token;

    // Find the partition
    partition_type::iterator p = m_blackboard.find( partition );
    
    // if partition found search for matches
    if (p != m_blackboard.end())
    {
	// copy partition to buffer
	buffer = p->second;

	// install tokenizer for buffer
	LnTokenizer tokenizer( buffer);
	// added by P.Wong on 09-09-2002 to reset operator string
	tokenizer.setOperators("");

	// search for pattern in buffer
	while( (token = tokenizer.getNextToken() ) != LNVOID )
	{
	    // if token is not an instance of pattern add token to partition
	    if( isInstance( token, pattern) == ARBS_FALSE )
	    {
		newpartition+=token;
	    }
	}	
	// update partition
	p->second = newpartition;
    }
    else // throw exception
    {
	throw std::runtime_error(PARTITION_NOT_FOUND); 
    }
}

/**
 * @brief Returns the entire contents of a partition.
 *
 * @param partition The name of the partition
 * @return The contents of the partition
 * @exception std::runtime_error If the partition is not found
 */
std::string LnBlackboard::getContents( std::string partition )
{   
    // Find the partition
    partition_type::iterator p = m_blackboard.find( partition );
    
    // if partition found return contents of partition
    if (p != m_blackboard.end())
    {
	return p->second;
    }
    else // throw exception
    {
	throw std::runtime_error(PARTITION_NOT_FOUND); 
    }
}

/**  
 * @brief Checks whether a string matches a pattern
 *
 * Works through each token in the expression and matches it against the corresponding pattern token.
 * The pattern can contain the following wildcards,  with the matching values returned as a list from
 * the function.
 * Wildcards supported are:
 * - ? - match a word and return matches
 * - ?? - match one or more and return matches
 * - == - match one or more words, but don't return them.
 *
 * @param expression The string to check
 * @param pattern The pattern to match against
 * @return Whether the string matches the pattern
 *
 * @todo If ever a function needed some test cases...
 * @todo Make the regex version return strings formatted correctly
 * @todo Replace the tokenizer version with the regex version if it works the same
 */
std::string LnBlackboard::isInstance( std::string pattern, std::string expression )
{
        std::string token1;
        std::string token2;
        std::string buffer;
        int flag = true;
        // added by P.Wong on 25.01.2002
        int i,j, nItems = 0;
        std::string sTemp;
        std::string token3;

        buffer = LIST_BEGIN;
        // delete first and last chars
        pattern = core( pattern);
        expression = core( expression);


        // SMELL - Function is horrible, and can probably be happily replaced
        // with a couple of choice regexes.
        
        /* Trying to do it with regex
        // First we convert the DARBS wildcard syntax to regex syntax, using
        // yet more regex
        // Regex is horrible, but here goes...
        // We need to match the whole pattern, so add the start and end markers
        std::string nexpression = "^" + expression + "$";
        // Replace ?? with (.*?)
        // .*? matches zero or more of any character, but is non-greedy
        // so doesn't munch through more of the pattern than it needs to.  The
        // extra parentheses make it a subexpression so we can grab the matched
        // value later
        nexpression = boost::regex_replace(nexpression, boost::regex("\\?\\?"), std::string("(.*?)"));
        // Replace ? with (\S*)
        // \S* matches zero or more non-whitespace characters.  The extra
        // parentheses make it a subexpression so we can grab the matched value
        // later.
        nexpression = boost::regex_replace(nexpression, boost::regex("\\?"), std::string("(\\\\S*)"));
        // Replace == with .*?
        // .*? matches zero or more of any character non-greedily.  This is
        // basically used to munch through anything which we are not looking
        // to return
        nexpression = boost::regex_replace(nexpression, boost::regex("=="), std::string(".*?"));

        // Now we use our newly constructed regex to match the variables in
        // the pattern
        std::cout << nexpression << std::endl;
        boost::regex re(nexpression);
        boost::match_results<std::string::const_iterator> matches;
        bool haveMatch = boost::regex_match(pattern, matches, re);
        if (haveMatch) {
            for (int i = 1; i < matches.size(); i++) {
                std::string match(matches[i].first, matches[i].second);
                std::cout << "matches[" << i << "] = " << match << std::endl;
            }
        }
        */

		// install tokenizers
		LnTokenizer tokenizer1( pattern );
		LnTokenizer tokenizer2( expression );
		// added by P.Wong on 09-09-2002 to reset operator string
		tokenizer1.setOperators("");
		tokenizer2.setOperators("");

		// compare items in lists
		while( (token1 != LNVOID) && (token1 != LNVOID) )
		{
			// get next items
			token1 = tokenizer1.getNextToken();
			token2 = tokenizer2.getNextToken();

			// added by P.Wong on 25.01.2002
			// to support the Wild Card "==" operator.
			// if item2 is a wild card
			if ( token2 == "==" ){
				if( tokenizer2.getNumberOfTokens() == 0 )
				{
					// no more tokens in item2, so condition must be fullfilled
					buffer += LIST_END;
					return buffer;
				}
				else // some tokens remaining
				{
					token2 = tokenizer2.getNextToken();
					// if the are variables after the wildcard, get the next normal item.
					nItems = 0;
					while ( ( token2 != LNVOID ) && (token2[0] == '?') && (token2[1] != '?') ) {
						token2 = tokenizer2.getNextToken();
						nItems++;
					}
					// shift out the ignore items.
					sTemp.erase();
					while( (token1 != token2) && (token1 != LNVOID) )
					{
						sTemp += token1;
						sTemp += LIST_SEPERATOR;
						token1 = tokenizer1.getNextToken();
					}
					// add the last nItem of sTemp to buffer
					if (nItems > 0) {
						LnTokenizer tokenizer3( sTemp );
						// added by P.Wong on 09-09-2002 to reset operator string
						tokenizer3.setOperators("");
						i = tokenizer3.getNumberOfTokens();
						for (j=0; j<i; j++) {
							token3 = tokenizer3.getNextToken();
							if (j >= i-nItems) {
								buffer += token3;
								// separate items in buffer
								buffer += LIST_SEPERATOR;
							}
						} // for
					} // if
				} // else
			} // end added by P.Wong
			// if item2 is not a variable
			else {
				if( token2[0] != '?' )
				{
					// if not equal
					if( token1 != token2 )
					{
						// pattern is not an instance of expression
						return ARBS_FALSE;
					}
				}
				else if( token2[1] == '?' ) // item2 is a list variable
				{
					if( tokenizer2.getNumberOfTokens() == 0 )
					{
						// no more tokens in item2, so condition must be fullfilled
						buffer += LIST_BEGIN;
						buffer += token1;
						buffer += tokenizer1.getRemains();
						buffer += LIST_END;
						buffer += LIST_END;
						return buffer;
					}
					else // some tokens remaining
					{
						token2 = tokenizer2.getNextToken();
						// added by P.Wong on 25.01.2002
						// if the are variables after the list variable, get the next normal item.
						nItems = 0;
						while ( ( token2 != LNVOID ) && (token2[0] == '?') && (token2[1] != '?') ) {
							token2 = tokenizer2.getNextToken();
							nItems++;
						}
						
						
						sTemp.erase();
						while( token1 != token2 && token1 != LNVOID )
						{
							sTemp += token1;
							sTemp += LIST_SEPERATOR;
							token1 = tokenizer1.getNextToken();
						}
						buffer += LIST_BEGIN;
						LnTokenizer tokenizer3( sTemp );
						// added by P.Wong on 09-09-2002 to reset operator string
						tokenizer3.setOperators("");
						i = tokenizer3.getNumberOfTokens();
						for (j=0; j<i-nItems; j++) {
							token3 = tokenizer3.getNextToken();
							buffer += token3;
							// separate items in buffer
							buffer += LIST_SEPERATOR;
						} // for
						buffer += LIST_END;
						// add the last nItem (variable values) of sTemp to buffer
						if (nItems > 0) {
							token3 = tokenizer3.getRemains();
							buffer += token3;
						} // if
						// end added by P.Wong
					}
				} //else if
				else // item1 is instance of variable item2
				{
					// add item1 to buffer
					buffer += token1;
					// separate items in buffer
					buffer += LIST_SEPERATOR;
				}
			}
		} // while

		// both tokens should be empty otherwise one list is
		// longer than the other and hence pattern is not an
		// instance of expression
		if( token1 != token2 )
		{
			flag = false;
		}

		// if pattern is an instance return instances list
		if( flag == true )
		{
			return buffer+LIST_END;
		}
		else
		{
			// return false
			return ARBS_FALSE;
		}
}

/**
 * @brief Removes the first and last characters of a string to get rid of surrounding brackets.
 *
 * @param strng The string to process
 * @return The substring with the characters removed
 */
std::string LnBlackboard::core( std::string strng )
{
    // return substring 
    return strng.substr(1,strng.length()-2);
}

/**
 * @brief Returns a substring of a string without unneccessery new line and tab characters.
 *
 * @param strng The string to process
 * @return The substring without the deliminators
 */
std::string LnBlackboard::compress( std::string strng )
{
    std::string delims = DELIMITERS;
    size_t idx;

    // start at first character in string
    idx=0;

    // for each character in string
    while(idx<strng.length() )
    {
	// if character is a delimeter
	if( delims.find( strng[idx] ) != -1 )
	{
	    // remove any following delimeters
	    while( (delims.find( strng[idx+1] ) != -1) && (idx+1<strng.length()))
	    {
		strng.erase(idx,1);
	    }
	}
	// next character
	idx++;	
    }

    // return string without unneccessery delimeters
    return strng;
}
