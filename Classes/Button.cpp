//
//  Button.cpp
//  Killit
//
//  Created by a on 2017. 9. 13..
//
//

#include "Button.h"

USING_NS_CC;
using namespace std;

Button *Button::create(const string &filename) {
    Button *ret = new (std::nothrow) Button();
    if (ret && ret->initWithFile(filename))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
