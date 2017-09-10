//
//  Player.cpp
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#include "Player.h"

USING_NS_CC;

Player *Player::create() {
    Player *ret = new (std::nothrow) Player();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool Player::init() {
    setScale(2);
    
    player = Sprite::create("res/player.png");
    player->getTexture()->setAliasTexParameters();
    this->addChild(player);
    
    arrow = Sprite::create("res/arrow.png");
    arrow->getTexture()->setAliasTexParameters();
    arrow->setPosition(cos(CC_DEGREES_TO_RADIANS(30)) * 16, sin(CC_DEGREES_TO_RADIANS(30)) * 16);
    arrow->setRotation(90 - 30);
    this->addChild(arrow);
    
    scheduleUpdate();
    
    return true;
}

void Player::update(float dt) {
}

void Player::onStickBegan(Vec2 angle, Ref *pSender) {
    this->angle = angle;
    
    float a = angle.getAngle();
    arrow->setRotation(90 - CC_RADIANS_TO_DEGREES(a));
    arrow->setPosition(cos(a) * 16, sin(a) * 16);
    touchJoystick = true;
}

void Player::onStickMoved(Vec2 angle, Ref *pSender) {
    this->angle = angle;
    
    float a = angle.getAngle();
    arrow->setRotation(90 - CC_RADIANS_TO_DEGREES(a));
    arrow->setPosition(cos(a) * 16, sin(a) * 16);
}

void Player::onStickEnded(Vec2 angle, Ref *pSender) {
    touchJoystick = false;
}
