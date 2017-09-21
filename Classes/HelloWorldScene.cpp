#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Joystick.h"
#include "Button.h"
#include "Player.h"
#include "CameraUtil.h"
#include "ZOrder.h"
#include "UIManager.h"
#include "Utils.h"

USING_NS_CC;
using namespace std;

HelloWorld::~HelloWorld() {
    for (int i = 0; i < mapHeight; i++) delete[] mapData[i];
    delete[] mapData;
    
    for (int i = 0; i < mapHeight; i++) delete[] mapTile[i];
    delete[] mapTile;
    
    for (int i = 0; i < mapHeight; i++) delete[] mapObjects[i];
    delete[] mapObjects;
    
    for (int i = 0; i < mapHeight; i++) delete[] mapFog[i];
    delete[] mapFog;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = visibleSize / 2;
    
    CameraUtil::getInstance()->initialize(this);
    
    auto bg = LayerColor::create(Color4B::BLACK);
    this->addChild(bg);
    
	/// 맵 파일 읽음
    auto mapFileString = FileUtils::getInstance()->getStringFromFile("res/map.txt");
    int **tempData, idx = -1;
    for (auto i : split(mapFileString, '\n')) {
        auto data = split(i, ' ');
        if (idx < 0) {
            mapWidth = toInt(data[0]);
            mapHeight = toInt(data[1]);
            
            mapData = new int*[mapHeight];
            tempData = new int*[mapHeight];
            idx++;
        } else {
            mapData[(idx / mapHeight)] = new int[mapWidth];
            tempData[(idx / mapHeight)] = new int[mapWidth];
            for (auto j : data) {
                tempData[(idx / mapHeight)][(idx % mapWidth)] = toInt(j);
                idx++;
            }
        }
    }
    
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            mapData[mapHeight - i - 1][j] = tempData[i][j];
        }
    }
    
    for (int i = 0; i < mapHeight; i++) delete[] tempData[i];
    delete[] tempData;
    
    int zorder = ZORDER::WALL;
    
    /// 맵 타일 초기화 및 맵 데이터 매핑
    mapTile = new Sprite**[mapHeight];
    mapObjects = new Sprite**[mapHeight];
    mapFog = new Sprite**[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        mapTile[i] = new Sprite*[mapWidth];
        mapObjects[i] = new Sprite*[mapWidth];
        mapFog[i] = new Sprite*[mapWidth];
        for (int j = 0; j < mapWidth; j++) {
            int tileSize = 24 * 2;
            Vec2 pos = Vec2(j * tileSize + origin.x - tileSize * mapWidth * 0.5,
                            i * tileSize + origin.y - tileSize * mapHeight * 0.5);

			mapObjects[i][j] = nullptr;
			if (mapData[i][j] <= 10) { /// 게임 오브젝트 및 바닥
				/// 게임 오브젝트 생성
				switch (mapData[i][j]) {
				case 1: /// 문 (오브젝트 밑에도 바닥이 필요해서 break 안씀)
					mapObjects[i][j] = Sprite::create("res/tile2.png");
					mapObjects[i][j]->setGlobalZOrder(zorder);
					mapObjects[i][j]->getTexture()->setAliasTexParameters();
					mapObjects[i][j]->setScale(2);
					mapObjects[i][j]->setPosition(pos);
					mapObjects[i][j]->setAnchorPoint(Vec2(0.5f, 0.25f));
					mapObjects[i][j]->setVisible(false);
					this->addChild(mapObjects[i][j]);
				case 0: /// 바닥
					mapTile[i][j] = Sprite::create("res/tile0.png");
					mapTile[i][j]->setGlobalZOrder(zorder - 1000);
					break;
				}
			} else { /// 맵 타일
				int idx = mapData[i][j] - 11;

				mapTile[i][j] = Sprite::create("res/tileset_wall2.png");
				mapTile[i][j]->setAnchorPoint(Vec2(0.5f, 0.25f));
				mapTile[i][j]->setGlobalZOrder(zorder);

				mapTile[i][j]->setTextureRect(Rect(24 * (idx % 7), 48 * (idx / 7), 24 ,48));
			}
            
            mapTile[i][j]->getTexture()->setAliasTexParameters();
            mapTile[i][j]->setScale(2);
            mapTile[i][j]->setPosition(pos);
            mapTile[i][j]->setVisible(false);
            this->addChild(mapTile[i][j]);

            /// 맵 시야 생성
            mapFog[i][j] = Sprite::create("res/tile4.png");
            mapFog[i][j]->setGlobalZOrder(ZORDER::FOG);
            mapFog[i][j]->getTexture()->setAliasTexParameters();
            mapFog[i][j]->setScale(2);
            mapFog[i][j]->setPosition(pos.x, pos.y + 24 * 2);
            mapFog[i][j]->setVisible(false);
            this->addChild(mapFog[i][j]);
        }
        zorder -= 1;
    }
    
    player = Player::create(2, 2);
	player->gridCoordUpdate(mapWidth, mapHeight);
    this->addChild(player);
    
    auto lb = Label::createWithSystemFont("x: 0\ny: 0", "", 24);
    lb->setTextColor(Color4B::RED);
    lb->setPosition(20, visibleSize.height - 20);
    lb->setAnchorPoint(Vec2(0, 1));
    lb->setName("debug1");
    lb->setGlobalZOrder(ZORDER::UI);
	CameraUtil::getInstance()->addUIChild(lb);
    
