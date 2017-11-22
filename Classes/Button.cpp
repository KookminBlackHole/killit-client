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

KIT_BEGIN

void Button::onTouchBegan(const cocos2d::Vec2 &position, int id) {
    this->id = id;
    onPressed();
}

void Button::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
    onReleased();
}

KIT_END
