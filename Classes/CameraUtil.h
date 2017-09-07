//
//  CameraUtil.hpp
//  Battle
//
//  Created by a on 2017. 9. 7..
//
//

#ifndef CameraUtil_h
#define CameraUtil_h

#include "cocos2d.h"

class CameraUtil : public cocos2d::Node {
private:
    CameraUtil() {};
    ~CameraUtil();
    
    static CameraUtil *instance;
    
public:
    static CameraUtil *getInstance();
    
public:
    cocos2d::Layer *fixedLayer;
    cocos2d::Scene *nowScene;
    
    cocos2d::Vec2 pos;
    
public:
    void initialize(cocos2d::Scene *now);
    
    void addUIChild(cocos2d::Node *child);
    
    void setPosition(float x, float y);
    void setPosition(const cocos2d::Vec2 &pos);
    void setRelPosition(float x, float y);
    void setRelPosition(const cocos2d::Vec2 &pos);
    
    const cocos2d::Vec2 &getPosition() const;
    
    void update(float dt) override;
    
};
#endif /* CameraUtil_h */
