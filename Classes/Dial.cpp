//
//  Dial.cpp
//  Killit
//
//  Created by a on 2017. 10. 27..
//

#include "Dial.h"

#include "Player.h"
#include "CameraUtil.h"
#include "ZOrder.h"

USING_NS_CC;
using namespace std;

Dial *Dial::create() {
    Dial *ret = new (std::nothrow) Dial();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool Dial::init() {
    
    return true;
}

void Dial::onTouchBegan(const cocos2d::Vec2 &position, int id) {
    this->id = id;
}

void Dial::onTouchMoved(const cocos2d::Vec2 &position, int id) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    
    auto angle = CC_RADIANS_TO_DEGREES((origin - position).getAngle());
    
    bnd->angle = angle;
}

void Dial::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
}

void Dial::bind(Player *player) {
    bnd = player;
}
