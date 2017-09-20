#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Button.h"

class Player;

class HelloWorld : public cocos2d::Scene
{
public:
    ~HelloWorld();
    virtual bool init();
    
    void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	bool isSolidObject(int x, int y);
    
public:
    int **mapData, mapWidth, mapHeight;
    cocos2d::Sprite ***mapTile, ***mapObjects, ***mapFog;
    
    //int state = 0;
    
    Player *player;
	Button * interactButton;
};

#endif // __HELLOWORLD_SCENE_H__
