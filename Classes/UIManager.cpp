#include "UIManager.h"

#include "CameraUtil.h"
#include "GameScene.h"
#include "ZOrder.h"

USING_NS_CC;
using namespace std;

UIManager *UIManager::create(Scene *parent) {
	UIManager *ret = new (std::nothrow) UIManager();
	if (ret && ret->init(parent)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool UIManager::init(Scene *parent) {
	auto parentScene = (GameScene *)parent;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UIManager::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UIManager::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UIManager::onTouchesEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	joystick = Joystick::create(Vec2::ZERO);
	joystick->setPosition(joystick->pad->getContentSize().width + 36, joystick->pad->getContentSize().height + 36);
	joystick->bind(parentScene->player);
	this->addChild(joystick);

	actionButton = ActionButton::create(parentScene->player);
	actionButton->setPosition(Vec2(visibleSize.width - actionButton->getContentSize().width - 36, actionButton->getContentSize().height + 36));
	this->addChild(actionButton);

	dial = Dial::create();
	dial->bind(parentScene->player);
	this->addChild(dial);

	return true;
}

void UIManager::onTouchesBegan(const vector<Touch*> &touches, Event *) {
	for (auto &t : touches) {
		Vec2 pos = t->getLocation() + CameraUtil::getInstance()->getPosition();
		int id = t->getID();

		if (joystick->id == -1) {
			auto bb = joystick->pad->getBoundingBox();
			auto cp = joystick->convertToNodeSpace(pos);
			if (Rect(bb.origin, bb.size + Size(20, 20)).containsPoint(cp)) {
				joystick->onTouchBegan(cp, id);
			}
		}
		if (actionButton->id == -1) {
			auto bb = actionButton->getBoundingBox();
			auto cp = this->convertToNodeSpace(pos);
			if (Rect(bb.origin, bb.size + Size(20, 20)).containsPoint(cp)) {
				actionButton->onTouchBegan(cp, id);
			}
		}
		if (dial->id == -1 &&
			joystick->id != id &&
			actionButton->id != id) {

			auto cp = dial->convertToNodeSpace(pos);
			dial->onTouchBegan(cp, id);
		}
	}
}

void UIManager::onTouchesMoved(const vector<Touch*> &touches, Event *) {
	for (auto &t : touches) {
		Vec2 pos = t->getLocation() + CameraUtil::getInstance()->getPosition();
		int id = t->getID();

		if (joystick->id == id) {
			auto cp = joystick->convertToNodeSpace(pos);
			joystick->onTouchMoved(cp, id);
		}
		if (dial->id == id) {
			auto cp = dial->convertToNodeSpace(pos);
			dial->onTouchMoved(cp, id);
		}
	}
}

void UIManager::onTouchesEnded(const vector<Touch*> &touches, Event *) {
	for (auto &t : touches) {
		Vec2 pos = t->getLocation() + CameraUtil::getInstance()->getPosition();
		int id = t->getID();

		if (joystick->id == id) {
			auto cp = joystick->convertToNodeSpace(pos);
			joystick->onTouchEnded(cp, id);
		}
		if (actionButton->id == id) {
			auto cp = this->convertToNodeSpace(pos);
			actionButton->onTouchEnded(cp, id);
		}
		if (dial->id == id) {
			auto cp = dial->convertToNodeSpace(pos);
			dial->onTouchEnded(cp, id);
		}
	}
}

