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

#include "GameObject.h"

class Player : public cocos2d::Node {
public:
	/// sx, sy: 플레이어의 타일맵 좌표, owner: 내 캐릭터 구분 
    static Player *create(int sx, int sy, bool owner);
    
    bool init() override;
    void update(float dt) override;
    
	/// 조이스틱 관련 콜백 함수.
    void onStickBegan(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    void onStickMoved(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    void onStickEnded(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    void onStickChanged(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    
    //void onAngleChanged(int idx);

	/// 캐릭터의 실제 좌표를 타일맵 좌표로 변환하는 함수.
	void calculateGridCoord();
	/// 캐릭터의 타일맵 좌표를 실제 좌표로 변경하는 함수(게임 초기 세팅에만 쓰임).
	void gridCoordUpdate();

	void move(float dt);
	void collision();
	void updateZOrder();
	/// 플레이어의 위치를 최종적으로 반영하는 함수.
	void updatePosition();
	/// 사용되고 있지 않는 함수.
	//bool checkGameObjects();
    /// 벽이 밑에 있는지 체크하고 있으면 투명도를 조절해주는 함수
    void checkSolidObjects();
    
    void attack();
    
    bool raycast(GameObject ***objects, float angle, float length, cocos2d::Vec2 *out);
    
public:
    cocos2d::Sprite *player;
    /// 벽과 충돌 할 때 쓰는 바운딩박스
	cocos2d::Rect solidBB;
	/// tempPosition을 쓰는 이유는 플레이어의 위치를 모든(이동, 충돌 등) 처리가 끝난 뒤 실제 적용 하기 위해서.
    cocos2d::Vec2 direction, tempPosition;
    
	/// 디버그 용 drawnode
    cocos2d::DrawNode *debugHP, *debugAttack, *debugAngle;

    bool touchJoystick = false, owner = false;
    float speed = 3.0f, angle = 0;
    
	int gX = 0, gY = 0, idx = 0;// , prevIdx = 0;
    
    int hp = 100;
    
};

#endif /* Player_h */
