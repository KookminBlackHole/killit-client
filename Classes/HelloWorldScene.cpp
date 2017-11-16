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
    
    //auto connectLayer = Layer::create();
    //connectLayer->setName("connectLayer");
    //this->addChild(connectLayer);
    //
    //auto lobbyTitle = Label::createWithSystemFont("lobby", "", 32);
    //lobbyTitle->setPosition(origin.x, visibleSize.height - 72);
    //lobbyTitle->setTextColor(Color4B::WHITE);
    //connectLayer->addChild(lobbyTitle);

    //auto nicknameField = TextFieldTTF::textFieldWithPlaceHolder("0.0.0.0", Size(320, 28), TextHAlignment::LEFT, "fonts/NanumGothic.ttf", 24);
    //nicknameField->setPosition(origin.x, origin.y);
    //connectLayer->addChild(nicknameField);
    //
    //auto tl = EventListenerTouchOneByOne::create();
    //tl->onTouchBegan = [=](Touch *t, Event *e)->bool {
    //    auto pos = Vec2(t->getLocation());
    //    
    //    if (nicknameField->getBoundingBox().containsPoint(pos)) {
    //        nicknameField->attachWithIME();
    //    } else {
    //        nicknameField->detachWithIME();
    //    }
    //    
    //    return false;
    //};
    //this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(tl, connectLayer);

    //auto startButton = MenuItemFont::create("Start", [=](Ref *r) {
    //    string ip = "0.0.0.0";
    //    if (nicknameField->getString() != "") ip = nicknameField->getString();
    //    gameStart(ip);
    //});
    //startButton->setFontNameObj("fonts/NanumGothic.ttf");
    //startButton->setColor(Color3B::WHITE);

    //auto menu = Menu::createWithItem(startButton);
    //menu->setPosition(origin.x, origin.y - 200);
    //connectLayer->addChild(menu);

    //auto listen = EventListenerKeyboard::create();
    //listen->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *e) {
    //    if (keyCode == EventKeyboard::KeyCode::KEY_R) {
    //        Director::getInstance()->replaceScene(HelloWorld::create());
    //    } else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
    //        string ip = "0.0.0.0";
    //        if (nicknameField->getString() != "") ip = nicknameField->getString();
    //        gameStart(ip);
    //    }
    //};
    //this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen, connectLayer);
    
    createGame(2, 2);

    return true;
}

void HelloWorld::gameStart(const string &ip) {
    client = SocketIO::connect("http://" + ip + ":8080", *this);
    client->on("lobby:connected", [&](SIOClient *client, const std::string &data) {
        auto send = createData({ "name", "\"Hi\"" });
        client->emit("lobby:player-ready", send);
    });

	client->on("game:start", [&](SIOClient *client, const std::string &data) {

		this->getChildByName<Layer*>("connectLayer")->runAction(RemoveSelf::create());

		auto pData = toJson(data);

		uuid = pData["uuid"].GetString();

		createGame(pData["x"].GetInt(), pData["y"].GetInt());

		auto otherPlayer = Player::create(pData["otherX"].GetInt(), pData["otherY"].GetInt(), false);
		otherPlayers.push_back(otherPlayer);
		this->addChild(otherPlayer);

		otherPlayer->gridCoordUpdate(mapWidth, mapHeight);
	});

	client->on("game:feed-player-direction", [&](SIOClient *client, const std::string &data) {
		auto pData = toJson(data);

		otherDirection = Vec2(pData["dirX"].GetDouble(), pData["dirY"].GetDouble());
		otherPosition = Vec2(pData["x"].GetDouble(), pData["y"].GetDouble());
		otherSpeed = pData["speed"].GetDouble();

		otherPlayers.front()->setPosition(otherPosition);
	});

	client->on("game:feed-player-angle", [&](SIOClient *client, const std::string &data) {
		auto pData = toJson(data);

		int idx = pData["angle"].GetInt();

		otherPlayers.front()->player->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
	});
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

	player = Player::create(x, y, true);
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
//    auto minVector = Vec2(1, 0), maxVector = Vec2::forAngle(CC_DEGREES_TO_RADIANS(45));
//    for (int x = max(pX - 0, 0); x < min(pX + 4, mapWidth); x++) {
//        for (int y = max(pY - 0, 0); y < min(pY + 4, mapHeight); y++) {
//            int maxY = (((maxVector * TILE_SIZE * (x - pX)) + player->getPosition()).y + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;
//            int minY = (((minVector * TILE_SIZE * (x - pX)) + player->getPosition()).y + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;
//            if (y <= maxY && y >= minY) mapFog[y][x]->setOpacity(0);
//        }
//    }

	player->updateZOrder();
	player->move(dt);
	player->collision();
	player->checkSolidObjects();
	player->updatePosition();
    
    time += dt;

    updatePosition(dt);
}

void HelloWorld::emit(const string &event, const string &args) {
	if (client != nullptr) {
		client->emit(event, args);
	}
}

void HelloWorld::updatePosition(float dt) {
    if (otherPlayers.size() > 0) {
        otherPlayers.front()->setPosition(otherPlayers.front()->getPosition() + otherDirection * otherSpeed * (dt * 60));
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
