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

	/// 플레이어 zorder 설정
	this->setGlobalZOrder(ZORDER::PLAYER);
	for (auto &i : this->getChildren()) {
		i->setGlobalZOrder(ZORDER::PLAYER);
	}
    
    scheduleUpdate();
    
    return true;
}

void Player::update(float dt) {
	float pAngle = CC_RADIANS_TO_DEGREES(angle.getAngle());
	if (pAngle > 90 || pAngle <= -90) { /// 왼쪽
		player->setFlippedX(true);
	} else {
		player->setFlippedX(false);
	}
}

void Player::onStickBegan(Vec2 angle, Ref *pSender) {
    this->angle = angle;
    
    float a = angle.getAngle();
    touchJoystick = true;
}

void Player::onStickMoved(Vec2 angle, Ref *pSender) {
    this->angle = angle;
    
    float a = angle.getAngle();
}

void Player::onStickEnded(Vec2 angle, Ref *pSender) {
    touchJoystick = false;
}

void Player::calculateGridCoord(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	gX = (this->getPositionX() + (24 * mapWidth - origin.x)) / 48;
	gY = (this->getPositionY() + (24 * mapHeight - origin.y)) / 48;
}

void Player::gridCoordUpdate(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	this->setPosition(Vec2(48 * gX - (24 * mapWidth - origin.x), 48 * gY - (24 * mapHeight - origin.y)));
}

void Player::move() {
	/// 플레이어가 조이스틱 터치했을 때 해당 방향으로 이동
	if (touchJoystick) {
		tempPosition = getPosition() + angle * speed;
	}
}

void Player::collision() {
	HelloWorld *parent = (HelloWorld *)getParent();

	/// 현재 위치를 기준으로 3x3칸을 충돌 검사를 함
	for (int i = MAX(gY - 1, 0); i < MIN(gY + 2, parent->mapHeight); i++) {
		for (int j = MAX(gX - 1, 0); j < MIN(gX + 2, parent->mapWidth); j++) {
			/// 벽이나 문이면
			if (parent->checkSolidObject(j, i)) {
				/// 플레이어와 상대 충돌체의 BoundingBox를 가져옴
				auto playerBB = Rect(Vec2(tempPosition.x + player->getContentSize().width / 2, tempPosition.y), player->getContentSize() * 2);
				auto otherBB = Rect(parent->mapTile[i][j]->getPosition(), Size(48, 48));
				/// 충돌 검사
				if (playerBB.intersectsRect(otherBB)) {
					parent->mapTile[i][j]->setColor(Color3B::RED);

					/// 0: left, 1: right, 2: up, 3: down
					int dir = 0;

					float angle = CC_RADIANS_TO_DEGREES((playerBB.origin - otherBB.origin).getAngle());

					if (angle > -45 && angle <= 45) dir = 1;
					else if (angle > 45 && angle <= 135) dir = 2;
					else if ((angle > 135 && angle <= 180) || (angle < -135 && angle >= -180)) dir = 0;
					else dir = 3;

					//if (dir == 0) {
					//	tempPosition.x = otherBB.getMinX() - 40;
					//} else if (dir == 1) {
					//	tempPosition.x = otherBB.getMaxX() + 8;
					//} else if (dir == 2) {
					//	tempPosition.y = otherBB.getMaxY();
					//} else {
					//	tempPosition.y = otherBB.getMinY() - 48;
					//}
				}
			}

		}
	}
}

void Player::updateZOrder() {
	HelloWorld *parent = (HelloWorld *)getParent();

	/// 플레이어 z-order 변경
	int zorder = parent->mapTile[int(clampf(gY, 0, parent->mapHeight - 1))][int(clampf(gX, 0, parent->mapWidth - 1))]->getGlobalZOrder() + 1000;
	this->setGlobalZOrder(zorder);
	for (auto &i : this->getChildren()) {
		i->setGlobalZOrder(zorder);
	}
}

void Player::updatePosition() {
	/// 계산된 플레이어 위치 실제 적용
	this->setPosition(tempPosition);
	CameraUtil::getInstance()->setPosition(this->getPosition());
}

void Player::checkGameObjects() {
	HelloWorld *parent = (HelloWorld *)getParent();
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	Vec2 check = this->getPosition() + angle * 48;

	int xx = (check.x + (24 * parent->mapWidth - origin.x)) / 48;
	int yy = (check.y + (24 * parent->mapHeight - origin.y)) / 48;

	//parent->mapTile[yy][xx]->setColor(Color3B::BLUE);

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

