//
//  Player.h
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#ifndef Player_h
#define Player_h

#include "cocos2d.h"

class Player : public cocos2d::Node {
public:
	/// 시작 x좌표, 시작 y좌표
    static Player *create(int sx, int sy);
    
    bool init() override;
    void update(float dt) override;
    
    void onStickBegan(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickMoved(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickEnded(cocos2d::Vec2 angle, cocos2d::Ref *pSender);

	/// 이름 변경 필요
	/// 실제 좌표를 그리드 좌표로 변환하는 함수
	void calculateGridCoord(int mapWidth, int mapHeight);
	/// 그리도 좌표를 실제 좌표에 반영시키는 함수
	void gridCoordUpdate(int mapWidth, int mapHeight);

	/// 계산 시점을 명확히 해야해서 따로 만든 함수들
	void move();
	void collision();
	void updateZOrder();
	void updatePosition();
	void checkGameObjects();
    
public:
    cocos2d::Sprite *player;
	cocos2d::Size playerSize;
	/// angle: 플레이어가 보고 있는 각도, pos: 플레이어 임시 좌표
    cocos2d::Vec2 angle, tempPosition;

    bool touchJoystick = false;
    float speed = 4.5f;
	/// 타일맵에서의 좌표
	int gX = 0, gY = 0;
    
};

#endif /* Player_h */
