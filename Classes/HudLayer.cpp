//
//  HudLayer.cpp
//  guyue
//
//  Created by dsh on 15/1/18.
//
//

#include "HudLayer.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
using namespace cocos2d;

HudLayer::HudLayer(void)
{
    _dPad = NULL;
}

HudLayer::~HudLayer(void)
{
}

bool HudLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
        
        
        
        auto layer = LayerColor::create(Color4B(0xFF, 0x00, 0x00, 0x80),400,400);
        auto callback = [](Touch * ,Event *)
        {
            return false;
        };
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        layer->ignoreAnchorPointForPosition(false);
        layer->setPosition( Point(0, 0) );
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,layer);
        //_eventDispatcher->addEventListenerWithFixedPriority(listener,-1);
        this->addChild(layer);
        
        _dPad = SimpleDPad::dPadWithFile("ui/direction_head.png", "ui/direction_tray.png", Point(100.0, 100.0));
        _dPad->setOpacity(100);
        _dPad->startRocker(true);
        this->addChild(_dPad);

        
        bRet = true;
    } while (0);
    
    return bRet;
}
