#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#include "network/SocketIO.h"

#include "GameObject.h"
#include "Button.h"
#include "Renderer.h"

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

	/// network
	void onConnect(SIOClient *client) override;
	void onMessage(SIOClient *client, const std::string &data) override;
	void onClose(SIOClient *client) override;
	void onError(SIOClient *client, const std::string &data) override;

	void createMap(float x, float y);

	void gameStart(const std::string &ip);

	void updatePosition(float dt);

	void emit(const std::string &event, const std::string &args);

public:
	int **mapData, mapWidth, mapHeight;
	cocos2d::Sprite ***mapFog;
	GameObject ***mapTile;

	Player *player;
	std::vector<Player*> otherPlayers;

	::Renderer *renderer;

	Button * interactButton;

	SIOClient *client;
	std::string uuid;

	cocos2d::Vec2 syncPosition, syncVelocity, otherDirection, otherPosition;
	float otherSpeed = 0;
	float time = 0, lastTime, delay = 0;
};

#endif // __HELLOWORLD_SCENE_H__

