//
//  Dial.h
//  Killit
//
//  Created by a on 2017. 10. 27..
//

#ifndef Dial_h
#define Dial_h

#include "cocos2d.h"

class Player;

class Dial : public cocos2d::Node {
public:
    static Dial *create();
    
    bool init() override;
    void bind(Player *player);
    
public:
    void onTouchBegan(const cocos2d::Vec2 &, int id);
    void onTouchMoved(const cocos2d::Vec2 &, int id);
    void onTouchEnded(const cocos2d::Vec2 &, int id);
    
public:
    /// 변수 이름 바꿔야 함..
    Player *bnd = nullptr;
    int id = -1;
    
};

#endif /* Dial_h */
