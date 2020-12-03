#include "utils.h"
#include <sstream>

std::vector<std::string> split(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string elem;
    std::vector<std::string> elems;
    while (std::getline(ss, elem, delim)) {
        elems.push_back(elem);
    }
    return elems;
};
