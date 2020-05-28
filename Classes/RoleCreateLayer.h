//
//  RoleCreateLayer.h
//  guyue
//
//  Created by dsh on 15/1/7.
//
//
/*
 * 段生辉 20150107
 */
#ifndef __guyue__RoleCreateLayer__
#define __guyue__RoleCreateLayer__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/cocosgui.h"
#include "cocostudio/cocostudio.h"
#include "extensions/cocos-ext.h" 
using namespace cocos2d;
using namespace CocosDenshion;
using namespace ui;
using namespace cocostudio;
USING_NS_CC_EXT;
class RoleCreateLayer:public Layer
{
private:
    Ref * m_selected;
    std::string m_nickName;
    int m_roleID;
    Vector<Ref*>m_roleArray;
    //CCArray* m_roleArray;
    //UITextButton* m_textButton;
    //UITextField* m_textField;
    
    
public:
    RoleCreateLayer();
    virtual ~RoleCreateLayer();
    bool init();
    static Scene * createScene();
    CREATE_FUNC(RoleCreateLayer);
    void joinGame();
    //void runActionsForFigure(Ref* pSender);
    void setSelector(Ref* pSender, Control::EventType controlEvent);
    void randomNickName(Ref* pSender);
    void sendMessage(Ref* pSender);
    void callBackEnter(Ref* obj);
    void runActionsForFigure(cocos2d::Ref *pSender);
};

#endif /* defined(__guyue__RoleCreateLayer__) */
