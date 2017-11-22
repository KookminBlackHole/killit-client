//
//  Player.cpp
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#include "Player.h"

#include "HelloWorldScene.h"
#include "ZOrder.h"
#include "CameraUtil.h"
#include "Definitions.h"
#include "Raycast.h"
#include "Utils.h"

USING_NS_CC;
using namespace std;

Player *Player::create(int sx, int sy, bool owner) {
    Player *ret = new (std::nothrow) Player();
    if (ret && ret->init())
    {
        ret->autorelease();
		ret->gX = sx;
		ret->gY = sy;
        ret->owner = owner;
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
    
	solidBB = Rect(-PLAYER_WIDTH, -36 * 2 / 2, PLAYER_WIDTH * DEFAULT_SCALE, PLAYER_HEIGHT * DEFAULT_SCALE);
    
    player = Sprite::create("res/player.png");
    player->getTexture()->setAliasTexParameters();
	player->setTextureRect(Rect(0 * 32, 0 * 36, 32, 36));
    this->addChild(player);
    
    debugHP = DrawNode::create();
//    debugHP->setPositionY(solidBB.getMaxY() + 4 + 4);
    this->addChild(debugHP);
    
    debugAttack = DrawNode::create();
    this->addChild(debugAttack);
    
    debugAngle = DrawNode::create();
    this->addChild(debugAngle);

	/// ≪√∑π¿AæO zorder º≥¡§
	this->setGlobalZOrder(ZORDER::PLAYER);
	for (auto &i : this->getChildren()) {
		i->setGlobalZOrder(ZORDER::PLAYER);
	}
    scheduleUpdate();
    
    return true;
}

void Player::update(float dt) {
    //prevIdx = idx;
    
	//if (angle < 0 || angle <= -180) angle += 360;
	//if (angle >= 360) angle -= 360;

	//idx = int(floor(angle / 45 + 0.5f)) % 8;
    //if (idx != prevIdx) onAngleChanged(idx);
    
    debugAngle->setRotation(-angle);
    
    if (owner) {
        debugHP->clear();
        debugHP->drawTriangle(Vec2(-3, 0 + PLAYER_HEIGHT + 5), Vec2(0, -5 + PLAYER_HEIGHT + 5), Vec2(3, 0 + PLAYER_HEIGHT + 5), Color4F::RED);
        
        debugAngle->clear();
        debugAngle->drawLine(Vec2(24, 0), Vec2(48, 0), Color4F::RED);
    }
}

void Player::onStickBegan(const Vec2 &direction, Ref *pSender) {
    this->direction = direction;
    touchJoystick = true;
}

void Player::onStickMoved(const Vec2 &direction, Ref *pSender) {
    this->direction = direction;
}

void Player::onStickEnded(const Vec2 &direction, Ref *pSender) {
    touchJoystick = false;
}

void Player::onStickChanged(const Vec2 &direction, Ref *pSender) {
    HelloWorld *parent = (HelloWorld *)getParent();

	float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
	if (angle < 0 || angle <= -180) angle += 360;
	if (angle >= 360) angle -= 360;

	if (direction != Vec2::ZERO) {
		idx = (int(floor(angle / 45 + 0.5f)) % 8);
		player->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
	}

    auto send = createData({ "uuid", "\"" + parent->uuid + "\"", "x", toString(getPositionX()).c_str(), "y", toString(getPositionY()).c_str(), "dirX", toString(direction.x).c_str(), "dirY", toString(direction.y).c_str(), "speed", toString(speed).c_str() });
    parent->emit("game:update-player-direction", send);
}

void Player::onAngleChanged(int idx) {
    HelloWorld *parent = (HelloWorld *)getParent();
    
    player->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
    auto send = createData({ "uuid", "\"" + parent->uuid + "\"", "angle", toString(idx) });
    parent->emit("game:update-player-angle", send);
}

void Player::calculateGridCoord(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	gX = ((this->getPositionX()) + (TILE_SIZE_HALF * (mapWidth - 1) - origin.x)) / TILE_SIZE + 1;
	gY = ((this->getPositionY()) + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;
}

void Player::gridCoordUpdate(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    tempPosition = Vec2(TILE_SIZE * gX - (TILE_SIZE_HALF * mapWidth - origin.x), TILE_SIZE * gY - (TILE_SIZE_HALF * mapHeight - origin.y));
	this->setPosition(tempPosition);
}

void Player::move(float dt) {
	/// ≪√∑π¿AæO∞° ¡∂¿AΩ∫?Ω ?O?°≪?¿ª ∂ß ≪y￥A πE≪?¿∏∑Œ ¿Aμø
	if (touchJoystick) {
		tempPosition = getPosition() + direction * speed * (dt * 60);
	}
}

// 충돌 범위가 정사각형이 아닌 경우 제대로 동작하지 않음
void Player::collision() {
	HelloWorld *parent = (HelloWorld *)getParent();
    
    /// 충돌 처리 시 벽에 끼이는 문제를 해결하기 위해 충돌 검사 순서를 가운데 - 위 - 아래 순으로 바꿈.
    int yOrder[3] = { 0, 1, -1 }, xOrder[3] = { 0, 1, -1 };
    for (int k = 0; k < 3; k++) {
        int i = clampf(gY + yOrder[k], 0, parent->mapHeight - 1);
        for (int l = 0; l < 3; l++) {
            int j = clampf(gX + xOrder[l], 0, parent->mapWidth - 1);
            //if (i == gY && j == gX) continue;
            if (parent->isSolidObject(j, i)) {
                /// ≪√∑π¿AæOøO ªU￥I √Eμπ√º¿≪ BoundingBox∏¶ ∞°¡Æø≫
                auto playerBB = Rect(tempPosition + solidBB.origin, solidBB.size);
				auto otherBB = parent->mapTile[i][j]->getBoundingBox();//Rect(parent->mapTile[i][j]->getPosition() - Size(TILE_SIZE_HALF, TILE_SIZE_HALF), Size(TILE_SIZE, TILE_SIZE));
				/// √Eμπ ∞AªA
				if (playerBB.intersectsRect(otherBB)) {
//                    parent->mapTile[i][j]->setColor(Color3B::MAGENTA);

					/// 0: left, 1: right, 2: up, 3: down
					int dir = 0;

					float angle = CC_RADIANS_TO_DEGREES((Vec2(playerBB.getMidX(), playerBB.getMidY()) - Vec2(otherBB.getMidX(), otherBB.getMidY())).getAngle());

					if (angle > -45 && angle <= 45) dir = 1;
					else if (angle > 45 && angle <= 135) dir = 2;
					else if ((angle > 135 && angle <= 180) || (angle < -135 && angle >= -180)) dir = 0;
					else dir = 3;

					if (dir == 0) {
						tempPosition.x = otherBB.getMinX() - solidBB.getMaxX();
					} else if (dir == 1) {
						tempPosition.x = otherBB.getMaxX() - solidBB.getMinX();
					} else if (dir == 2) {
						tempPosition.y = otherBB.getMaxY() - solidBB.getMinY();
					} else {
                        tempPosition.y = otherBB.getMinY() - solidBB.getMaxY();
					}
				}
			}

		}
	}
}

void Player::updateZOrder() {
	HelloWorld *parent = (HelloWorld *)getParent();

	/// ≪√∑π¿AæO z-order ∫Ø∞E
    int zorder = parent->mapTile[int(clampf(gY, 0, parent->mapHeight - 1))][int(clampf(gX, 0, parent->mapWidth - 1))]->getGlobalZOrder() + 1000;
    this->setGlobalZOrder(zorder);
    for (auto &i : this->getChildren()) {
        i->setGlobalZOrder(zorder);
    }
}

void Player::updatePosition() {
	/// ∞EªIμ≫ ≪√∑π¿AæO ¿ß?° Ω≪¡¶ ¿˚øI
	this->setPosition(tempPosition);
    
	CameraUtil::getInstance()->setPosition((this->getPositionX()), (this->getPositionY()));
}

bool Player::checkGameObjects() {
	HelloWorld *parent = (HelloWorld *)getParent();
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	Vec2 check = this->getPosition() + direction * TILE_SIZE;

    int xx = (check.x + (TILE_SIZE_HALF * (parent->mapWidth - 1) - origin.x)) / TILE_SIZE + 1;
	int yy = (check.y + (TILE_SIZE_HALF * (parent->mapHeight - 1) - origin.y)) / TILE_SIZE + 1;

	switch (parent->mapData[yy][xx]) {
        case 1:
            parent->mapData[yy][xx] = 3;
            break;
        case 3:
            parent->mapData[yy][xx] = 1;
            break;
        default:
            return false;
	}
    
    return true;
}

void Player::checkSolidObjects() {
    HelloWorld *parent = (HelloWorld *)getParent();
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    
    if (gY > 0 && parent->isSolidObject(gX, gY - 1)) {
        for (int i = MAX(gX - 1, 0); i < MIN(gX + 2, parent->mapWidth - 1); i++) {
            if (parent->isSolidObject(i, gY - 1)) {
                parent->mapTile[gY - 1][i]->setOpacity(255 * 0.5f);
            }
        }
    }
}

void Player::attack() {
    HelloWorld *parent = (HelloWorld *)getParent();
    Vec2 dot;
    this->raycast(parent->mapTile, angle, 500, dot);
    
    parent->getChildByName<DrawNode*>("debug2")->clear();
    parent->getChildByName<DrawNode*>("debug2")->drawLine(getPosition(), dot, Color4F::BLACK);
    parent->getChildByName<DrawNode*>("debug2")->drawDot(dot, 4, Color4F::RED);
}

bool Player::raycast(GameObject ***objects, float angle, float length, Vec2 &out) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    auto ray = getPosition() - Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
    bool escape = false;
    for (int i = 0; i < length && !escape; i += 4) {
        ray += Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle)) * 4;
        
        // 현재 문의 상태를 알 수 없어서 문에 무조건 충돌함.
        int lim = floor(i / TILE_SIZE) + 1;
        for (int k = max(gY - lim, 0); k < min(gY + lim + 1, 64) && !escape; k++) {
            for (int l = max(gX - lim, 0); l < min(gX + lim + 1, 64) && !escape; l++) {
                if (objects[k][l]->isSolidObject()) {
                    if (objects[k][l]->getBoundingBox().containsPoint(ray)) escape = true;
                }
            }
        }
    }
    
    out = ray;
    
    return escape;
}
