#ifndef _MAINMENU_MENU_H_
#define _MAINMENU_MENU_H_

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	bool init();

	CREATE_FUNC(MainMenu);
};

#endif

