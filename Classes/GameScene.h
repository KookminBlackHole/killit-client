#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#include "network/SocketIO.h"

#include "GameObject.h"
#include "Button.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d::network;

class Player;

class GameScene : public cocos2d::Scene, public SocketIO::SIODelegate {
public:
	virtual ~GameScene();
	virtual bool init();

	void update(float dt);

	CREATE_FUNC(GameScene);

	bool isSolidObject(int x, int y);

	/// Socket.io관련 함수.
	void onConnect(SIOClient *client) override;
	void onMessage(SIOClient *client, const std::string &data) override;
	void onClose(SIOClient *client) override;
	void onError(SIOClient *client, const std::string &data) override;

	void createMap(float x, float y);

	void gameStart(const std::string &ip);

	/// 상대 플레이어의 위치를 보정하기 위한 함수
	void updatePosition(float dt);

	void emit(const std::string &event, const std::string &args);

public:
	/// 안개 타일
	cocos2d::Sprite ***mapFog;
	/// 맵 타일
	GameObject ***mapTile;

	/// 내 캐릭터
	Player *player;
	/// 상대 캐릭터
	std::vector<Player*> otherPlayers;

	/// 상호작용 버튼(공격 버튼)
	Button * interactButton;

	SIOClient *client;

	/// 게임 uuid
	std::string uuid;

	/// 상대 플레이어의 위치를 보정하는데 필요한 변수.
	cocos2d::Vec2 syncPosition, syncVelocity, otherDirection, otherPosition;
	float otherSpeed = 0;
	float time = 0, lastTime, delay = 0;
};

#endif // __HELLOWORLD_SCENE_H__

