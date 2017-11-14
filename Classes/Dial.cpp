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
#include "Definitions.h"
#include "HelloWorldScene.h"
#include "Raycast.h"

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
    
    HelloWorld *parent = (HelloWorld *)bnd->getParent();
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = size / 2;
    
    prevAngle = CC_RADIANS_TO_DEGREES((position - origin).getAngle());
    
    // 1. 터치한 좌표에 문이 있는지 확인
    // 2. 문이 있으면 레이캐스트로 벽 뒤에서 문을 열진 않았는지 확인
    int fx = (bnd->getPositionX() + position.x - size.width + TILE_SIZE_HALF * (parent->mapWidth - 1)) / TILE_SIZE + 1;
    int fy = (bnd->getPositionY() + position.y - size.height + TILE_SIZE_HALF * (parent->mapHeight - 1)) / TILE_SIZE + 1;
    
    if (fx < 0 || fx > parent->mapWidth -1 || fy < 0 || fy > parent->mapHeight - 1) return;
    
    if (parent->mapTile[fy][fx]->type > 0 && parent->mapTile[fy][fx]->type < 11) {
        GameObject *contactObject;
        Vec2 trash;
        if (raycast(parent->mapTile, bnd->getPosition(), prevAngle, 150, trash, contactObject)) {
            if (contactObject->type == 1) { // 문 오브젝트를 클릭했을 때
                contactObject->setSolidObject(!contactObject->isSolidObject());
            }
            
        }
    }
}

void Dial::onTouchMoved(const cocos2d::Vec2 &position, int id) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    
    auto angle = CC_RADIANS_TO_DEGREES((position - origin).getAngle());
    bnd->angle += clampf(angle - prevAngle, -10.0f, 10.0f) * sensitivity;
    
    
    prevAngle = angle;
}

void Dial::onTouchEnded(const cocos2d::Vec2 &position, int id) {
    this->id = -1;
}

void Dial::bind(Player *player) {
    bnd = player;
}
