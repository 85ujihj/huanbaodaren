

#ifndef __mir9__PropColumn__
#define __mir9__PropColumn__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "PropIconShow.h"
USING_NS_CC;
USING_NS_CC_EXT;

class PropColumnMenu: public Layer {
    
    LayerColor* m_propColumn;
    
    PropIconShow** m_propVec;
    
    PropIconShow* m_editProp;

public:
    
    PropColumnMenu();
    
    ~PropColumnMenu();
    
    CREATE_FUNC(PropColumnMenu);
    
    bool init();
    
    void onEnterTransitionDidFinish();
    
protected:
    
   // void registerWithTouchDispatcher(void);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    
	void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void updatePropVecPoint();
    
    cocos2d::Rect getPropRect(int i);
    
    cocos2d::Vec2 getPropPoint(int i);
    
    void propHoming(int i);
    
    void swapProp(int a, int b);
};

#endif /* defined(__mir9__PropColumn__) */
