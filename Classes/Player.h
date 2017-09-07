//
//  Player.h
//  Battle
//
//  Created by a on 2017. 9. 4..
//
//

#ifndef Player_h
#define Player_h

#include "cocos2d.h"

class Player : public cocos2d::Node {
public:
    static Player *create();
    
    bool init() override;
    void update(float dt) override;
    
    void onStickBegan(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickMoved(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickEnded(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    
public:
    cocos2d::Sprite *player, *arrow;
    cocos2d::Vec2 angle;
    bool touchJoystick = false;
    
    float speed = 2;
    
};

#endif /* Player_h */
