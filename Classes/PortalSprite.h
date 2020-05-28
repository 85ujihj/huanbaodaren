

#ifndef __mir_mobile__PortalSprite__
#define __mir_mobile__PortalSprite__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
//传送
class PortalSprite: public Sprite 
{
    
    std::string m_path;
    
    Sprite* m_sprite;
    
    void playAnimate();
    
public:
    
    PortalSprite(const char* path);
    
    virtual ~PortalSprite();
    
    static PortalSprite* createWithPortalSprite(const char* fileName);
    
    bool init();
    
};

#endif /* defined(__mir_mobile__PortalSprite__) */
