#include <string>
#include <sstream>

extern "C"
std::string square(std::string arg) {
    std::istringstream in(arg);
    double num;
    in >> num;
    double sq = num * num;
    std::stringstream out;
    out << sq;
    return(out.str());
}

    
