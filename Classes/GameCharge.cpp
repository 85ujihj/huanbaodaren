//
//  GameCharge.cpp
//  GuangTouQiang
//
//  Created by Thunder on 2019/1/7.
//

#include "GameCharge.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ios_helper/ChargeUtil.h"//zsd_replace_ioshelper

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

//zsd_publish_functions

bool GameCharge::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
//    auto root = CSLoader::getInstance()->createNode("charge/chargeLayer.csb");
//    addChild(root);
//    root->setScaleX(960.0f/1280.0f);
//    root->setScaleY(640.0f/ 720.0f);
//
//    auto btn_6 = dynamic_cast<Button*>(root->getChildByName("btn_6"));
//    btn_6->addClickEventListener([=](Ref* sender) {
//
//        ChargeUtil::doCharge(0);
//        this->removeFromParent();
//    });
//
//    auto btn_30 = dynamic_cast<Button*>(root->getChildByName("btn_30"));
//    btn_30->addClickEventListener([=](Ref* sender) {
//
//        ChargeUtil::doCharge(1);
//        this->removeFromParent();
//    });
//
//    auto btn_68 = dynamic_cast<Button*>(root->getChildByName("btn_68"));
//    btn_68->addClickEventListener([=](Ref* sender) {
//
//        ChargeUtil::doCharge(2);
//        this->removeFromParent();
//    });
    
    return true;
}

//zsd_publish_functions

//zsd_publish_functions
