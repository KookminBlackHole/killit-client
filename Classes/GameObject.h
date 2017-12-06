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

	/// 솔리드 오브젝트 설정 함수.
	void setSolidObject(bool solid);
	/// 충돌 영역 설정.
	void setSolidArea(const cocos2d::Rect &area);

	/// 솔리드 오브젝트 여부 반환 함수.
    bool isSolidObject() const;
	/// 상호작용 오브젝트 여부 반환 함수.
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
