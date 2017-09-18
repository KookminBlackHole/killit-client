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

CameraUtil *CameraUtil::getInstance() {
    return instance;
}

CameraUtil::~CameraUtil() {
    this->unscheduleUpdate();
}

void CameraUtil::initialize(Scene *now) {
    this->autorelease();
    
    nowScene = now;
    
    fixedLayer = Layer::create();
    now->addChild(fixedLayer);
    
    now->addChild(this);
}

void CameraUtil::addUIChild(Node *child) {
    fixedLayer->addChild(child);
}

void CameraUtil::setPosition(float x, float y) {
    Camera::getDefaultCamera()->setPosition(Vec2(x, y));
    fixedLayer->setPosition(Camera::getDefaultCamera()->getPosition() - fixedLayer->getContentSize() / 2);
    fixedLayer->setRotation(Camera::getDefaultCamera()->getRotation());
}

void CameraUtil::setPosition(const Vec2 &pos) {
    Camera::getDefaultCamera()->setPosition(pos);
    fixedLayer->setPosition(Camera::getDefaultCamera()->getPosition() - fixedLayer->getContentSize() / 2);
    fixedLayer->setRotation(Camera::getDefaultCamera()->getRotation());
}

const Vec2 &CameraUtil::getPosition() const {
    return fixedLayer->getPosition();
}
