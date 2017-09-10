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
    void update(float dt) override;
    
protected:
    void onTouchesBegan(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    
public:
    cocos2d::Sprite *pad, *stick;
    /// 변수 이름 바꿔야 함..
    Player *bnd = nullptr;
    cocos2d::EventListenerTouchAllAtOnce *listener;
    int touchId = -1;
    
    cocos2d::Vec2 pos;
    
};

#endif /* Joystick_h */
