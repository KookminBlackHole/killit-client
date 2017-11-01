//
//  GameObject.cpp
//  Killit
//
//  Created by a on 2017. 11. 1..
//

#include "GameObject.h"

USING_NS_CC;
using namespace std;

GameObject *GameObject::create(const string &imagePath, int type) {
    GameObject *ret = new (std::nothrow) GameObject();
    if (ret && ret->init(imagePath, type)) {
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

GameObject *GameObject::create(SpriteFrame *sf, int type) {
	GameObject *ret = new (std::nothrow) GameObject();
	if (ret && ret->initWithTexture(sf, type)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool GameObject::init(const string &imagePath, int type) {
	setScale(2);

    if (imagePath != "") {
        image = Sprite::create(imagePath);
		image->getTexture()->setAliasTexParameters();
        this->addChild(image);
    }
    
    this->type = type;

	return true;
}

bool GameObject::initWithTexture(SpriteFrame *sf, int type) {
	setScale(2);

	image = Sprite::createWithSpriteFrame(sf);
	image->getTexture()->setAliasTexParameters();
	this->addChild(image);

	this->type = type;

	return true;
}

void GameObject::setSolidObject(bool solid) {
	isSolid = solid;
}


void GameObject::setSolidArea(const Rect &area) {
	collisionArea = area;
}

void GameObject::setZOrder(int zorder) {
	this->setGlobalZOrder(zorder);
	for (auto &i : getChildren()) i->setGlobalZOrder(zorder);
}

Rect GameObject::getBoundingBox() const {
	return Rect(getPosition() + collisionArea.origin, collisionArea.size);
}