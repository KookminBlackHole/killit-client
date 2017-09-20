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

USING_NS_CC;

Player *Player::create(int sx, int sy) {
    Player *ret = new (std::nothrow) Player();
    if (ret && ret->init())
    {
        ret->autorelease();
		ret->gX = sx;
		ret->gY = sy;
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

	/// ≪√∑π¿AæO zorder º≥¡§
	this->setGlobalZOrder(ZORDER::PLAYER);
	for (auto &i : this->getChildren()) {
		i->setGlobalZOrder(ZORDER::PLAYER);
	}
    
    /// Rect(-16, -16 - 24 + 16)
	solidBB = Rect(-16, -24, 32, 32);
    
    scheduleUpdate();
    
    return true;
}

void Player::update(float dt) {
	float pAngle = CC_RADIANS_TO_DEGREES(angle.getAngle());
	if (pAngle > 90 || pAngle <= -90) { /// ø?￢?
		player->setFlippedX(true);
	} else {
		player->setFlippedX(false);
	}
}

void Player::onStickBegan(Vec2 angle, Ref *pSender) {
    this->angle = angle;
    touchJoystick = true;
}

void Player::onStickMoved(Vec2 angle, Ref *pSender) {
    this->angle = angle;
}

void Player::onStickEnded(Vec2 angle, Ref *pSender) {
    touchJoystick = false;
}

void Player::calculateGridCoord(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	gX = (this->getPositionX() + (24 * (mapWidth - 1) - origin.x)) / 48 + 1;
	gY = (this->getPositionY() + (24 * (mapHeight - 1) - origin.y)) / 48 + 1;
}

void Player::gridCoordUpdate(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    tempPosition = Vec2(48 * gX - (24 * mapWidth - origin.x), 48 * gY - (24 * mapHeight - origin.y));
	this->setPosition(tempPosition);
}

void Player::move() {
	/// ≪√∑π¿AæO∞° ¡∂¿AΩ∫?Ω ?O?°≪?¿ª ∂ß ≪y￥A πE≪?¿∏∑Œ ¿Aμø
	if (touchJoystick) {
		tempPosition = getPosition() + angle * speed;
	}
}

void Player::collision() {
	HelloWorld *parent = (HelloWorld *)getParent();
    
    /// ≪?¿A ¿ß?°∏¶ ±?¡y¿∏∑Œ 3x3?≠¿ª √Eμπ ∞AªA∏¶ ≪‘
    /// 충돌 처리 시 벽에 끼이는 문제를 해결하기 위해 충돌 검사 순서를 가운데 - 위 - 아래 순으로 바꿈.
    int yOrder[3] = { 0, 1, -1 }, xOrder[3] = { 0, 1, -1 };
    for (int k = 0; k < 3; k++) {
        int i = clampf(gY + yOrder[k], 0, parent->mapHeight - 1);
        for (int l = 0; l < 3; l++) {
            int j = clampf(gX + xOrder[l], 0, parent->mapWidth - 1);
            if (i == gY && j == gX) continue;
            /// ∫Æ¿A≥™ πÆ¿A∏E
            if (parent->checkSolidObject(j, i)) {
                /// ≪√∑π¿AæOøO ªU￥I √Eμπ√º¿≪ BoundingBox∏¶ ∞°¡Æø≫
                auto playerBB = Rect(tempPosition + solidBB.origin, solidBB.size);
                auto otherBB = Rect(parent->mapTile[i][j]->getPosition() - Size(24, 24), Size(48, 48));
				/// √Eμπ ∞AªA
				if (playerBB.intersectsRect(otherBB)) {
					parent->mapTile[i][j]->setColor(Color3B::MAGENTA);

					/// 0: left, 1: right, 2: up, 3: down
					int dir = 0;

					float angle = CC_RADIANS_TO_DEGREES((Vec2(playerBB.getMidX(), playerBB.getMidY()) - Vec2(otherBB.getMidX(), otherBB.getMidY())).getAngle());

					if (angle > -45 && angle <= 45) dir = 1;
					else if (angle > 45 && angle <= 135) dir = 2;
					else if ((angle > 135 && angle <= 180) || (angle < -135 && angle >= -180)) dir = 0;
					else dir = 3;
                    
                    if (dir == 0)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("left");
                    if (dir == 1)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("right");
                    if (dir == 2)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("up");
                    if (dir == 3)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("down");

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
	CameraUtil::getInstance()->setPosition(this->getPosition());
}

void Player::checkGameObjects() {
	HelloWorld *parent = (HelloWorld *)getParent();
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	Vec2 check = this->getPosition() + angle * 48;

    int xx = (check.x + (24 * (parent->mapWidth - 1) - origin.x)) / 48 + 1;
	int yy = (check.y + (24 * (parent->mapHeight - 1) - origin.y)) / 48 + 1;

//	parent->mapTile[yy][xx]->setColor(Color3B::BLUE);

	//state = 0;
	switch (parent->mapData[yy][xx]) {
	case 1:
		parent->mapTile[yy][xx]->setColor(Color3B::GREEN);
		//state = 1;
		break;
	case 2:
		parent->mapTile[yy][xx]->setColor(Color3B::YELLOW);
		//state = 2;
		break;
	}
}

