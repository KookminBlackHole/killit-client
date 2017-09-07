//
//  CameraUtil.cpp
//  Battle
//
//  Created by a on 2017. 9. 7..
//
//

#include "CameraUtil.h"

USING_NS_CC;

CameraUtil *CameraUtil::instance = new CameraUtil();

CameraUtil *CameraUtil::getInstance()
{
    return instance;
}

CameraUtil::~CameraUtil() {
    this->unscheduleUpdate();
}

void CameraUtil::initialize(Scene *now)
{
    this->autorelease();
    
    nowScene = now;
    
    fixedLayer = LayerColor::create(Color4B::RED);
	//fixedLayer->setGlobalZOrder(99999999999);
	fixedLayer->setOpacity(0);
    now->addChild(fixedLayer);
    
    this->scheduleUpdate();
    
    now->addChild(this);
}

void CameraUtil::addUIChild(Node *child)
{
    fixedLayer->addChild(child);
}

void CameraUtil::update(float dt)
{
    Camera::getDefaultCamera()->setPosition(pos);
    fixedLayer->setPosition(Camera::getDefaultCamera()->getPosition() - fixedLayer->getContentSize() / 2);
    fixedLayer->setRotation(Camera::getDefaultCamera()->getRotation());
}

void CameraUtil::setPosition(float x, float y)
{
    pos.x = x;
    pos.y = y;
}

void CameraUtil::setPosition(const Vec2 &pos)
{
    this->pos.x = pos.x;
    this->pos.y = pos.y;
}

void CameraUtil::setRelPosition(float x, float y)
{
    pos.x += x;
    pos.y += y;
}

void CameraUtil::setRelPosition(const Vec2 &pos)
{
    this->pos.x += pos.x;
    this->pos.y += pos.y;
}

const Vec2 &CameraUtil::getPosition() const
{
    return pos;
}
