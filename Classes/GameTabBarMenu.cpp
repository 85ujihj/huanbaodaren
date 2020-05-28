

#include "GameTabBarMenu.h"
#include "GameScene.h"
#include "SkillTableView.h"
#include "PropColumn.h"
#include "GameOptions.h"


#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

#define WINSIZE Director::getInstance()->getWinSize()

bool GameTabBarMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    /*
     //    Layout* widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/head_UI/head_UI.json"));
     //    if (widget)
     //    {
     //        //layer->addWidget(widget);
     //        layer->addChild(widget);
     //
     //    }
     ui::Widget * pNode=cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/head_UI/head_UI.json");
     layer->addChild(pNode);
     
     
     //坑啊 label 改为text了 诶
     const char* nickName = PlayerController::sharePlayerController()->getPlayerName().c_str();
     m_nickNameTTF = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(pNode,"nickName"));
     if (m_nickNameTTF)
     {
     m_nickNameTTF->setString(nickName);
     }
     */
    
    Layout* widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/tabbarMenu/tabbarMenu.json"));
    if (widget)
    {
        widget->setTouchEnabled(true);
        this->addChild(widget);
    }

    Button* roleInfoBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_role"));
    if (roleInfoBtn)
    {
         roleInfoBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showRoleInfo));
        
    }

    Button* backPackBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_backPack"));
    if (backPackBtn)
    {
      
        backPackBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showBackPack));
    }

    Button* skillBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_skill"));
    if (skillBtn)
    {
        
        skillBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showSkillInfo));
    }

    Button* strengthenBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_strengthen"));
    if (strengthenBtn)
    {
      
        strengthenBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showStrengthen));
    }

    Button* makeFriendsBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_makeFriends"));
    if (makeFriendsBtn)
    {
        makeFriendsBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showMakeFriends));
    }

    Button* destinyBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_destiny"));
    if (destinyBtn)
    {
        destinyBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showDestiny));
    }

    Button* mallBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_mall"));
    if (mallBtn)
    {
        mallBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showMall));
    }

    Button* canonBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButton_canon"));
    if (canonBtn)
    {
        canonBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showCanon));
    }

    Button* setUpBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(widget,"TextButtonsetUp"));
    if (setUpBtn)
    {
        setUpBtn->addTouchEventListener(this, toucheventselector(GameTabBarMenu::showSetUp));
    }
    
    return true;
}

void GameTabBarMenu::showRoleInfo(Ref* sender , Control::EventType controlEvent)
{

}

void GameTabBarMenu::showBackPack(Ref* sender , Control::EventType controlEvent)
{
    PropColumnMenu* layer= PropColumnMenu::create();
    GAME_UILAYER->addSmallMenu(layer);
    GAME_UILAYER->getOperationMenu()->joinEditState();
}

void GameTabBarMenu::showSkillInfo(Ref* sender , Control::EventType controlEvent)
{
    SkillTableView* layer = SkillTableView::create();
    GAME_UILAYER->addSmallMenu(layer);
    GAME_UILAYER->getOperationMenu()->joinEditState();
}

void GameTabBarMenu::showStrengthen(Ref* sender , Control::EventType controlEvent)
{
    
}

void GameTabBarMenu::showMakeFriends(Ref* sender , Control::EventType controlEvent)
{
    
}

void GameTabBarMenu::showDestiny(Ref* sender , Control::EventType controlEvent)
{
    
}

void GameTabBarMenu::showMall(Ref* sender , Control::EventType controlEvent)
{
    
}

void GameTabBarMenu::showCanon(Ref* sender , Control::EventType controlEvent)
{
    
}

void GameTabBarMenu::showSetUp(Ref* sender , Control::EventType controlEvent)
{
    GameOptions* layer = GameOptions::create();
    GAME_UILAYER->addBigMenu(layer);
}

void GameTabBarMenu::hideGameTabBarMenu()
{
    this->stopAllActions();
    Size winSize = Director::getInstance()->getWinSize();
    MoveTo* moveTo = MoveTo::create(0.3f, Point(winSize.width-960, -100));
    EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    this->runAction(easeBack);
}

void GameTabBarMenu::showGameTabBarMenu()
{
    this->stopAllActions();
    Size winSize = Director::getInstance()->getWinSize();
    MoveTo* moveTo = MoveTo::create(0.3f, Point(winSize.width-960, 0));
    EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    this->runAction(easeBack);
}
