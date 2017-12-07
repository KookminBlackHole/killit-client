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
		/// 디버그 용으로 ip 입력하게 함.
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

	/// 플레이어가 서버와 연결 성공 후 메시지를 받음.
    client->on("lobby:connected", [&](SIOClient *client, const std::string &data) {
        auto send = createData({ "name", "\"Hi\"" });
		/// 받았으면 플레이어 정보와 함께 준비 완료 메시지 보냄.
        client->emit("lobby:player-ready", send);
    });

	/// 플레이어 2명이 모여 서버에서 준비가 완료되면 메시지를 받음.
	client->on("game:start", [&](SIOClient *client, const std::string &data) {
		/// 로비 화면을 지움.
		this->getChildByName<Layer*>("connectLayer")->runAction(RemoveSelf::create());

		auto pData = toJson(data);
        
		/// uuid 저장 후,
		uuid = pData["uuid"].GetString();
		/// 맵을 만듦 (이 때 자기 캐릭터 생성).
		createMap(pData["x"].GetInt(), pData["y"].GetInt());

		/// 상대 플레이어 생성
		auto otherPlayer = Player::create(pData["otherX"].GetInt(), pData["otherY"].GetInt(), false);
		otherPlayers.push_back(otherPlayer);
		this->addChild(otherPlayer);

		/// 상대 플레이어 위치 업데이트.
		otherPlayer->gridCoordUpdate();
	});

	/// 상대 플레이어의 이동 방향이 변경 됐을 때,
	client->on("game:feed-player-direction", [&](SIOClient *client, const std::string &data) {
		auto pData = toJson(data);

		/// 적용 후 상대 플레이어의 좌표를 실제 위치로 동기화 함.
		otherDirection = Vec2(pData["dirX"].GetDouble(), pData["dirY"].GetDouble());
		otherPosition = Vec2(pData["x"].GetDouble(), pData["y"].GetDouble());
		otherSpeed = pData["speed"].GetDouble();

		/// 상대 플레이어 이미지 각도도 동기화해야 함.

		otherPlayers.front()->setPosition(otherPosition);
	});

	//client->on("game:feed-player-angle", [&](SIOClient *client, const std::string &data) {
	//	auto pData = toJson(data);

	//	int idx = pData["angle"].GetInt();

	//	otherPlayers.front()->player->setTextureRect(Rect(0 * 32, idx * 36, 32, 36));
	//});
}

void GameScene::createMap(float x, float y) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = visibleSize / 2;

	/// 맵 로드 후 만듦.
    MapLoader::getInstance()->loadData("res/map.txt");
    MapLoader::getInstance()->createMap(this, &mapTile, &mapFog);

	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

	/// 내 캐릭터 생성 후 위치 업데이트. 
	player = Player::create(x, y, true);
	player->gridCoordUpdate();
    this->addChild(player);

	/// 디버그용 라벨.
	auto lb = Label::createWithSystemFont("x: 0\ny: 0", "", 24);
	lb->setTextColor(Color4B::RED);
	lb->setPosition(20, visibleSize.height - 20);
	lb->setAnchorPoint(Vec2(0, 1));
	lb->setName("debug1");
	lb->setGlobalZOrder(ZORDER::UI);
	CameraUtil::getInstance()->addUIChild(lb);

	/// uimanager 생성.
	auto uim = UIManager::create(this);
	CameraUtil::getInstance()->addUIChild(uim);

	auto dn = DrawNode::create(4);
	dn->setName("debug2");
	dn->setGlobalZOrder(ZORDER::UI);
	this->addChild(dn);

	/// update함수 스케쥴 등록.
	this->scheduleUpdate();
}

void GameScene::update(float dt) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	int mapWidth = MapLoader::getInstance()->getWidth(), mapHeight = MapLoader::getInstance()->getHeight();

	/// 이전 플레이어의 위치 가져옴.
	int pX = player->gX, pY = player->gY;

	/// 이전에 그려진 맵 지우기
	for (int i = max(pY - 6, 0); i < min(pY + 6, mapHeight); i++) {
		for (int j = max(pX - 9, 0); j < min(pX + 10, mapWidth); j++) {
			mapTile[i][j]->setVisible(false);
			mapFog[i][j]->setVisible(false);
			mapFog[i][j]->setOpacity(255 * 1.0f);
		}
	}

	/// 플레이어 현재 위치 가져옴.
	player->calculateGridCoord();
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
	/// 0.5도 간격으로 360도 검사.
    for (float r = 0; r < 360; r += 0.5f) {
        bool escape = false;
        for (int i = 0; i < 20; i++) {
			/// 벽과 충돌 후 바로 break를 하지 않는 이유는 부딪힌 벽은 보여야 하기 때문.
            if (escape) break;

			/// 플레이어의 위치에서 r 각도로 타일 사이즈의 반 길이만큼 더해줌.
            float x = player->getPositionX() + cos(CC_DEGREES_TO_RADIANS(r)) * i * TILE_SIZE_HALF;
            float y = player->getPositionY() + sin(CC_DEGREES_TO_RADIANS(r)) * i * TILE_SIZE_HALF;

			/// 그 더해진 각도로 나온 좌표를 타일맵 좌표로 변경.
            int gX = (x + (TILE_SIZE_HALF * (mapWidth - 1) - origin.x)) / TILE_SIZE + 1;
            int gY = (y + (TILE_SIZE_HALF * (mapHeight - 1) - origin.y)) / TILE_SIZE + 1;

			/// 변환된 타일맵 좌표가 맵을 벗어나는지 체크.
            if (gY > mapHeight - 1 || gY < 0 || gX > mapWidth - 1 || gX < 0) continue;
			/// 변환된 위치의 맵 타일이 솔리드 오브젝트인지 검사
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
