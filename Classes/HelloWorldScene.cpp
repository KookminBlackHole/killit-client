#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Joystick.h"
#include "Button.h"
#include "Player.h"
#include "CameraUtil.h"
#include "ZOrder.h"
#include "UIManager.h"
#include "Utils.h"
#include "Definitions.h"

#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace network;
using namespace std;

HelloWorld::~HelloWorld() {
    for (int i = 0; i < mapHeight; i++) delete[] mapData[i];
    delete[] mapData;
    
    for (int i = 0; i < mapHeight; i++) delete[] mapTile[i];
    delete[] mapTile;
    
    for (int i = 0; i < mapHeight; i++) delete[] mapFog[i];
    delete[] mapFog;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = visibleSize / 2;
    
    auto bg = LayerColor::create(Color4B::BLACK);
    this->addChild(bg);

//    auto waitLabel = Label::createWithTTF("상대방을 기다리는 중입니다", "res/NanumGothic.ttf", 24);
//    waitLabel->setPosition(origin);
//    this->addChild(waitLabel);
//
//    waitLabel->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=] {waitLabel->setString("상대방을 기다리는 중입니다"); }), DelayTime::create(0.5f), CallFunc::create([=] {waitLabel->setString("상대방을 기다리는 중입니다.");}), DelayTime::create(0.5f), CallFunc::create([=] {waitLabel->setString("상대방을 기다리는 중입니다.."); }), DelayTime::create(0.5f), CallFunc::create([=] {waitLabel->setString("상대방을 기다리는 중입니다..."); }), DelayTime::create(0.5f), NULL)));
//
//    client = SocketIO::connect("http://104.131.125.14:8080", *this);
//
//    client->on("connected", [&](SIOClient *client, const std::string &data) {
//        auto send = createData("name", "\"Ho\"", "");
//        client->emit("player-ready", send);
//    });
//
//    client->on("game:start", [&](SIOClient *client, const std::string &data) {
//        MessageBox(data.c_str(), "");
//    });
//
//    // 디버그용 (R키를 누르면 재시작함)
//    auto listen = EventListenerKeyboard::create();
//    listen->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *e) {
//        if (keyCode == EventKeyboard::KeyCode::KEY_R) {
//            Director::getInstance()->replaceScene(HelloWorld::create());
//        }
//    };
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen, this);
//
//    client->on("start", [&](SIOClient *client, const std::string &data) {
//        client->emit("start", "");
//
//        client->on("create-this-player", [&](SIOClient *c, const string &data) {
//            auto otherPlayer = Player::create(2, 2, false);
//            otherPlayers.push_back(otherPlayer);
//            this->addChild(otherPlayer);
//        });
//
//        client->on("other-player", [&](SIOClient *c, const string &data) {
//            if (otherPlayers.size() > 0) {
//                rapidjson::Document doc;
//                doc.Parse(data.c_str());
//                float x = doc["x"].GetDouble(), y = doc["y"].GetDouble();
//                float dx = doc["dirX"].GetDouble(), dy = doc["dirY"].GetDouble();
//                float angle = doc["angle"].GetDouble();
//
//                otherPlayers.front()->angle = angle;
//
//                delay = time - lastTime;
//                lastTime = time;
//                time = 0;
//
//                syncPosition = Vec2(x, y);
//                syncVelocity = Vec2(dx, dy) * otherPlayers.front()->speed;
//            }
//        });
//
//        createGame(0, 0);
//    });
    
    createGame(0, 0);

    return true;
}

