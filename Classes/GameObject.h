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
	static GameObject *create(cocos2d::SpriteFrame *sf, int type);
    
    bool init(const std::string &imagePath, int type);
	bool initWithTexture(cocos2d::SpriteFrame *sf, int type);

	void setSolidObject(bool solid);
	void setSolidArea(const cocos2d::Rect &area);

    bool isSolidObject() const;
	bool isInteractionObject() const;
    
	void setZOrder(int zorder);

	cocos2d::Rect getBoundingBox() const;
    
public:
    cocos2d::Sprite *image;
    cocos2d::Rect collisionArea;
    
    int type = -1;
	bool isSolid = false;
    
};

#endif /* GameObject_h */
