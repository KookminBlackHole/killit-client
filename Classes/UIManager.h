#ifndef UIManager_h
#define UIManager_h

#include "cocos2d.h"

class UIManager : public cocos2d::Node {
public:
	static UIManager *create();

	bool init() override;

	void onTouchesBegan(const std::vector<cocos2d::Touch*> &, cocos2d::Event *);
	void onTouchesMoved(const std::vector<cocos2d::Touch*> &, cocos2d::Event *);
	void onTouchesEnded(const std::vector<cocos2d::Touch*> &, cocos2d::Event *);

public:
	cocos2d::EventListenerTouchAllAtOnce *listener;
};

#endif