#ifndef UIManager_h
#define UIManager_h

#include "cocos2d.h"

#include "Joystick.h"
#include "ActionButton.h"
#include "Dial.h"

class UIManager : public cocos2d::Node {
public:
    static UIManager *create(cocos2d::Scene *parent);

    bool init(cocos2d::Scene *parent);

	void onTouchesBegan(const std::vector<cocos2d::Touch*> &, cocos2d::Event *);
	void onTouchesMoved(const std::vector<cocos2d::Touch*> &, cocos2d::Event *);
	void onTouchesEnded(const std::vector<cocos2d::Touch*> &, cocos2d::Event *);

public:
	cocos2d::EventListenerTouchAllAtOnce *listener;
    
    Joystick *joystick;
    ActionButton *actionButton;
    Dial *dial;
    
};

#endif
