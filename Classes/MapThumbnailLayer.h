

#ifndef __mir9__MapThumbnailLayer__
#define __mir9__MapThumbnailLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
//#include "ui/CocosGUI.h"
//#include "cocostudio/CocoStudio.h"
//#include "CCScrollView.h"
USING_NS_CC;
USING_NS_CC_EXT;
//using namespace ui;
using namespace std;

//,public CCTargetedTouchDelegate

class MapThumbnailLayer;
class MapThumbnailMenu:public Sprite
{

public:
    
    MapThumbnailMenu();
    
    ~MapThumbnailMenu();
    
    static MapThumbnailMenu* create();
    
    bool initWithFile(const char *pszFilename);
    
    virtual void onEnter();
    
    virtual void onExit();
    
protected:
    
    Sprite* m_playerIndicator;
    
    CCDictionary* m_enemyDic;
    
    vector<int> keyVec;
    
    CCLabelTTF* m_pCoordinateTTF;
    
protected:
    
    void update(float delay);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
};

class MapThumbnailScrollView;
class MapThumbnailLayer:public Layer
{
    
public:
    
    CREATE_FUNC(MapThumbnailLayer);
    
    bool init();
    
protected:
    
    MapThumbnailScrollView* m_nMap;
    
    //void registerWithTouchDispatcher(void);
    void removeSelf(Ref* obj, Control::EventType);
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
};

class MapThumbnailScrollView:public extension::ScrollView
{
    
    bool m_bIsMoved;
    
    Sprite* m_playerIndicator;
    
    Sprite* m_pEndPoint;
    
    void update(float delay);
    
    cocos2d::Vec2 m_beginPoint;
    
public:
    
    MapThumbnailScrollView();
    
    ~MapThumbnailScrollView();
    
    static MapThumbnailScrollView* create();
    
    void initWithMap();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
};

#endif /* defined(__mir9__MapThumbnailLayer__) */
