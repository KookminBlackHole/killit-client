//
//  Joystick.cpp
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#include "Joystick.h"
#include "Player.h"
#include "CameraUtil.h"
#include "ZOrder.h"

USING_NS_CC;
using namespace std;

Joystick *Joystick::create(const Vec2 &position) {
    Joystick *ret = new (std::nothrow) Joystick();
    if (ret && ret->init())
    {
        ret->setPosition(position);
        
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

Joystick *Joystick::create(float x, float y) {
    return Joystick::create(Vec2(x, y));
}

bool Joystick::init() {
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(255 * 0.25f);
    this->setScale(2);
    
    pad = Sprite::create("res/joystickPad.png");
    pad->getTexture()->setAliasTexParameters();
    this->addChild(pad);
    
    stick = Sprite::create("res/interaction.png");
    stick->getTexture()->setAliasTexParameters();
    this->addChild(stick);
    
    this->setGlobalZOrder(ZORDER::UI);
    for (auto &i : this->getChildren()) {
        i->setGlobalZOrder(ZORDER::UI);
    }
    
//    scheduleUpdate();
    
    return true;
}

void Joystick::onTouchBegan(const cocos2d::Vec2 &position, int id) {
    this->id = id;
    
    pos = position;
    
    int radius = pad->getContentSize().width / 2;
    if (pos.getLengthSq() > radius * radius) pos = pos.getNormalized() * radius;
   
    stick->setPosition(pos);
    bnd->onStickBegan(pos.getNormalized(), this);
}

void Joystick::onTouchMoved(const cocos2d::Vec2 &position, int id) {
    pos = position;
    
    int radius = pad->getContentSize().width / 2;
    if (pos.getLengthSq() > radius * radius) pos = pos.getNormalized() * radius;
    
    stick->setPosition(pos);
    bnd->onStickMoved(pos.getNormalized(), this);
}

void Joystick::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
    stick->setPosition(Vec2::ZERO);
    bnd->onStickEnded(pos.getNormalized(), this);
}

void Joystick::bind(Player *player) {
    bnd = player;
}