void HelloWorld::createGame(float x, float y) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = visibleSize / 2;

	CameraUtil::getInstance()->initialize(this);

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
    mapTile = new GameObject**[mapHeight];
	mapFog = new Sprite**[mapHeight];
	for (int i = 0; i < mapHeight; i++) {
        mapTile[i] = new GameObject*[mapWidth];
		mapFog[i] = new Sprite*[mapWidth];
		for (int j = 0; j < mapWidth; j++) {
			Vec2 pos = Vec2(j * TILE_SIZE + origin.x - TILE_SIZE * mapWidth * 0.5,
				i * TILE_SIZE + origin.y - TILE_SIZE * mapHeight * 0.5);
			if (mapData[i][j] <= 10) {/// 게임 오브젝트 생성
				mapTile[i][j] = GameObject::create("res/tile0.png", mapData[i][j]);
				mapTile[i][j]->setSolidObject(false);
                mapTile[i][j]->setZOrder(zorder - 1000);
				if (mapData[i][j] == 1) {
					auto door = Sprite::create("res/tile2.png");
					door->getTexture()->setAliasTexParameters();
                    door->setAnchorPoint(Vec2(0.5f, 0.25f));
                    door->setGlobalZOrder(zorder);
					mapTile[i][j]->addChild(door);
                    
                    mapTile[i][j]->setSolidObject(true);
                    mapTile[i][j]->setSolidArea(Rect(-Vec2(TILE_SIZE_HALF, TILE_SIZE_HALF), { TILE_SIZE, TILE_SIZE }));
                    mapTile[i][j]->type = mapData[i][j];
				}
			} else { /// 맵 타일
				int idx = mapData[i][j] - 11;

				auto sf = SpriteFrame::create("res/tileset_wall2.png", Rect(REAL_TILE_WIDTH * (idx % 7), REAL_TILE_HEIGHT * (idx / 7), REAL_TILE_WIDTH, REAL_TILE_HEIGHT));

				mapTile[i][j] = GameObject::create(sf, mapData[i][j]);
				mapTile[i][j]->setSolidObject(true);
				mapTile[i][j]->setSolidArea(Rect(-Vec2(TILE_SIZE_HALF, TILE_SIZE_HALF), { TILE_SIZE, TILE_SIZE }));
				mapTile[i][j]->image->setAnchorPoint(Vec2(0.5f, 0.25f));
				mapTile[i][j]->setZOrder(zorder);
			}

            mapTile[i][j]->setPosition(pos);
			mapTile[i][j]->setVisible(false);
			this->addChild(mapTile[i][j]);

            /// 맵 시야 생성
			mapFog[i][j] = Sprite::create("res/tile5.png");
			mapFog[i][j]->setGlobalZOrder(ZORDER::FOG);
			mapFog[i][j]->getTexture()->setAliasTexParameters();
			mapFog[i][j]->setScale(2);
			mapFog[i][j]->setPosition(pos.x, pos.y + TILE_SIZE);
			mapFog[i][j]->setVisible(false);
            
			this->addChild(mapFog[i][j]);
		}
		zorder -= 1;
	}

	player = Player::create(2, 2, true);
	player->gridCoordUpdate(mapWidth, mapHeight);
	this->addChild(player);

	auto lb = Label::createWithSystemFont("x: 0\ny: 0", "", 24);
	lb->setTextColor(Color4B::RED);
	lb->setPosition(20, visibleSize.height - 20);
	lb->setAnchorPoint(Vec2(0, 1));
	lb->setName("debug1");
	lb->setGlobalZOrder(ZORDER::UI);
	CameraUtil::getInstance()->addUIChild(lb);

	auto uim = UIManager::create(this);
	CameraUtil::getInstance()->addUIChild(uim);

	auto dn = DrawNode::create(4);
	dn->setName("debug2");
	dn->setGlobalZOrder(ZORDER::UI);
	this->addChild(dn);

	this->scheduleUpdate();
}

void HelloWorld::update(float dt) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	int pX = player->gX, pY = player->gY;

	/// 이전에 그려진 맵 지우기
	for (int i = max(pY - 6, 0); i < min(pY + 6, mapHeight); i++) {
		for (int j = max(pX - 9, 0); j < min(pX + 10, mapWidth); j++) {
			mapTile[i][j]->setVisible(false);
			mapFog[i][j]->setVisible(false);
			mapFog[i][j]->setOpacity(255 * 1.0f);
		}
	}

	/// 플레이어 좌표를 타일맵 좌표로 변환
	player->calculateGridCoord(mapWidth, mapHeight);
	pX = player->gX;
	pY = player->gY;

	/// 현재 위치에서 맵 그리기
	for (int i = max(pY - 6, 0); i < min(pY + 6, mapHeight); i++) {
		for (int j = max(pX - 9, 0); j < min(pX + 10, mapWidth); j++) {
            mapTile[i][j]->setOpacity(255);
			mapTile[i][j]->setVisible(true);
			mapFog[i][j]->setVisible(true);
		}
	}

//	안개 투명도 설정
	for (float r = 0; r < 360; r += 0.5f) {
		bool escape = false;
		for (int i = 0; i < 20; i++) {
			if (escape) break;

			float x = player->getPositionX() + cos(CC_DEGREES_TO_RADIANS(r)) * i * TILE_SIZE_HALF;
			float y = player->getPositionY() + sin(CC_DEGREES_TO_RADIANS(r)) * i * TILE_SIZE_HALF;

			int gX = (x + (TILE_SIZE_HALF * (mapWidth - 1) - origin.x)) / TILE_SIZE + 1;
			int gY = (y + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;

			if (gY > mapHeight - 1 || gY < 0 || gX > mapWidth - 1 || gX < 0) continue;

			if (isSolidObject(gX, gY)) escape = true;

			mapFog[gY][gX]->setOpacity(255 * MAX(((i - 12.0f) / 8.0f), 0));
		}
	}

	player->updateZOrder();
	player->move();
	player->collision();
	player->checkSolidObjects();
	player->updatePosition();

//    auto data = createData("x", to_string(player->getPositionX()).c_str(),
//                           "y", to_string(player->getPositionY()).c_str(),
//                           "dirX", to_string((player->direction * player->speed).x).c_str(),
//                           "dirY", to_string((player->direction * player->speed).y).c_str(),
//                           "angle", to_string(player->angle).c_str(), "");
//    client->emit("player-position", data);
    
    time += dt;

    updatePosition(dt);
}

void HelloWorld::updatePosition(float dt) {
    if (otherPlayers.size() > 0) {
        otherPlayers.front()->updateZOrder();
        float d = time / delay;
        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString(to_string(time) + ", " + to_string(delay) + ", " + to_string(d));
//        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setScale(d);
        
//        auto pos = lerp(otherPlayers.front()->getPosition(), syncPosition, 0.5);
        auto pos = syncPosition;
        otherPlayers.front()->setPosition(pos);
    }
}

bool HelloWorld::isSolidObject(int x, int y) {
    return mapTile[y][x]->isSolidObject();
}

void HelloWorld::onConnect(SIOClient *client) {
}

void HelloWorld::onMessage(SIOClient *client, const std::string &data) {
}

void HelloWorld::onClose(SIOClient *client) {
}

void HelloWorld::onError(SIOClient *client, const std::string &data) {
}
