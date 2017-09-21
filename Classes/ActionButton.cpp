//
//  ActionButton.c
//  Killit
//
//  Created by a on 2017. 9. 21..
//
//

#include "ActionButton.h"

#include "ZOrder.h"

ActionButton *ActionButton::create(Player *player) {
    ActionButton *ret = new (std::nothrow) ActionButton();
    if (ret && ret->initWithFile("res/interaction.png")) {
        ret->autorelease();
        ret->player = player;
		ret->initialize();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ActionButton::initialize() {
    this->setScale(2);
    this->setOpacity(255 * 0.25f);
    this->getTexture()->setAliasTexParameters();
    this->setGlobalZOrder(ZORDER::UI);
}

void ActionButton::onPressed() {
    player->checkGameObjects();
}

void ActionButton::onReleased() {
    
}
