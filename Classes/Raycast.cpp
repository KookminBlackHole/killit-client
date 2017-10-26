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

Vec2 raycast(Rect ***rects, const Vec2 &start, float angle, float length) {
    Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    auto ray = start - Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
    bool escape = false;
    for (int i = 0; i < length && !escape; i += 4) {
        ray += Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle)) * 4;
        
        // 맵 가로 세로 길이에 종속적인 문제 나중에 해결해야함. 아니면 아예 플레이어 클래스에 종속 시키는 방법도 있을듯.
        // 현재 문의 상태를 알 수 없어서 문에 무조건 충돌함.
        int gX = (start.x + (TILE_SIZE_HALF * (64 - 1) - origin.x)) / TILE_SIZE + 1;
        int gY = (start.y + (TILE_SIZE_HALF * (64 - 1) - origin.y)) / TILE_SIZE + 1;
        int lim = floor(i / TILE_SIZE) + 1;
        for (int k = max(gY - lim, 0); k < min(gY + lim + 1, 64) && !escape; k++) {
            for (int l = max(gX - lim, 0); l < min(gX + lim + 1, 64) && !escape; l++) {
                if (rects[k][l] != nullptr) {
                    if (rects[k][l]->containsPoint(ray)) escape = true;
                }
            }
        }
    }
    
    return ray;
}
