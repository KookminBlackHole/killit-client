#include "UIManager.h"

USING_NS_CC;
using namespace std;

UIManager *UIManager::create() {
	UIManager *ret = new (std::nothrow) UIManager();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool UIManager::init() {
	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UIManager::onTouchesBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void UIManager::onTouchesBegan(const vector<Touch*>&, Event *) {
}

void UIManager::onTouchesMoved(const vector<Touch*>&, Event *) {
}

void UIManager::onTouchesEnded(const vector<Touch*>&, Event *) {
}