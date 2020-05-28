//
//  LoginScene.cpp
//  guyue
//
//  Created by dsh on 15/1/7.
//
//

#include "LoginScene.h"
#include "RoleCreateLayer.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

bool LoginScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto wsize=Director::getInstance()->getWinSize();
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/tlcangshan.mp3",true);
    
//    auto node=GUIReader::getInstance()->widgetFromJsonFile("gylogin_1.json");
//    this->addChild(node);
//    
//    Button * btnBengin=(Button*)node->getChildByTag(110);
//    btnBengin->addTouchEventListener([](Ref* obj,Widget::TouchEventType t){
//    
//        Director::getInstance()->replaceScene(RoleCreateLayer::createScene());
//    });
    
    Sprite * bj=Sprite::create("logo.png");
    bj->setPosition(wsize.width/2, wsize.height/2);
    this->addChild(bj);
    
    MenuItemImage * login=MenuItemImage::create("btn_start_d.png", "btn_start_n.png", CC_CALLBACK_1(LoginScene::btnClick, this));
    login->setPosition(wsize.width/2, login->getContentSize().height+10);
    Menu *menu=Menu::create(login, NULL);
    menu->setAnchorPoint(Point::ZERO);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    return true;
}

void LoginScene::btnClick(Ref * obj){
    Director::getInstance()->replaceScene(RoleCreateLayer::createScene());
}

Scene * LoginScene::createScene()
{
    auto scene=Scene::create();
    auto layer=LoginScene::create();
    scene->addChild(layer);
    return scene;
}

//void LoginScene::joinGame()
//{
//    Director::getInstance()->replaceScene(RoleCreateLayer::createScene());
//    this->removeFromParent();
//}

