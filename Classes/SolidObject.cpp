//
//  SolidObject.cpp
//  Killit
//
//  Created by a on 2017. 11. 1..
//

#include "SolidObject.h"

SolidObject *SolidObject::create() {
    SolidObject *ret = new (std::nothrow) SolidObject();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
