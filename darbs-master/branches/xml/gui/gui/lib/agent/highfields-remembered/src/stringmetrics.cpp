#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <boost/algorithm/string.hpp>
#include <math.h>

// Taken from simmetrics - http://simmetrics.cvs.sourceforge.net
extern "C"
std::string cosinedistance(std::string arg) {
    std::vector<std::string> strs;
    boost::split(strs, arg, boost::is_any_of("|"));

    std::vector<std::string> str1Tokens;
    boost::split(str1Tokens, strs[0], boost::is_any_of(" \n\t,.;!?\"'()"));
    std::vector<std::string> str2Tokens;
    boost::split(str2Tokens, strs[1], boost::is_any_of(" \n\t,.;!?\"'()"));

    std::set<std::string> allTokens;
    for (int i=0; i<str1Tokens.size(); i++) {
        allTokens.insert(str1Tokens[i]);
    }
    int termsInString1 = allTokens.size();

    std::set<std::string> secondStringTokens;
    for (int i=0; i<str2Tokens.size(); i++) {
        secondStringTokens.insert(str2Tokens[i]);
    }
    int termsInString2 = secondStringTokens.size();

    std::set<std::string>::iterator it = secondStringTokens.begin();
    for (it; it!=secondStringTokens.end(); it++) {
        allTokens.insert(*it);
    }

    int commonTerms = termsInString1 + termsInString2 - allTokens.size();
    
    double retVal = (double) commonTerms;
    retVal /= pow(termsInString1, 0.5) * pow(termsInString2, 0.5);
    std::stringstream out;
    out << retVal;
    return(out.str());
}

extern "C"
std::string commonterms(std::string arg) {
    std::vector<std::string> strs;
    boost::split(strs, arg, boost::is_any_of("|"));

    std::vector<std::string> str1Tokens;
    boost::split(str1Tokens, strs[0], boost::is_any_of(" \n\t,.;!?\"'()"));
    std::vector<std::string> str2Tokens;
    boost::split(str2Tokens, strs[1], boost::is_any_of(" \n\t,.;!?\"'()"));

    std::set<std::string> allTokens;
    for (int i=0; i<str1Tokens.size(); i++) {
        allTokens.insert(str1Tokens[i]);
    }
    int termsInString1 = allTokens.size();

    std::set<std::string> secondStringTokens;
    for (int i=0; i<str2Tokens.size(); i++) {
        secondStringTokens.insert(str2Tokens[i]);
    }
    int termsInString2 = secondStringTokens.size();

    std::set<std::string>::iterator it = secondStringTokens.begin();
    for (it; it!=secondStringTokens.end(); it++) {
        allTokens.insert(*it);
    }

    int commonTerms = termsInString1 + termsInString2 - allTokens.size();

    std::stringstream out;
    out << commonTerms;
    return(out.str());
}
