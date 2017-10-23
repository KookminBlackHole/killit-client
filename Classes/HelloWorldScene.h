#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "network/SocketIO.h"

#include "Button.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d::network;

class Player;

class HelloWorld : public cocos2d::Scene, public SocketIO::SIODelegate
{
public:
    ~HelloWorld();
    virtual bool init();
    
    void update(float dt);
    
    CREATE_FUNC(HelloWorld);

	bool isSolidObject(int x, int y);
    
    /// network
    void onConnect(SIOClient *client) override;
    void onMessage(SIOClient *client, const std::string &data) override;
    void onClose(SIOClient *client) override;
    void onError(SIOClient *client, const std::string &data) override;

	void createGame(float x, float y);
    
    void updatePosition(float dt);
    
public:
    int **mapData, mapWidth, mapHeight;
    cocos2d::Sprite ***mapTile, ***mapObjects, ***mapFog;
    
    Player *player;
    std::vector<Player*> otherPlayers;
    
	Button * interactButton;
    
    SIOClient *client;
    
    cocos2d::Vec2 syncPosition, syncVelocity;
    float time = 0, lastTime, delay = 0;
};

#endif // __HELLOWORLD_SCENE_H__
