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
#include <initializer_list>

#include "json/rapidjson.h"
#include "json/document.h"
#include "cocos2d.h"
#include "Definitions.h"

KIT_BEGIN

std::vector<std::string> split(const std::string &s, char delim);

int toInt(const std::string &txt);

rapidjson::Document toJson(const std::string &txt);

std::string toString(int n);
std::string toString(float n);

const std::string createData(std::initializer_list<std::string> list);

cocos2d::Vec2 lerp(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to, float alpha);

KIT_END

#endif /* Utils_h */
