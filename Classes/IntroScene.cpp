#include "IntroScene.h"

#include "GameScene.h"

USING_NS_CC;

bool IntroScene::init() {
	LayerColor *background = LayerColor::create(Color4B::BLACK);
	this->addChild(background);

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = size / 2;

	NodeGrid *grid = NodeGrid::create();
	this->addChild(grid);

	Sprite *image = Sprite::create("res/logo_black.png");
	image->getTexture()->setAliasTexParameters();
	image->setPosition(origin);
	image->setScale(2);
	grid->addChild(image);

	grid->runAction(Sequence::create(Ripple3D::create(3.0f, Size(100, 100), origin, 220, 3, 100.0f), CallFunc::create([&] {

	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, GameScene::create()));

	}), NULL));

	return true;
}
