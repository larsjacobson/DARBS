#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/algorithm/string/trim.hpp>
#include <math.h>
#include <stdlib.h>
#include <time.h>

extern "C"
std::string getInput(std::string question) {
    boost::algorithm::trim(question);
    std::cout << question << std::endl;
    std::string in;
    getline(std::cin, in);
    return(in); 
}

extern "C"
std::string setVariable(std::string value) {
    return(value);
}

extern "C"
std::string addInteger(std::string arg) {
    std::stringstream in, out;
    int value, add;
    // Chuck the argument to a stringstream
    in << arg;
    // Grab out the first and second values into integers
    in >> value >> add;
    // Add the integer
    value += add;
    // Chuck the new value into a new string and return it
    out << value;
    return(out.str());
}

extern "C"
std::string calculateInteger(std::string arg) {
    std::stringstream in, out;
    int valueA, valueB, returnValue;
    std::string op;
    // Chuck the argument to a stringstream
    in << arg;
    // Grab the two values, and the operator
    in >> valueA >> op >> valueB;
    if (op == "+") {
        returnValue = valueA + valueB;
    }
    else if (op == "-") {
        returnValue = valueA - valueB;
    }
    else if (op == "*") {
        returnValue = valueA * valueB;
    }
    else if (op == "/") {
        returnValue = valueA / valueB;
    }
    else if (op == "%") {
        returnValue = valueA % valueB;
    }
    out << returnValue;
    return out.str();
}

extern "C"
std::string choose(std::string arg) {
    std::stringstream in;
    std::string value;
    std::vector<std::string> values;
    // Slow doing this each time, but we need proper randomization!
    srand(time(NULL));
    in << arg;
    while (in >> value) {
        values.push_back(value);
    }
    int index = floor(rand() * values.size() / RAND_MAX);
    return values[index];
}
