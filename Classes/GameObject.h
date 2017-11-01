//
//  GameObject.h
//  Killit
//
//  Created by a on 2017. 11. 1..
//

#ifndef GameObject_h
#define GameObject_h

#include "cocos2d.h"

class GameObject : public cocos2d::Node {
public:
    static GameObject *create(const std::string &imagePath, int type);
    
    bool init(const std::string &imagePath, int type);
    
public:
    cocos2d::Sprite *image;
    cocos2d::Rect collisionArea;
    
    int type = -1;
    
};

#endif /* GameObject_h */
