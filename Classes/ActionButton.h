//
//  ActionButton.h
//  Killit
//
//  Created by a on 2017. 9. 21..
//
//

#ifndef ActionButton_h
#define ActionButton_h

#include "Button.h"
#include "Player.h"

class ActionButton : public Button {
public:
    static ActionButton *create(Player *player);
    
    void initialize() override;
    
public:
    void onPressed() override;
    void onReleased() override;
    
    Player *player;
    
};

#endif /* ActionButton_h */
