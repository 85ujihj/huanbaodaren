//
//  GameLoading.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#ifndef __guyue__GameLoading__
#define __guyue__GameLoading__

#include "cocos2d.h"

USING_NS_CC;
//, public CCTargetedTouchDelegate
class GameLoading: public Sprite{
    
    int m_mapID;
    
    int m_born;
    
    Sprite* m_sprite;
    
public:
    
    GameLoading(const int mapID, const int born);
    
    virtual ~GameLoading();
    
    static GameLoading* runGameLoading(int mapID, const int born);
    
    void setOpacity(GLubyte opacity);
    
   
    
   
    
protected:
    
    void onEnter();
//    
//    void onExit();
    
     bool init();
    
    void initGameScene(float delay = 0);
    
    void updateBgMap(float delay = 0);
    
    void updateFinish();
    
   
     //bool onTouchBegan(Touch *touch, Event *unused_event);
};


#endif /* defined(__guyue__GameLoading__) */
