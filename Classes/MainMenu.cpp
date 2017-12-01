#include "MainMenu.h"

#include "HelloWorldScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);

	return scene;
}

bool MainMenu::init(){
	if (!Layer::init()){ 
		return false;
	}
}


