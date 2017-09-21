//
//  Button.cpp
//  Killit
//
//  Created by a on 2017. 9. 13..
//
//

#include "Button.h"

USING_NS_CC;
using namespace std;

Button *Button::create(const string &filename) {
    Button *ret = new (std::nothrow) Button();
    if (ret && ret->initWithFile(filename))
    {
        ret->autorelease();
		ret->initialize();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Button::initialize() {
    
}

void Button::onTouchBegan(const cocos2d::Vec2 &position, int id) {
    this->id = id;
    MessageBox("", "");
}

void Button::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
}
