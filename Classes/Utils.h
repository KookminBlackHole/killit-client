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

const std::string createData(const std::string &tag, ...) {
    va_list args;
    va_start(args, tag);
    
    std::string ret = "[{";
    
    ret += "\"" + tag + "\"";
    ret += ":";
    
    auto item = va_arg(args, char *);
    std::string str = item;
    int i = 0;
    while (str != "") {
        if (i % 2 == 0) { // 데이터 값인 경우
            ret += str + ",";
        } else { // 태그인 경우
            ret += "\"" + str + "\"" + ":";
        }
        item = va_arg(args, char *);
        str = item;
        i++;
    }
    
    ret = ret.substr(0, ret.length() - 1); // 마지막 , 지워줌
    ret += "}]";
    
    va_end(args);
    
    return ret;
}

cocos2d::Vec2 lerp(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to, float alpha) {
    return cocos2d::Vec2(cocos2d::MathUtil::lerp(from.x, to.x, alpha), cocos2d::MathUtil::lerp(from.y, to.y, alpha));
}

#endif /* Utils_h */
