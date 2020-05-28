

#ifndef __mir_mobile__BgMapFloorTile__
#define __mir_mobile__BgMapFloorTile__

#include <iostream>
#include "cocos2d.h"

#define OFF_SIZE Vec2(128, 128)

USING_NS_CC;
class BgMapFloorTile: public cocos2d::Node {
    
protected:
    
    bool m_nIsDisplay;

    Sprite* m_sprite;
    
public:
    
    BgMapFloorTile();
    
    virtual ~BgMapFloorTile();
    
    CREATE_FUNC(BgMapFloorTile);

    CC_SYNTHESIZE_RETAIN(cocos2d::CCString*, m_fileName, FloorTileFileName);
    
    void IntelligentDisplay(cocos2d::Rect& showRect, cocos2d::Rect& hideRect);
    
protected:
    
    void displayImageView();
    
    void hideImageView();
    
    void initWithImageView(Texture2D* texture);
    
};


#endif /* defined(__mir_mobile__BgMapFloorTile__) */
