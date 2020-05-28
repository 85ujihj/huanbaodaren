//
//  MAlert.h
//  guyue
//
//  Created by dsh on 15/1/18.
//
//

#ifndef __guyue__MAlert__
#define __guyue__MAlert__

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;

USING_NS_CC_EXT;


class PopupLayer : public Layer{
    
public:
    
    PopupLayer();
    ~PopupLayer();
    virtual bool init();
    
    
    //需要重写触摸注册函数，重新给定触摸级别
    //virtual void registerWithTouchDispatcher();
    
    //重写触摸函数，返回true，屏蔽其它层，达到“模态”效果
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    
    //静态创建函数，创建一个弹出层，设置背景图片
    static PopupLayer* create(const char* backgroundImage);
    
    
    //设置标题
    void setTitle(const char* title, int fontsize = 20);
    
    //设置文本内容，padding 为文字到对话框两边预留的距离，这是可控的，距上方的距离亦是如此
    void setContentText(const char* text, int fontsize=20, int padding=50, int paddingTop=100);
    
    
    //设置上层对象和上层回调函数，用于回调时传递CCNode参数
    void setCallBackFunc(Ref* target, SEL_CallFuncN callfun);
    
    
    //添加menuItem按钮，封装了一个函数，传入些必要的参数
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag=0);
    
    
    //为了在显示层时的属性生效，选择在onEnter里动态生成
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(PopupLayer);
    void show();
    
private:
    
    void buttonCallBack(Ref* pSender);
    
    
    //文字内容两边的空白区域
    int m_contentPadding;
    int m_contentPaddingTop;
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
    
    //定义了CCMenu*类型变量m_pMenu, 并且直接定义默认的set/get方法
    
    CC_SYNTHESIZE_RETAIN(Menu*, m_pMenu, MenuButton);
    CC_SYNTHESIZE_RETAIN(Sprite*, m_sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m_s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(LabelTTF*, m_ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(LabelTTF*, m_ltContentText, LabelContentText);
    
};

#endif /* defined(__guyue__MAlert__) */
