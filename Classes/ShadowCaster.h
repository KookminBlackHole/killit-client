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
    
    void computeOctant(int radius, int octant, GameObject ***tile, cocos2d::Sprite ***fog);
    void computeColumnPortion(int radius, ColumnPortion current, GameObject ***tile, cocos2d::Sprite ***fog, std::queue<ColumnPortion> queue);
    bool isInRadius(int x, int y, int length);
    
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
