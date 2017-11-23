//
//  ShadowCaster.cpp
//  Killit
//
//  Created by a on 2017. 11. 23..
//

#include "ShadowCaster.h"

#include "GameObject.h"

USING_NS_CC;

ShadowCaster ShadowCaster::instance = ShadowCaster();

ShadowCaster *ShadowCaster::getInstance() {
    return &instance;
}

void ShadowCaster::compute(int x, int y, int radius, GameObject ***tile, cocos2d::Sprite ***fog) {
    computeOctant(radius, 0, tile, fog);
}

void ShadowCaster::computeOctant(int radius, int octant, GameObject ***tile, cocos2d::Sprite ***fog) {
    std::queue<ColumnPortion> queue;
    
    queue.push(ColumnPortion(0, { 1, 0 }, { 1, 1 }));
    while (!queue.empty()) {
        auto cur = queue.front();
        queue.pop();
        
        if (cur.x > radius) continue;
        
        computeColumnPortion(radius, cur, tile, fog, queue);
    }
}

void ShadowCaster::computeColumnPortion(int radius, ColumnPortion current, GameObject ***tile, cocos2d::Sprite ***fog, std::queue<ColumnPortion> queue) {
    int topY;
    if (current.x == 0) topY = 0;
    else {
//        int quotient = ();
    }
}

bool ShadowCaster::isInRadius(int x, int y, int length) {
    return (2 * x - 1) * (2 * x - 1) + (2 * y - 1) * (2 * y - 1) <= 4 * length * length;
}
