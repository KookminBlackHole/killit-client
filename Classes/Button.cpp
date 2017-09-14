//
//  Button.cpp
//  Killit
//
//  Created by a on 2017. 9. 13..
//
//

#include "Button.h"

USING_NS_CC;
using namespace std;

Button *Button::create(const string &filename) {
    Button *ret = new (std::nothrow) Button();
    if (ret && ret->initWithFile(filename))
    {
        ret->autorelease();
		ret->initialize();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Button::initialize() {
	//auto listener = EventListenerTouchAllAtOnce::create();
	//listener->onTouchesBegan = [&](const vector<Touch *> &t, Event *e) {
	//	//MessageBoxA(0, 0, 0, 0);c
	//	callback();
	//	/*if (getBoundingBox().containsPoint(t.front()->getLocation())) {

	//	}*/
	//};
	//
	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Button::onTouchBegan(std::function<void()> callback) {
	this->callback = callback;
}