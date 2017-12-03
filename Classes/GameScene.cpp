#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Joystick.h"
#include "Button.h"
#include "Player.h"
#include "CameraUtil.h"
#include "ZOrder.h"
#include "UIManager.h"
#include "Utils.h"
#include "Definitions.h"
#include "MapLoader.h"

USING_NS_CC;
using namespace network;
using namespace std;

//#define MULTIPLAY

GameScene::~GameScene() {
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

    for (int i = 0; i < mapHeight; i++) delete[] mapTile[i];
    delete[] mapTile;
    
    for (int i = 0; i < mapHeight; i++) delete[] mapFog[i];
    delete[] mapFog;
}

// on "init" you need to initialize your instance
bool GameScene::init() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = visibleSize / 2;
    
    CameraUtil::getInstance()->initialize(this);
    
    auto bg = LayerColor::create(Color4B::WHITE);
    this->addChild(bg);
    
#ifdef MULTIPLAY
    auto connectLayer = Layer::create();
    connectLayer->setName("connectLayer");
    this->addChild(connectLayer);
    
    auto lobbyTitle = Label::createWithSystemFont("lobby", "", 32);
    lobbyTitle->setPosition(origin.x, visibleSize.height - 72);
    lobbyTitle->setTextColor(Color4B::BLACK);
    connectLayer->addChild(lobbyTitle);

    auto nicknameField = TextFieldTTF::textFieldWithPlaceHolder("0.0.0.0", Size(320, 28), TextHAlignment::LEFT, "fonts/NanumGothic.ttf", 24);
    nicknameField->setPosition(origin.x, origin.y);
    nicknameField->setTextColor(Color4B::BLACK);
    connectLayer->addChild(nicknameField);
    
    static int idx = 0;
    static float force = 0;
    static int oy[8];
    
    auto loadingCircle = Sprite::create("res/player.png");
    loadingCircle->setPosition(visibleSize.width - 120, 120);
    loadingCircle->getTexture()->setAliasTexParameters();
    loadingCircle->setScale(2);
    loadingCircle->setTextureRect(Rect(0 * 32, 0 * 36, 32, 36));
    loadingCircle->schedule([=](float dt) {
        loadingCircle->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
        idx = (idx + 1) % 8;
    }, 0.15f, "loading");
    //loadingCircle->runAction(RepeatForever::create(RotateBy::create(1.2f, -360)));
	loadingCircle->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0f, Vec2(0, 8)), MoveBy::create(1.0f, Vec2(0, -8)), nullptr)));
    connectLayer->addChild(loadingCircle);
    
    auto loadingText = Label::createWithTTF("Loading..", "fonts/NanumGothic.ttf", 24);
    loadingText->setPosition(loadingCircle->getPositionX(), loadingCircle->getPositionY() - 45);
    loadingText->setTextColor(Color4B::BLACK);
    
    for (int i = 0; i < loadingText->getStringLength(); i++) {
        auto txt = loadingText->getLetter(i);
        oy[i] = txt->getPositionY();
    }
    
    loadingText->schedule([=](float dt) {
        for (int i = 0; i < loadingText->getStringLength(); i++) {
            auto txt = loadingText->getLetter(i);
            txt->setPositionY(oy[i] + sin(CC_RADIANS_TO_DEGREES(force + i)) * 4.0f);
        }
        force += 0.0016f;
        if (force > 90) force = 0;
    }, "loadingText");
    connectLayer->addChild(loadingText);
    
    auto tl = EventListenerTouchOneByOne::create();
    tl->onTouchBegan = [=](Touch *t, Event *e)->bool {
        auto pos = Vec2(t->getLocation());
        
        if (nicknameField->getBoundingBox().containsPoint(pos)) {
            nicknameField->attachWithIME();
        } else {
            nicknameField->detachWithIME();
        }
        
        return false;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(tl, connectLayer);

    auto startButton = MenuItemFont::create("Start", [=](Ref *r) {
        string ip = "0.0.0.0";
        if (nicknameField->getString() != "") ip = nicknameField->getString();
        gameStart(ip);
    });
    startButton->setFontNameObj("fonts/NanumGothic.ttf");
    startButton->setColor(Color3B::BLACK);

    auto menu = Menu::createWithItem(startButton);
    menu->setPosition(origin.x, origin.y - 200);
    connectLayer->addChild(menu);

    auto listen = EventListenerKeyboard::create();
    listen->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *e) {
        if (keyCode == EventKeyboard::KeyCode::KEY_R) {
            Director::getInstance()->replaceScene(GameScene::create());
        } else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            string ip = "0.0.0.0";
            if (nicknameField->getString() != "") ip = nicknameField->getString();
            gameStart(ip);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen, connectLayer);
    
#else
    createMap(5, 5);
    
#endif
    
    return true;
}

void GameScene::gameStart(const string &ip) {
    client = SocketIO::connect("http://" + ip + ":8080", *this);
    client->on("lobby:connected", [&](SIOClient *client, const std::string &data) {
        auto send = createData({ "name", "\"Hi\"" });
        client->emit("lobby:player-ready", send);
    });

	client->on("game:start", [&](SIOClient *client, const std::string &data) {
		this->getChildByName<Layer*>("connectLayer")->runAction(RemoveSelf::create());

		auto pData = toJson(data);
        
		uuid = pData["uuid"].GetString();

		createMap(pData["x"].GetInt(), pData["y"].GetInt());

		auto otherPlayer = Player::create(pData["otherX"].GetInt(), pData["otherY"].GetInt(), false);
		otherPlayers.push_back(otherPlayer);
		this->addChild(otherPlayer);

		otherPlayer->gridCoordUpdate(MapLoader::getInstance()->getWidth(), MapLoader::getInstance()->getHeight());
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

void GameScene::createMap(float x, float y) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = visibleSize / 2;

    MapLoader::getInstance()->loadData("res/map.txt");
    MapLoader::getInstance()->createMap(this, &mapTile, &mapFog);

	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

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

void GameScene::update(float dt) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

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

	///	안개 투명도 설정
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
	player->move(dt);
	player->collision();
	player->checkSolidObjects();
	player->updatePosition();
    
    time += dt;

    updatePosition(dt);
}

void GameScene::emit(const string &event, const string &args) {
	if (client != nullptr) {
		client->emit(event, args);
	}
}

void GameScene::updatePosition(float dt) {
    if (otherPlayers.size() > 0) {
        otherPlayers.front()->setPosition(otherPlayers.front()->getPosition() + otherDirection * otherSpeed * (dt * 60));
    }
}

bool GameScene::isSolidObject(int x, int y) {
    return mapTile[y][x]->isSolidObject();
}

void GameScene::onConnect(SIOClient *client) {
}

void GameScene::onMessage(SIOClient *client, const std::string &data) {
}

void GameScene::onClose(SIOClient *client) {
}

void GameScene::onError(SIOClient *client, const std::string &data) {
}
