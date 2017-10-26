//
//  Raycast.h
//  Killit
//
//  Created by a on 2017. 10. 25..
//

#ifndef Raycast_h
#define Raycast_h

#include "cocos2d.h"
#include <limits>

//class Edge {
//public:
//    Edge(const cocos2d::Vec2 &a, const cocos2d::Vec2 &b) : a(a), b(b) {}
//    
//    friend class Rectangle;
//    
//private:
//    bool operator()(const cocos2d::Vec2 &p) const;
//    
//    cocos2d::Vec2 a, b;
//    
//    static const float EPSILON;
//    static const float MIN;
//    static const float MAX;
//};
//
//class Rectangle {
//public:
//    Rectangle(const cocos2d::Vec2 &pos, const cocos2d::Size &size);
//    
//    bool contains(const cocos2d::Vec2 &p) const;
//    
//private:
//    std::vector<Edge> edges;
//    
//};

// 폴리곤을 지원하는 레이캐스트
//cocos2d::Vec2 raycast(const std::vector<Rectangle> &rects, const cocos2d::Vec2 &start, float angle, float length);
// 사각형만 지원하는 레이캐스트
cocos2d::Vec2 raycast(const std::vector<cocos2d::Rect> &rects, const cocos2d::Vec2 &start, float angle, float length);

#endif /* Raycast_h */
