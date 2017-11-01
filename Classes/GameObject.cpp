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

bool GameObject::init(const string &imagePath, int type) {
    if (imagePath != "") {
        image = Sprite::create(imagePath);
        this->addChild(image);
    }
    
    this->type = type;
}
