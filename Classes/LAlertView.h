

#ifndef __mir9__LAlertView__
#define __mir9__LAlertView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
typedef void (Ref::*SEL_Alert)(const int number);
#define Alert_selector(_SELECTOR) (SEL_Alert)(&_SELECTOR)
//, public CCTargetedTouchDelegate
class LAlertView: public Scale9Sprite{
    
    //CCArray* m_btnArray;
    CC_SYNTHESIZE_RETAIN(CCArray*, m_btnArray, BtnArray);
    
    Ref* m_target;
    
    SEL_Alert m_callBack;
    
    const char* m_title;

    const char* m_messageText;
    
public:
    
    LAlertView(const char* title, const char* messageText);
    
    virtual ~LAlertView();
    
    static LAlertView* create(const char* title, const char* messageText);
    
    bool init();
    
    void addButtonWithTitle(const char* btnTitle);
    
    void show(Ref* target, SEL_Alert selector);

private:
    
    void onEnter();
    
    void onExit();
    
    void touchUpInSide(Ref* sender, Control::EventType controlEvent);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
};

#endif /* defined(__mir9__LAlertView__) */
