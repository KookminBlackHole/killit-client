//
//  Button.h
//  Killit
//
//  Created by a on 2017. 15. 12..
//
//

#ifndef Button_h
#define Button_h

#include "cocos2d.h"

class Button : public cocos2d::Sprite {
public:
    static Button *create(const std::string &filename);
    
	void initialize();
    
//    virtual void onPressed() = 0;
//    virtual void onReleased() = 0;

public:
    void onTouchBegan(const cocos2d::Vec2 &position, int id);
    void onTouchEnded(const cocos2d::Vec2 &position, int id);
    
    int id = -1;
    
};

#endif /* Button_h */
