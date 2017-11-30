//
//  Joystick.h
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#ifndef Joystick_h
#define Joystick_h

#include "cocos2d.h"

class Player;

class Joystick : public cocos2d::Node {
public:
    static Joystick *create(const cocos2d::Vec2 &position);
    static Joystick *create(float x, float y);
    
    bool init() override;
    void bind(Player *player);
//    void update(float dt) override;
    
public:
    void onTouchBegan(const cocos2d::Vec2 &, int id);
    void onTouchMoved(const cocos2d::Vec2 &, int id);
    void onTouchEnded(const cocos2d::Vec2 &, int id);
    
public:
    cocos2d::Sprite *pad, *stick;
    /// 변수 이름 바꿔야 함..
    Player *player = nullptr;
    int id = -1;
    int way = 8;
    float distance = 360.0f / way;
    
    cocos2d::Vec2 direction, prevDirection;
    
};

#endif /* Joystick_h */
