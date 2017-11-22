//
//  GameObject.cpp
//  Killit
//
//  Created by a on 2017. 11. 1..
//

#include "Object.h"

USING_NS_CC;
using namespace std;

KIT_BEGIN

Object *Object::create(const string &imagePath, int type) {
    Object *ret = new (std::nothrow) Object();
    if (ret && ret->init(imagePath, type)) {
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

Object *Object::create(SpriteFrame *sf, int type) {
    Object *ret = new (std::nothrow) Object();
	if (ret && ret->initWithTexture(sf, type)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool Object::init(const string &imagePath, int type) {
	setScale(2);
    setCascadeOpacityEnabled(true);

    if (imagePath != "") {
        image = Sprite::create(imagePath);
		image->getTexture()->setAliasTexParameters();
        this->addChild(image);
    }
    
    this->type = type;

	return true;
}

bool Object::initWithTexture(SpriteFrame *sf, int type) {
	setScale(2);
    setCascadeOpacityEnabled(true);

	image = Sprite::createWithSpriteFrame(sf);
	image->getTexture()->setAliasTexParameters();
	this->addChild(image);

	this->type = type;

	return true;
}

void Object::setSolidObject(bool solid) {
	isSolid = solid;
}

void Object::setSolidArea(const Rect &area) {
	collisionArea = area;
}

bool Object::isSolidObject() const {
    return isSolid;
}

void Object::setZOrder(int zorder) {
	this->setGlobalZOrder(zorder);
	for (auto &i : getChildren()) i->setGlobalZOrder(zorder);
}

Rect Object::getBoundingBox() const {
	return Rect(getPosition() + collisionArea.origin, collisionArea.size);
}

KIT_END
