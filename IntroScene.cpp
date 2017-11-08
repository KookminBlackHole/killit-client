#include "IntroScene.h"

#include "HelloWorldScene.h"

USING_NS_CC;

bool IntroScene::init() {
	LayerColor *background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = size / 2;

	NodeGrid *grid = NodeGrid::create();
	this->addChild(grid);

	Sprite *image = Sprite::create("res/logo.png");
	image->getTexture()->setAliasTexParameters();
	image->setPosition(origin);
	image->setScale(2);
	grid->addChild(image);

	grid->runAction(Sequence::create(Ripple3D::create(3.0f, Size(100, 100), origin, 350, 3, 100.0f), CallFunc::create([&] {

	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, HelloWorld::create()));

	}), NULL));

	return true;
}
