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
	// sx, sy: 플레이어의 타일맵 좌표, player: 현재 플레이어 구분 
    static Player *create(int sx, int sy, bool owner);
    
    bool init() override;
    void update(float dt) override;
    
    void onStickBegan(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    void onStickMoved(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    void onStickEnded(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    
    void onStickChanged(const cocos2d::Vec2 &angle, cocos2d::Ref *pSender);
    
    void onAngleChanged(int idx);

	/// ¿Ã∏ß ∫Ø∞Ê « ø‰
	/// Ω«¡¶ ¡¬«•∏¶ ±◊∏ÆµÂ ¡¬«•∑Œ ∫Ø»Ø«œ¥¬ «‘ºˆ
	void calculateGridCoord(int mapWidth, int mapHeight);
	/// ±◊∏Æµµ ¡¬«•∏¶ Ω«¡¶ ¡¬«•ø° π›øµΩ√≈∞¥¬ «‘ºˆ
	void gridCoordUpdate(int mapWidth, int mapHeight);

	/// ∞ËªÍ Ω√¡°¿ª ∏Ì»Æ»˜ «ÿæﬂ«ÿº≠ µ˚∑Œ ∏∏µÁ «‘ºˆµÈ
	void move(float dt);
	void collision();
	void updateZOrder();
	void updatePosition();
	/// 사용되고 있지 않는 함수.
	//bool checkGameObjects();
    /// 벽이 밑에 있는지 체크하고 있으면 투명도를 조절해주는 함수
    void checkSolidObjects();
    
    void attack();
    
    bool raycast(GameObject ***objects, float angle, float length, cocos2d::Vec2 *out);
    
public:
    cocos2d::Sprite *player;
    /// 벽과 충돌 할 떄 쓰는 바운딩박스
	cocos2d::Rect solidBB;
    cocos2d::Vec2 direction, tempPosition;
    
    cocos2d::DrawNode *debugHP, *debugAttack, *debugAngle;

    bool touchJoystick = false, owner = false;
    float speed = 3.0f, angle = 0;
    
	int gX = 0, gY = 0, idx = 0;// , prevIdx = 0;
    
    int hp = 100;
    
};

#endif /* Player_h */
