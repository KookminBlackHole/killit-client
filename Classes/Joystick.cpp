//
//  Joystick.cpp
//  Battle
//
//  Created by a on 2017. 9. 4..
//
//

#include "Joystick.h"
#include "Player.h"

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
    this->setOpacity(255 * 0.5f);
    this->setScale(2);
    
    pad = Sprite::create("res/joystickPad.png");
    pad->getTexture()->setAliasTexParameters();
    this->addChild(pad);
    
    stick = Sprite::create("res/joystick.png");
    stick->getTexture()->setAliasTexParameters();
    this->addChild(stick);
    
    listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(Joystick::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(Joystick::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(Joystick::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
    
    return true;
}

void Joystick::onTouchesBegan(const vector<Touch *> &touches, Event *e) {
    for (auto &t : touches) {
        auto bb = pad->getBoundingBox();
        if (Rect(bb.origin.x, bb.origin.y, bb.size.width + 20, bb.size.height + 20).containsPoint(this->convertToNodeSpace(t->getLocation()))) {
            if (touchId == -1) {
                touchId = t->getID();
            
                pos = this->convertToNodeSpace(t->getLocation());
                int radius = pad->getContentSize().width / 2;
                if (pos.getLengthSq() > radius * radius) pos = pos.getNormalized() * radius;
                stick->setPosition(pos);
                bnd->onStickBegan(pos.getNormalized(), this);
            }
        }
    }
}

void Joystick::onTouchesMoved(const vector<Touch *> &touches, Event *e) {
    for (auto &t : touches) {
        if (touchId == t->getID()) {
            pos = this->convertToNodeSpace(t->getLocation());
            int radius = pad->getContentSize().width / 2;
            if (pos.getLengthSq() > radius * radius) pos = pos.getNormalized() * radius;
            stick->setPosition(pos);
            bnd->onStickMoved(pos.getNormalized(), this);
        }
    }
}

void Joystick::onTouchesEnded(const vector<Touch *> &touches, Event *e) {
    touchId = -1;
    stick->setPosition(Vec2::ZERO);
    bnd->onStickEnded(pos.getNormalized(), this);
}

void Joystick::bind(Player *player) {
    bnd = player;
}

void Joystick::update(float dt) {
}
