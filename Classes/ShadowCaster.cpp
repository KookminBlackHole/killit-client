//
//  ShadowCaster.cpp
//  Killit
//
//  Created by a on 2017. 11. 23..
//

#include "ShadowCaster.h"

#include "GameObject.h"
#include "Utils.h"

USING_NS_CC;

ShadowCaster ShadowCaster::instance = ShadowCaster();

ShadowCaster *ShadowCaster::getInstance() {
    return &instance;
}

void ShadowCaster::compute(int x, int y, int radius, GameObject ***tile, cocos2d::Sprite ***fog) {
	for (int i = 0; i < 8; i++)
		computeOctant(x, y, radius, i, tile, fog);
}

void ShadowCaster::computeOctant(int pX, int pY, int radius, int octant, GameObject ***tile, cocos2d::Sprite ***fog) {
    std::queue<ColumnPortion> queue;
    
    queue.push(ColumnPortion(0, { 1, 0 }, { 1, 1 }));
    while (!queue.empty()) {
        auto cur = queue.front();
        queue.pop();
        
        if (cur.x > radius) continue;
        
        computeColumnPortion(pX, pY, radius, octant, cur, tile, fog, queue);
    }
}

void ShadowCaster::computeColumnPortion(int pX, int pY, int radius, int octant, ColumnPortion current, GameObject ***tile, cocos2d::Sprite ***fog, std::queue<ColumnPortion> &queue) {
	DirectionVector bottom = current.bottom, top = current.top;
	int x = current.x;

    int topY;
    if (x == 0) topY = 0;
    else {
        int quotient = (2 * x + 1) * top.y / (2 * top.x);
		int remainder = (2 * x + 1) * top.y % (2 * top.x);

		if (remainder > top.x) topY = quotient + 1;
		else topY = quotient;
    }

	int bottomY;
	if (x == 0) bottomY = 0;
	else {
		int quotient = (2 * x - 1) * bottom.y / (2 * bottom.x);
		int remainder = (2 * x - 1) * bottom.y % (2 * bottom.x);

		if (remainder >= bottom.x) bottomY = quotient + 1;
		else bottomY = quotient;
	}

	int wasLastCellOpaque = -1;
	for (int y = topY; y >= bottomY; --y) {
		bool inRadius = isInRadius(x, y, radius);
		if (inRadius) {
			/// setFoV
            int length = (2 * x - 1) * (2 * x - 1) + (2 * y - 1) * (2 * y - 1);
            length = length < 50 ? 0 : length;
			convertFogOctant(x, y, pX, pY, octant, fog)->setOpacity(length);
		}

		bool currentIsOpaque = !inRadius || convertObjectOctant(x, y, pX, pY, octant, tile)->isSolidObject();
		if (wasLastCellOpaque != -1) {
			if (currentIsOpaque) {
				if (wasLastCellOpaque == 0) {
					queue.push(ColumnPortion(x + 1, { x * 2 - 1, y * 2 + 1 }, top));
				}
			} else if (wasLastCellOpaque == 1) {
				top = { x * 2 + 1, y * 2 + 1 };
			}
		}

		wasLastCellOpaque = currentIsOpaque;
	}

	if (wasLastCellOpaque == 0) {
		queue.push(ColumnPortion(x + 1, bottom, top));
	}
}

bool ShadowCaster::isInRadius(int x, int y, int length) {
    return (2 * x - 1) * (2 * x - 1) + (2 * y - 1) * (2 * y - 1) <= 4 * length * length;
}

Sprite * ShadowCaster::convertFogOctant(int x, int y, int pX, int pY, int octant, Sprite ***fog) {
	Sprite *ret = nullptr;
	switch (octant) {
	case 0: ret = fog[clamp(y + pY, 0, 63)][clamp(x + pX, 0, 63)]; break;
	case 1: ret = fog[clamp(x + pY, 0, 63)][clamp(y + pX, 0, 63)]; break;
	case 2: ret = fog[clamp(x + pY, 0, 63)][clamp(-y + pX, 0, 63)]; break;
	case 3: ret = fog[clamp(y + pY, 0, 63)][clamp(-x + pX, 0, 63)]; break;
	case 4: ret = fog[clamp(-y + pY, 0, 63)][clamp(-x + pX, 0, 63)]; break;
	case 5: ret = fog[clamp(-x + pY, 0, 63)][clamp(-y + pX, 0, 63)]; break;
	case 6: ret = fog[clamp(-x + pY, 0, 63)][clamp(y + pX, 0, 63)]; break;
	case 7: ret = fog[clamp(-y + pY, 0, 63)][clamp(x + pX, 0, 63)]; break;
	}

	return ret;
}

GameObject * ShadowCaster::convertObjectOctant(int x, int y, int pX, int pY, int octant, GameObject ***tile) {
	GameObject *ret = nullptr;
	switch (octant) {
	case 0: ret = tile[clamp(y + pY, 0, 63)][clamp(x + pX, 0, 63)]; break;
	case 1: ret = tile[clamp(x + pY, 0, 63)][clamp(y + pX, 0, 63)]; break;
	case 2: ret = tile[clamp(x + pY, 0, 63)][clamp(-y + pX, 0, 63)]; break;
	case 3: ret = tile[clamp(y + pY, 0, 63)][clamp(-x + pX, 0, 63)]; break;
	case 4: ret = tile[clamp(-y + pY, 0, 63)][clamp(-x + pX, 0, 63)]; break;
	case 5: ret = tile[clamp(-x + pY, 0, 63)][clamp(-y + pX, 0, 63)]; break;
	case 6: ret = tile[clamp(-x + pY, 0, 63)][clamp(y + pX, 0, 63)]; break;
	case 7: ret = tile[clamp(-y + pY, 0, 63)][clamp(x + pX, 0, 63)]; break;
	}

	return ret;
}
