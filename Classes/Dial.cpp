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
    
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    
    prevAngle = CC_RADIANS_TO_DEGREES((position - origin).getAngle());
}

void Dial::onTouchMoved(const cocos2d::Vec2 &position, int id) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    
    auto angle = CC_RADIANS_TO_DEGREES((position - origin).getAngle());
    bnd->angle += clampf(angle - prevAngle, -10.0f, 10.0f) * 2.5f;
    
    CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("now: " + to_string(bnd->angle));
    
    prevAngle = angle;
}

void Dial::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
}

void Dial::bind(Player *player) {
    bnd = player;
}
