//
//  LoginScene.h
//  guyue
//
//  Created by dsh on 15/1/7.
//
//
/*
 * 段生辉 20150107
 */
#ifndef __guyue__LoginScene__
#define __guyue__LoginScene__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/cocosgui.h"
#include "cocostudio/cocostudio.h"
using namespace cocos2d;
using namespace CocosDenshion;
using namespace ui;
using namespace cocostudio;
class LoginScene:public Layer
{
public:
    bool init();
    static Scene * createScene();
    CREATE_FUNC(LoginScene);
    void joinGame();
    void btnClick(Ref * obj);
};



#endif /* defined(__guyue__LoginScene__) */
