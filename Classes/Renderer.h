//
//  Renderer.h
//  Killit
//
//  Created by a on 2017. 11. 22..
//

#ifndef Renderer_h
#define Renderer_h

#include "cocos2d.h"

#include <vector>
#include <functional>

class Renderer : public cocos2d::Sprite {
public:
    static Renderer *create(const cocos2d::Size &size);
    static Renderer *create(int w, int h);
    static Renderer *create();
    
    virtual ~Renderer();
    
    bool init(cocos2d::Size size);
    
    void update(float dt) override;
    
    void add(cocos2d::Node *node);
    void remove(cocos2d::Node *node);
    
    void setFunction(const std::function<void()> &drawFunction);
    
protected:
    cocos2d::RenderTexture *rt;
    std::vector<cocos2d::Node *> nodes;
    std::function<void()> drawFunction;
    
};

#endif /* Renderer_h */
