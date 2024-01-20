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
 * @file LnTokenizer.cpp
 * @brief String tokenizer for DARBS
 */

// Global includes
#include <stdio.h>
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

// Local includes
#include "LnTokenizer.hpp"

// Defines
#define COMMENT_END_NOT_FOUND "End of comment not found"


// Global variables


// Member functions

/**
 * @brief Default constructor.  Sets default whitespace, bracket and operator
 * characters.
 */
LnTokenizer::LnTokenizer()
{
    // set default whitespace
    setWhitespace( WHITESPACE);

    // set default brackets
    setBrackets( BRACKETS);

    // set default operators
    setOperators( OPERATORS );
}

/**
 * @brief Construtor. Sets default whitespace, bracket and operator
 * characters, and sets the internal buffer to string buffer.
 *
 * @param buffer The string to be tokenized
 */
LnTokenizer::LnTokenizer( std::string buffer )
{
    // set string to be tokenized
    setString( buffer );

    // set default whitespace
    setWhitespace( WHITESPACE);

    // set default brackets
    setBrackets( BRACKETS);

    // set default operators
    setOperators( OPERATORS );
}

/**
 * @brief Empty destructor
 */
LnTokenizer::~LnTokenizer()
{
    // nothing here yet
}

/**
 * @brief Set the characters used as delimiters, i.e. for separating tokens in
 * strings.
 *
 * @param whitespace A string containing the whitespace characters
 */
void LnTokenizer::setWhitespace( std::string whitespace )
{
    m_whitespace=whitespace;
}

/**
 * @brief Set the characters used as brackets to group expressions, e.g.
 * to make lists.
 *
 * @param brackets A string containing pairs of characters to use as brackets
 */
void LnTokenizer::setBrackets( std::string brackets )
{
    m_brackets=brackets;
}

/**
 * @brief Sets the characters used as operators
 * 
 * An operator character is used to break down a string into expressions that
 * can be used for further processing, e.g. in a parser for a calculator.
 *
 * @param operators A string containing the characters to use as operators
 */
void LnTokenizer::setOperators( std::string operators )
{
    m_operators=operators;
}

/**
 * @brief Sets the string to be tokenized.
 *
 * @param buffer The string to be tokenize
 */
void LnTokenizer::setString( std::string buffer )
{
    // set string to be tokenized
    m_string = buffer;
}

/**
 * Cuts off the first token from internal buffer and returns it.
 *
 * Uses m_whitespace, m_brackets and m_operators as delimiters.
 *
 * @return String containing the next token
 */
std::string LnTokenizer::getNextToken()
{
	size_t cp;
	size_t idx;
	bool bDebug=false;

	int	openedBracketsCounter;
	int closedBracketsCounter;

	std::string token;
	
	if (bDebug) {
		std::cout << "debugging tokeniser..." << std::endl;
		std::cout << "<debug> m_string = " << m_string << std::endl;
	}
	// strip leading and tailing white space
	strip( m_string, BOTH, m_whitespace);

	if (bDebug) {
		std::cout << "after stripping" << std::endl;
		std::cout << "<debug> m_string = " << m_string << std::endl;
	}
	// is there anything left?
	if( m_string.length() < 1 ) // no
	{
		return LNVOID; // buffer is empty
	}
	// else get first token
  // if token starts with an open bracket,token is itself a list
	if( isOpenBracket( m_string[0]) )
	{
		// set bracket counters
		closedBracketsCounter = 0;
		openedBracketsCounter = 1;
	
		// find corresponding closing bracket
		idx = 1;
		while( ( openedBracketsCounter > closedBracketsCounter ) && ( idx < m_string.length() ))
		{
			if( isOpenBracket( m_string[idx] ) )
	    {
				openedBracketsCounter++;
			}
			else if( isCloseBracket( m_string[idx] ))
	    {
				closedBracketsCounter++;
	  	}
	    idx++;
		}
		token = m_string.substr(0,idx);
		m_string.erase(0,idx);
	}
	else if( m_string.substr(0,2) == "/*" ) // C-style comment
	{
		// find corresponding tag
		cp = m_string.find("*/");

		// if found
		if( cp !=-1 )
		{
	    token = m_string.substr(0,cp+2);
	    m_string.erase(0,cp+2);
			if (bDebug) {
				std::cout << "token is a comment" << std::endl;
				std::cout << "<debug> token = " << token << std::endl;
			}
		}
		else
		{
	     throw std::runtime_error(COMMENT_END_NOT_FOUND);
		}
	}
	else // token is a keyword or operator
	{
		cp = m_string.find_first_of( m_whitespace+m_brackets+m_operators );
		if( cp != -1 ) // delimiter was found
		{
			if( m_operators.find(m_string[cp]) != -1 ) // token is an operator
			{
				token = m_string.substr(0,1);
				m_string.erase(0,1);
				if (bDebug) {
					std::cout << "token is an operator" << std::endl;
					std::cout << "<debug> token = " << token << std::endl;
				}
			}
			else // token is keyword
	    {
				token = m_string.substr(0, cp);
				m_string.erase(0,cp);
				if (bDebug) {
					std::cout << "token is a keyword" << std::endl;
					std::cout << "<debug> token = " << token << std::endl;
				}
	    }
		}
		else // rest of string is a token
		{
			token = m_string;
			m_string.erase();
			if (bDebug) {
				std::cout << "last token" << std::endl;
				std::cout << "<debug> token = " << token << std::endl;
			}
		}
	}

  // eleminate whitespaces and the beginning and the end
  strip( token, BOTH);
	if (bDebug) {
		std::cout << "after stripping" << std::endl;
		std::cout << "<debug> token = " << token << std::endl;
		std::cout << "done debugging tokeniser..." << std::endl;
	}

  // that's all folks
  return token;
}

