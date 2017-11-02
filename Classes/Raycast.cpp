//
//  Raycast.cpp
//  Killit
//
//  Created by a on 2017. 10. 25..
//

#include "Raycast.h"

#include "Definitions.h"

USING_NS_CC;
using namespace std;

// 가장 작은 양수를 epsilon이라 하지만 너무 작은 수를 더하면 같은 좌표로 인식해서 무한 루프에 빠지므로 1로 설정해둠.
//const float Edge::EPSILON = 1;
//const float Edge::MIN = std::numeric_limits<float>().min();
//const float Edge::MAX = std::numeric_limits<float>().max();
//
//bool Edge::operator()(const Vec2 &p) const {
//    if (a.y > b.y) return Edge(b, a)(p);
//    if (p.y == a.y || p.y == b.y) return operator()(Vec2(p.x, p.y + EPSILON));
//    if (p.y > b.y || p.y < a.y || p.x > fmax(a.x, b.x)) return false;
//    if (p.x < fmin(a.x, b.x)) return true;
//    auto blue = abs(a.x - p.x) > MIN ? (p.y - a.y) / (p.x - a.x) : MAX;
//    auto red = abs(a.x - b.x) > MIN ? (b.y - a.y) / (b.x - a.x) : MAX;
//    return blue >= red;
//}
//
//Rectangle::Rectangle(const Vec2 &pos, const Size &size) {
//    Vec2 min = pos - size / 2, max = pos + size / 2;
//
//    auto e1 = Edge(min, { max.x, min.y });
//    auto e2 = Edge({ max.x, min.y }, max);
//    auto e3 = Edge(max, { min.x, max.y });
//    auto e4 = Edge({ min.x, max.y }, min);
//
//    edges.push_back(e1);
//    edges.push_back(e2);
//    edges.push_back(e3);
//    edges.push_back(e4);
//}
//
//bool Rectangle::contains(const Vec2 &p) const {
//    int c = 0;
//    for (auto e : edges) if (e(p)) c++;
//    return c % 2;
//}

//Vec2 raycast(const vector<Rectangle> &rects, const Vec2 &start, float angle, float length) {
//    auto ray = start - Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
//    bool escape = false;
//    for (int i = 0; i < length && !escape; i++) {
//        ray += Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
//        
//        for (auto r : rects) {
//            if (r.contains(ray)) { escape = true; break; }
//        }
//    }
//    
//    return ray;
//}

bool raycast(GameObject ***objects, const Vec2 &start, float angle, float length, Vec2 &contactPosition, GameObject *&contactObject) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    auto ray = start - Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
    for (int i = 0; i < length; i += 4) {
        ray += Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle)) * 4;
        
        int gX = (start.x + (TILE_SIZE_HALF * (64 - 1) - origin.x)) / TILE_SIZE + 1;
        int gY = (start.y + (TILE_SIZE_HALF * (64 - 1) - origin.y)) / TILE_SIZE + 1;
        int lim = floor(i / TILE_SIZE) + 1;
        for (int k = max(gY - lim, 0); k < min(gY + lim + 1, 64); k++) {
            for (int l = max(gX - lim, 0); l < min(gX + lim + 1, 64); l++) {
                // 솔리드 오브젝트이거나 상호작용 오브젝트인 경우에 충돌 감지
                if (objects[k][l]->isSolidObject() || (objects[k][l]->type >= 0 && objects[k][l]->type < 10)) {
                    if (objects[k][l]->getBoundingBox().containsPoint(ray)) {
                        contactObject = objects[k][l];
                        contactPosition = ray;
                        return true;
                    }
                }
            }
        }
    }
    
    contactPosition = ray;
    
    return false;
}

