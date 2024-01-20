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
 * @file LnTokenizer.hpp
 * @brief Header file for the Tokenizer class
 */

#ifndef __LNTOKENIZER_HPP
#define __LNTOKENIZER_HPP

// Global includes
#include<string>

// Local includes 


// Defines
#define LNVOID "LnTokenizer_empty_string"
#define HEAD 0
#define TAIL 1
#define BOTH 2
#define WHITESPACE " \t\n"  // default whitespace characters
#define BRACKETS   "[]{}()" // default bracket characters
#define OPERATORS  "*/+-"   // default operator characters

// Global variables

// Class definitions

/**
 * @brief A class used to break down a given string into tokens, which
 * are separated by whitespaces or operator characters.
 *
 * An internal buffer is processed one token at a time, with the tokenizer only
 * reading as much data as necessary to separate off a new expression.
 * Expressions may be grouped by brackets. Such a group is treated as a single
 * token.
 *
 * Whitespace characters, bracket characters and operator characters are 
 * pre-defined, but can be changed by using the approprated member 
 * functions.
 */
class  LnTokenizer
{
public:
    // constuctors
    LnTokenizer();
    LnTokenizer( std::string buffer);    

    // destructor
    ~LnTokenizer();

    // set whitespace, bracket and operator characters
    void setWhitespace( std::string whitespace );
    void setBrackets( std::string brackets );
    void setOperators( std::string operators );

    // set string to be tokenized
    void setString( std::string buffer);

    // get next token
    std::string getNextToken();

    // get remaining string
    std::string getRemains();

    // get number of tokens in buffer
    int getNumberOfTokens();

    // strip string
    friend void strip( std::string& , int mode=HEAD, std::string=WHITESPACE );

    // check for brackets
    friend int isCloseBracket( char &item);
    friend int isOpenBracket( char &item);
  /** replace sSrc in the string with sDes. */
  std::string Replace(std::string sSrc, std::string sDes);

protected:


private:

    // data members
    std::string m_whitespace; // contains all whitespace characters
    std::string m_brackets;   // contains bracket characters
    std::string m_operators;  // contains operator characters
    std::string m_string;     // contains string to be tokenized
};

void strip( std::string& , int mode, std::string);
int isCloseBracket( char &item);
int isOpenBracket( char &item);
#endif
