//
//  ShadowCaster.h
//  Killit
//
//  Created by a on 2017. 11. 23..
//

#ifndef ShadowCaster_h
#define ShadowCaster_h

#include "cocos2d.h"

#include <queue>

class GameObject;
class ColumnPortion;

class ShadowCaster {
public:
    static ShadowCaster *getInstance();
    
    void compute(int x, int y, int radius, GameObject ***tile, cocos2d::Sprite ***fog);
    
private:
    ShadowCaster() {}
    virtual ~ShadowCaster() {}
    
    void computeOctant(int pX, int pY, int radius, int octant, GameObject ***tile, cocos2d::Sprite ***fog);
    void computeColumnPortion(int pX, int pY, int radius, int octant, ColumnPortion current, GameObject ***tile, cocos2d::Sprite ***fog, std::queue<ColumnPortion> &queue);
    bool isInRadius(int x, int y, int length);
	cocos2d::Sprite *convertFogOctant(int x, int y, int pX, int pY, int octant, cocos2d::Sprite ***fog);
	GameObject *convertObjectOctant(int x, int y, int pX, int pY, int octant, GameObject ***tile);
    
    static ShadowCaster instance;
    
};

struct DirectionVector {
    int x, y;
};

class ColumnPortion {
public:
    ColumnPortion(int x, DirectionVector bottom, DirectionVector top) :
    x(x),
    bottom(bottom),
    top(top)
    {}
    
    int x;
    DirectionVector bottom, top;
};

#endif /* ShadowCaster_h */
