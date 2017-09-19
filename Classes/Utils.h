//
//  Utils.h
//  Killit
//
//  Created by a on 2017. 9. 19..
//
//

#ifndef Utils_h
#define Utils_h

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int toInt(const std::string &txt) {
    int ret;
    
    std::istringstream ss(txt);
    ss >> ret;
    
    return ret;
}

#endif /* Utils_h */
