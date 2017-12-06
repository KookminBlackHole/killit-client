//
//  Player.cpp
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#include "Player.h"

#include "GameScene.h"
#include "ZOrder.h"
#include "CameraUtil.h"
#include "Definitions.h"
#include "Raycast.h"
#include "Utils.h"
#include "MapLoader.h"

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
    setScale(DEFAULT_SCALE);
    
	/// 플레이어 충돌 영역 설정.
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
    GameScene *parent = (GameScene *)getParent();

	float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
	if (angle < 0 || angle <= -180) angle += 360;
	if (angle >= 360) angle -= 360;

	/// 플레이어 이동 방향을 0~7까지의 숫자로 변환.
	if (direction != Vec2::ZERO) {
		idx = (int(floor(angle / 45 + 0.5f)) % 8);
		player->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
	}

	/// 각도도 함께 보내줄 예정.

    auto send = createData({ "uuid", "\"" + parent->uuid + "\"", "x", toString(getPositionX()).c_str(), "y", toString(getPositionY()).c_str(), "dirX", toString(direction.x).c_str(), "dirY", toString(direction.y).c_str(), "speed", toString(speed).c_str() });
    parent->emit("game:update-player-direction", send);
}

//void Player::onAngleChanged(int idx) {
//    GameScene *parent = (GameScene *)getParent();
//    
//    player->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
//    auto send = createData({ "uuid", "\"" + parent->uuid + "\"", "angle", toString(idx) });
//    parent->emit("game:update-player-angle", send);
//}

