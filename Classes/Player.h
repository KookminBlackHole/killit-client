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
	/// Ω√¿€ x¡¬«•, Ω√¿€ y¡¬«•
    static Player *create(int sx, int sy);
    
    bool init() override;
    void update(float dt) override;
    
    void onStickBegan(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickMoved(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickEnded(cocos2d::Vec2 angle, cocos2d::Ref *pSender);

	/// ¿Ã∏ß ∫Ø∞Ê « ø‰
	/// Ω«¡¶ ¡¬«•∏¶ ±◊∏ÆµÂ ¡¬«•∑Œ ∫Ø»Ø«œ¥¬ «‘ºˆ
	void calculateGridCoord(int mapWidth, int mapHeight);
	/// ±◊∏Æµµ ¡¬«•∏¶ Ω«¡¶ ¡¬«•ø° π›øµΩ√≈∞¥¬ «‘ºˆ
	void gridCoordUpdate(int mapWidth, int mapHeight);

	/// ∞ËªÍ Ω√¡°¿ª ∏Ì»Æ»˜ «ÿæﬂ«ÿº≠ µ˚∑Œ ∏∏µÁ «‘ºˆµÈ
	void move();
	void collision();
	void updateZOrder();
	void updatePosition();
	void checkGameObjects();
    /// 벽이 밑에 있는지 체크하고 있으면 투명도를 조절해주는 함수
    void checkSolidObjects();
    
public:
    cocos2d::Sprite *player;
    /// 벽과 충돌 할 떄 쓰는 바운딩박스
	cocos2d::Rect solidBB;
	/// angle: «√∑π¿ÃæÓ∞° ∫∏∞Ì ¿÷¥¬ ∞¢µµ, pos: «√∑π¿ÃæÓ ¿”Ω√ ¡¬«•
    cocos2d::Vec2 angle, tempPosition;

    bool touchJoystick = false;
    float speed = 3.0f;
	/// ≈∏¿œ∏ ø°º≠¿« ¡¬«•
	int gX = 0, gY = 0;
    
};

#endif /* Player_h */