/**
 * @brief Check whether the argument is an open bracket character '{', '[' or
 * '('.
 *
 * @param item The character to test
 * @return Whether the item is an open bracket
 */
int isOpenBracket( char &item)
{ 
    if( item == '{' || item =='[' || item == '(' )
    {
	return true;
    }
    else
    {
	return false;
    }
}

/**
 * @brief Returns whether the argument is an closed bracket character '{', '['
 * or '('.
 *
 * @param item The character to test
 * @return Whether the item is a close bracket
 */
int isCloseBracket( char &item)
{
    if( item == '}' || item ==']' || item == ')' )
    {
	return true;
    }
    else
    {
	return false;
    }
}

/**
 * @brief Deletes heading and/or tailing characters matching those provided in
 * a string.
 *
 * @param strng Reference to a string to be striped
 * @param mode Where to string the characters from.  Can be HEAD, TAIL or BOTH.
 * @param delim  - string containing the characters to be deleted
 */
void strip( std::string& strng , int mode, std::string delim)
{
    size_t cp;

    switch( mode )
    {
    case HEAD:	
	// strip leading white space
	cp = strng.find_first_not_of(delim);
	if( cp != -1 )
	{
	    strng.erase(0,cp);
	}
	break;
    case TAIL:
	// strip trailing white space
	cp = strng.find_last_not_of(delim);
	if( (cp != -1) && (cp+1<strng.length()) )
	{
	    strng.erase( cp+1, strng.length() );
	}
	break;
    case BOTH:
	strip( strng, HEAD, delim );
	strip( strng, TAIL, delim );
	break;
    default:
	break;
    }
    return;
}

/**
 * Returns the remaining non-tokenized buffer
 *
 * @return A string containing the remaining internal data
 */
std::string LnTokenizer::getRemains()
{
    // return internal buffer
    return m_string;
}


/**
 * Counts the number of tokens in the internal buffer.
 *
 * @return The number of tokens in the internal buffer
 */
int LnTokenizer::getNumberOfTokens()
{
    LnTokenizer tokenizer( m_string );
    int num=0;
    std::string buffer;

    // while buffer is not empty
    while( (buffer=tokenizer.getNextToken() ) != LNVOID )
    {
	// count token
	num++;
    }
    return num;
}

/**
 * @brief Replaces every token in the internal buffer which matches a string
 * with a different token
 *
 * @param sSrc The string to match against
 * @param sDes The string to replace matches with
 * @return String containing the internal buffer after the replacements
 */
std::string LnTokenizer::Replace(std::string sSrc, std::string sDes){
	std::string sToken, sTemp, sData;
	while ( ((sToken = getNextToken()) != LNVOID) ){
		if ( sToken == sSrc ){
			sData += (sDes + " ");
		}
		else {
			sData += (sToken + " ");
		}
	}
	m_string = sData;
	return sData;		
}
