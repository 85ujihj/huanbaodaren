

#ifndef __mir9__CCTouchMouse__
#define __mir9__CCTouchMouse__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class CCTouchMouse: public Sprite {
    
    //CCArray* m_effectArray;
    Vector<SpriteFrame*>m_effectArray;
    void setHide();
    
public:
    
    CCTouchMouse();
    
    virtual ~CCTouchMouse();
    
    CREATE_FUNC(CCTouchMouse);
    
    void playEffect(const cocos2d::Vec2& point);
    
};

#endif /* defined(__mir9__CCTouchMouse__) */