//    this->schedule([=](float dt){
//        lb->setString("x: " + to_string(player->gX) + " y: " + to_string(player->gY));
//    }, "debug");

	auto uim = UIManager::create(this);
	CameraUtil::getInstance()->addUIChild(uim);

    auto dn = DrawNode::create(4);
    dn->setName("debug2");
    dn->setGlobalZOrder(ZORDER::UI);
    this->addChild(dn);
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float dt) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	int pX = player->gX, pY = player->gY;
    
    /// 이전에 그려진 맵 지우기
    for (int i = max(pY - 9, 0); i < min(pY + 9, mapHeight); i++) {
        for (int j = max(pX - 14, 0); j < min(pX + 15, mapWidth); j++) {
            mapTile[i][j]->setVisible(false);
            mapFog[i][j]->setVisible(false);
            mapFog[i][j]->setOpacity(255 * 1.0f);
            if (mapObjects[i][j] != nullptr) mapObjects[i][j]->setVisible(false);
        }
    }

    /// 플레이어 좌표를 타일맵 좌표로 변환
	player->calculateGridCoord(mapWidth, mapHeight);
	pX = player->gX;
	pY = player->gY;

    /// 현재 위치에서 맵 그리기
    for (int i = max(pY - 9, 0); i < min(pY + 9, mapHeight); i++) {
        for (int j = max(pX - 14, 0); j < min(pX + 15, mapWidth); j++) {
            mapTile[i][j]->setVisible(true);
            mapTile[i][j]->setOpacity(255);
            mapTile[i][j]->setColor(Color3B::WHITE);
            mapFog[i][j]->setVisible(true);
            if (mapObjects[i][j] != nullptr) mapObjects[i][j]->setVisible(true);
        }
    }
    
    /// 안개 투명도 설정
	for (float r = 0; r < 360; r += 0.5f) {
		bool escape = false;
		for (int i = 0; i < 20; i++) {
			if (escape) break;

			float x = player->getPositionX() + cos(CC_DEGREES_TO_RADIANS(r)) * i * 24;
			float y = player->getPositionY() + sin(CC_DEGREES_TO_RADIANS(r)) * i * 24;
            
			int gX = (x + (24 * (mapWidth - 1) - origin.x)) / 48 + 1;
            int gY = (y + (24 * (mapHeight - 1) - origin.y)) / 48 + 1;
            
			if (gY > mapHeight - 1 || gY < 0 || gX > mapWidth - 1 || gX < 0) continue;

			if (isSolidObject(gX, gY)) escape = true;

			mapFog[gY][gX]->setOpacity(255 * MAX(((i - 12.0f) / 8.0f), 0));
		}
	}
	
	player->updateZOrder();
	player->move();
	player->collision();
//    player->checkGameObjects();
    player->checkSolidObjects();
	player->updatePosition();
}

bool HelloWorld::isSolidObject(int x, int y) {
	int value = mapData[y][x];
	if (value <= 10) { /// 게임 오브젝트
		if (value == 1) return true; /// 문
	} else { /// 블록 타일
		return true;
	}
	return false;
}
