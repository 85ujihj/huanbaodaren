

#ifndef __mir9__GameOptions__
#define __mir9__GameOptions__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameOptions: public Node
{
    
    Sprite* m_pBg;
    
public:
    
    CREATE_FUNC(GameOptions);
    
    void onEnterTransitionDidFinish();
};

#endif /* defined(__mir9__GameOptions__) */
