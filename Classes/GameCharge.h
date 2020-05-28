//
//  GameCharge.hpp
//  GuangTouQiang
//
//  Created by Thunder on 2019/1/7.
//

#ifndef GameCharge_hpp
#define GameCharge_hpp

#include <stdio.h>
#include "cocos2d.h"

//zsd_publish_class_GameCharge

class GameCharge : public cocos2d::Layer
{
    //zsd_publish_functions
public:
    virtual bool init();
    //zsd_publish_functions
    CREATE_FUNC(GameCharge);
    
public:
    static int m_chargeId;
private:
    //zsd_publish_functions
};

#endif /* GameCharge_hpp */
