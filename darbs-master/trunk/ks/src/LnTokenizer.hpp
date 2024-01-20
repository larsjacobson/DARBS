//////////////////////////////////////////////////////////////////////////
//
// File: LnTokenizer.hpp
//
//////////////////////////////////////////////////////////////////////////
//
// $Id: LnTokenizer.hpp,v 1.1 2001/09/06 14:22:28 kcpw2 Exp $
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis: 
//
// Definition file for LnTokenizer class
// 
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2000/05/10: Lars Nolle: initial implementation
//
//////////////////////////////////////////////////////////////////////////
#ifndef __LNTOKENIZER_HPP
#define __LNTOKENIZER_HPP

///////////////////////////// global includes ////////////////////////////
#include<string>

///////////////////////////// local includes /////////////////////////////


///////////////////////////// defines ////////////////////////////////////
#define LNVOID "LnTokenizer_empty_string"
#define HEAD 0
#define TAIL 1
#define BOTH 2
#define WHITESPACE " \t\n"  // default whitespace characters
#define BRACKETS   "[]{}()" // default bracket characters
#define OPERATORS  "*/+-"   // default operator characters

///////////////////////////// global variables ///////////////////////////

///////////////////////////// class definitions //////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// LnTokenizer
//
//////////////////////////////////////////////////////////////////////////
//
// Synopsis:  
//
// LnTokenizer can be used to break down a given string into tokens, which
// are separated by whitespaces or operator characters. Expressions may be
// grouped by brackets. Such a group is treated as a single token. 
//
// Whitespace characters, bracket characters and operator characters are 
// pre-defined, but can be changed by using the approprated member 
// functions.
//
//////////////////////////////////////////////////////////////////////////
//
// History:
//
// 2000/06/21: Lars Nolle: initial implementation
//
//////////////////////////////////////////////////////////////////////////
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

void strip(std::string& , int mode, std::string);
int isCloseBracket( char &item);
int isOpenBracket( char &item);

#endif 
