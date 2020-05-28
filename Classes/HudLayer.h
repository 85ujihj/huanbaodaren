//
//  HudLayer.h
//  guyue
//
//  Created by dsh on 15/1/18.
//
//

#ifndef __guyue__HudLayer__
#define __guyue__HudLayer__

#include "cocos2d.h"
#include "SimpleDPad.h"


class HudLayer : public cocos2d::Layer
{
public:
    HudLayer(void);
    ~HudLayer(void);
    
    bool init();
    CREATE_FUNC(HudLayer);
    CC_SYNTHESIZE(SimpleDPad*, _dPad, DPad);
    
};

#endif /* defined(__guyue__HudLayer__) */
