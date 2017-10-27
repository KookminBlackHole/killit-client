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
}

void Dial::onTouchMoved(const cocos2d::Vec2 &position, int id) {
}

void Dial::onTouchEnded(const cocos2d::Vec2 &position, int id) {
}

void Dial::bind(Player *player) {
    bnd = player;
}
