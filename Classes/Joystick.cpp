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
    
    prevDirection = direction;
    
    float angle = CC_RADIANS_TO_DEGREES(position.getAngle());
    angle = floor(angle * way / 360.0f + 0.5f) * distance;
    
    int radius = pad->getContentSize().width / 2 - 20;
    direction = Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
   
    stick->setPosition(direction * radius);
    
    player->onStickBegan(direction, this);
    if (direction != prevDirection) player->onStickChanged(direction, this);
}

void Joystick::onTouchMoved(const cocos2d::Vec2 &position, int id) {
    prevDirection = direction;
    
    float angle = CC_RADIANS_TO_DEGREES(position.getAngle());
    angle = floor(angle * way / 360.0f + 0.5f) * distance;
    
    int radius = pad->getContentSize().width / 2 - 20;
    direction = Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
    
    stick->setPosition(direction * radius);
    
    player->onStickMoved(direction, this);
    if (direction != prevDirection) player->onStickChanged(direction, this);
}

void Joystick::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
    direction = Vec2::ZERO;
    stick->setPosition(Vec2::ZERO);
    player->onStickEnded(direction, this);
    player->onStickChanged(direction, this);
}

void Joystick::bind(Player *player) {
    this->player = player;
}
