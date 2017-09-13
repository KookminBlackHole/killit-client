#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Player;

class HelloWorld : public cocos2d::Scene
{
public:
    ~HelloWorld();
    virtual bool init();
    
    void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    int **mapData, mapWidth, mapHeight;
    cocos2d::Sprite ***mapTile, ***mapObjects, ***mapFog;
    
    int state = 0;
    
    Player *player;
    /// 맵 위에서의 플레이어 좌표
    int pX, pY;
};

#endif // __HELLOWORLD_SCENE_H__