void Player::calculateGridCoord() {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

	gX = ((this->getPositionX()) + (TILE_SIZE_HALF * (mapWidth - 1) - origin.x)) / TILE_SIZE + 1;
	gY = ((this->getPositionY()) + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;
}

void Player::gridCoordUpdate() {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

    tempPosition = Vec2(TILE_SIZE * gX - (TILE_SIZE_HALF * mapWidth - origin.x), TILE_SIZE * gY - (TILE_SIZE_HALF * mapHeight - origin.y));
	this->setPosition(tempPosition);
}

void Player::move(float dt) {
	/// 조이스틱이 움직일 때만 위치 입력
	if (touchJoystick) {
		tempPosition = getPosition() + direction * speed * (dt * 60);
	}
}

/// 충돌 범위가 정사각형이 아닌 경우 제대로 동작하지 않음
void Player::collision() {
	GameScene *parent = (GameScene *)getParent();
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();
    
	/// 플레이어 위치를 중심으로 3x3칸 충돌 검사를 함.
    /// 충돌 처리 시 벽에 끼이는 문제를 해결하기 위해 충돌 검사 순서를 가운데 - 위 - 아래 순으로 바꿈.
    int yOrder[3] = { 0, 1, -1 }, xOrder[3] = { 0, 1, -1 };
    for (int k = 0; k < 3; k++) {
		/// 검사 좌표가 맵 범위를 벗어나지 않도록 제한.
        int i = clampf(gY + yOrder[k], 0, mapHeight - 1);
        for (int l = 0; l < 3; l++) {
            int j = clampf(gX + xOrder[l], 0, mapWidth - 1);
			/// 가운데 플레이어 위치는 검사하지 않음.
            if (i == gY && j == gX) continue;
			/// 현재 검사한 칸이 솔리드 오브젝트라면,
            if (parent->isSolidObject(j, i)) {
				/// 플레이어와 타일의 바운딩 박스를 가져옴.
                auto playerBB = Rect(tempPosition + solidBB.origin, solidBB.size);
				auto otherBB = parent->mapTile[i][j]->getBoundingBox();//Rect(parent->mapTile[i][j]->getPosition() - Size(TILE_SIZE_HALF, TILE_SIZE_HALF), Size(TILE_SIZE, TILE_SIZE));
				/// 충돌 검사
				if (playerBB.intersectsRect(otherBB)) {
//                    parent->mapTile[i][j]->setColor(Color3B::MAGENTA);

                    /// 벽 기준
					/// 0: left, 1: right, 2: up, 3: down
					/// 캐릭터와 충돌체의 각도를 계산함.
					float angle = CC_RADIANS_TO_DEGREES((Vec2(playerBB.getMidX(), playerBB.getMidY()) - Vec2(otherBB.getMidX(), otherBB.getMidY())).getAngle());

					int dir = 0;

					if (angle > -45 && angle <= 45) dir = 1;
					else if (angle > 45 && angle <= 135) dir = 2;
					else if ((angle > 135 && angle <= 180) || (angle < -135 && angle >= -180)) dir = 0;
					else dir = 3;

					/// 부딪힌 각도에 따라서 위치 조정을 함.
					if (dir == 0) {
						/// 플레이어가 벽의 왼쪽에 부딪히면 플레이어의 x좌표를 벽의 왼쪽 끝으로 옮겨줌. 
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
	GameScene *parent = (GameScene *)getParent();
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

	/// 플레이어의 zorder를 현재 올라와 있는 타일의 zorder보다 높게 올림.
    int zorder = parent->mapTile[int(clampf(gY, 0, mapHeight - 1))][int(clampf(gX, 0, mapWidth - 1))]->getGlobalZOrder() + 1000;
    this->setGlobalZOrder(zorder);
    for (auto &i : this->getChildren()) {
        i->setGlobalZOrder(zorder);
    }
}

void Player::updatePosition() {
	this->setPosition(tempPosition);
    
	CameraUtil::getInstance()->setPosition((this->getPositionX()), (this->getPositionY()));
}

//bool Player::checkGameObjects() {
//	GameScene *parent = (GameScene *)getParent();
//	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
//	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();
//
//	Vec2 check = this->getPosition() + direction * TILE_SIZE;
//
//    int xx = (check.x + (TILE_SIZE_HALF * (mapWidth - 1) - origin.x)) / TILE_SIZE + 1;
//	int yy = (check.y + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;
//
//	switch (MapLoader::getInstance()->getMapData(xx, yy)) {
//        case 1:
//			MapLoader::getInstance()->setMapData(xx, yy, 3);
//            break;
//        case 3:
//			MapLoader::getInstance()->setMapData(xx, yy, 1);
//            break;
//        default:
//            return false;
//	}
//    
//    return true;
//}

void Player::checkSolidObjects() {
    GameScene *parent = (GameScene *)getParent();
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();
    
    if (gY > 0 && parent->isSolidObject(gX, gY - 1)) {
        for (int i = MAX(gX - 1, 0); i < MIN(gX + 2, mapWidth - 1); i++) {
            if (parent->isSolidObject(i, gY - 1)) {
                parent->mapTile[gY - 1][i]->setOpacity(255 * 0.5f);
            }
        }
    }
}

void Player::attack() {
    GameScene *parent = (GameScene *)getParent();
    Vec2 dot;
    this->raycast(parent->mapTile, angle, 500, &dot);
    
    parent->getChildByName<DrawNode*>("debug2")->clear();
    parent->getChildByName<DrawNode*>("debug2")->drawLine(getPosition(), dot, Color4F::BLACK);
    parent->getChildByName<DrawNode*>("debug2")->drawDot(dot, 4, Color4F::RED);
}

bool Player::raycast(GameObject ***objects, float angle, float length, Vec2 *out) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

    auto ray = getPosition() - Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
    bool escape = false;
    for (int i = 0; i < length && !escape; i += 4) {
        ray += Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle)) * 4;
        
		/// 레이캐스트 검사를 할 때 모든 오브젝트를 검사하는게 아닌 선이 닿는 범위 내의 오브젝트만 검사함. 
		/// 캐릭터 충돌 레이캐스트 따로 만들어야 할듯.
        int lim = floor(i / TILE_SIZE) + 1;
        for (int k = max(gY - lim, 0); k < min(gY + lim + 1, mapHeight) && !escape; k++) {
            for (int l = max(gX - lim, 0); l < min(gX + lim + 1, mapWidth) && !escape; l++) {
                if (objects[k][l]->isSolidObject()) {
                    if (objects[k][l]->getBoundingBox().containsPoint(ray)) escape = true;
                }
            }
        }	
    }
    
    if (out != nullptr) *out = ray;
    
    return escape;
}
