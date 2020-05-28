

#ifndef __mir_mobile__LTextField__
#define __mir_mobile__LTextField__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

NS_CC_BEGIN
//, public CCTargetedTouchDelegate
class LyfTextFieldTTF: public TextFieldTTF {
    
public:
    
	LyfTextFieldTTF();
    
	virtual ~LyfTextFieldTTF();
    
	static LyfTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const Size& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
	static LyfTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
	virtual void onEnter();
    
	virtual void onExit();
    
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
	/**
	 @brief    Open keyboard and receive input text.
	 */
	virtual bool attachWithIME();
    
	/**
	 @brief    End text input and close keyboard.
	 */
	virtual bool detachWithIME();
    
	virtual void setString(const char *text);
    
	virtual void deleteBackward();
    
    virtual void setPlaceHolder(const char * text);
    
   // virtual void setTouchDispatcher(bool bSwallowsTouches);
    
//    void registerWithTouchDispatcher();
//    
//    void unRegisterWithTouchDispatcher();
    
    CC_SYNTHESIZE(int, m_nMaxLength, MaxLength);
    
    CC_SYNTHESIZE(Size, m_nSize, Size);
    
    CC_SYNTHESIZE(int, m_nType, Type);
    
private:
    
	void initCursor(int height);
    
	bool isInTextField(Touch *pTouch);
    
    void setContentSize(const cocos2d::Size &contentSize);
    
private:
	// 点击开始位置
    cocos2d::Vec2 m_beginPos;
    
	// 光标精灵
	Sprite *m_pCursorSprite;
    
	// 光标动画
	Action *m_pCursorAction;
    
    bool m_isInputState;
    
    bool m_nbSwallowsTouches;
    
    bool m_IsRehisterTouchDispatcher;
    
     cocos2d::Vec2 m_touchBeginUIPoint;
};

NS_CC_END

#endif /* defined(__mir_mobile__LTextField__) */
