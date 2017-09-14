//
//  Button.h
//  Killit
//
//  Created by a on 2017. 9. 13..
//
//

#ifndef Button_h
#define Button_h

#include "cocos2d.h"

class Button : public cocos2d::Sprite {
public:
    static Button *create(const std::string &filename);
    
	void initialize();

public:
	void onTouchBegan(std::function<void()>);

	std::function<void()> callback;
    
};

#endif /* Button_h */
