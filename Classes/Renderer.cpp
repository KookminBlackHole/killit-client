//
//  Renderer.cpp
//  Killit
//
//  Created by a on 2017. 11. 22..
//

#include "Renderer.h"

USING_NS_CC;
using namespace std;

::Renderer *::Renderer::create(const Size &size) {
    ::Renderer *ret = new (std::nothrow) ::Renderer();
    if (ret && ret->init(size)) {
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

::Renderer *::Renderer::create(int w, int h) { return ::Renderer::create(Size(w, h)); }
::Renderer *::Renderer::create() { return ::Renderer::create(Size::ZERO); }

::Renderer::~Renderer() {
    for (auto &i : nodes) i->release();
    nodes.clear();
    
    rt->release();
}

bool ::Renderer::init(Size size) {
    if (size.equals(Size::ZERO)) size = Director::getInstance()->getVisibleSize();
    
    rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    rt->retain();
    
    bool ret = initWithTexture(rt->getSprite()->getTexture());
    
    this->setAnchorPoint(Vec2(0, 0));
    this->setFlippedY(true);
    
    rt->setAutoDraw(false);
    
    scheduleUpdate();
    
    return ret;
}

void ::Renderer::update(float dt) {
    rt->beginWithClear(0, 0, 0, 0);
    
    for (auto &i : nodes) {
        i->visit();
    }
    
    rt->end();
    
    this->setTexture(rt->getSprite()->getTexture());
}

void ::Renderer::add(Node *node) {
    node->retain();
    nodes.push_back(node);
}

void ::Renderer::remove(Node *node) {
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if (it == nodes.end()) return;
    
    node->release();
    nodes.erase(it);
}
