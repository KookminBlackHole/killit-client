#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "network/SocketIO.h"

#include "Button.h"

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
    
public:
    int **mapData, mapWidth, mapHeight;
    cocos2d::Sprite ***mapTile, ***mapObjects, ***mapFog;
    
    Player *player;
	Button * interactButton;
    
    SIOClient *client;
};

#endif // __HELLOWORLD_SCENE_H__
