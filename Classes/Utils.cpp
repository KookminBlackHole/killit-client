//
//  Utils.cpp
//  Killit
//
//  Created by a on 2017. 11. 15..
//

#include "Utils.h"

using namespace std;

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    
    return elems;
}

int toInt(const string &txt) {
    int ret;
    
    istringstream ss(txt);
    ss >> ret;
    
    return ret;
}

rapidjson::Document toJson(const std::string &txt) {
    rapidjson::Document ret;
    ret.Parse(txt.c_str());
    return ret;
}

string toString(int n) {
    std::ostringstream ss;
    ss << n ;
    return ss.str();
}

string toString(float n) {
    std::ostringstream ss;
    ss << n ;
    return ss.str();
}

const string createData(initializer_list<string> list) {
    int idx = 0;
    
    string ret = "{";
    
    for (auto i : list) {
        if (idx % 2 == 0) {
            ret += "\"" + i + "\":";
        } else {
            ret += i + ",";
        }
        idx++;
    }
    
    ret = ret.substr(0, ret.length() - 1); // 마지막 , 지워줌
    ret += "}";
    
    return ret;
}

cocos2d::Vec2 lerp(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to, float alpha) {
    return cocos2d::Vec2(cocos2d::MathUtil::lerp(from.x, to.x, alpha), cocos2d::MathUtil::lerp(from.y, to.y, alpha));
}
