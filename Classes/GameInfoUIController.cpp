

#include "GameInfoUIController.h"
#include "LMenu.h"
#include "PlayerController.h"
#include "Player.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

GameInfoUIController::GameInfoUIController()
:m_operationMenu(NULL)
,m_isShowTabBar(true)
,m_headIcon(NULL)
,m_headBtn(NULL)
,m_levelTTF(NULL)
,m_nickNameTTF(NULL)
,m_bloodPro(NULL)
,m_magicPro(NULL)
,m_gameTabBarMenu(NULL)
,m_attackEnemyInfo(NULL)
,m_chatInterface(NULL)
,m_pMenuBig(NULL)
{

}

GameInfoUIController::~GameInfoUIController()
{

}


bool GameInfoUIController::init()
{
    if (!Layer::init())
        return false;
    
    m_winSize = Director::getInstance()->getWinSize();
    // 头像 血条
    this->initWithHeadUI();
    //右上小地图
    m_mapThumbnailMenu = MapThumbnailMenu::create();
    m_mapThumbnailMenu->setPosition(ccpSub(m_winSize, Point(100, 100)));
    this->addChild(m_mapThumbnailMenu);
    //技能条
    m_operationMenu = LOperationMenu::create();
    this->addChild(m_operationMenu);

    //初始化敌人信息（血条 名字）一开始隐藏
    m_attackEnemyInfo = AttackEnemyInfo::create();
    m_attackEnemyInfo->setPosition(Point(m_winSize.width/2-125, m_winSize.height-110));
    this->addChild(m_attackEnemyInfo);
    m_attackEnemyInfo->hide();

    //频道 
    m_chatInterface = ChatInterface::create();
    m_chatInterface->setPosition(Point(220, 0));
    this->addChild(m_chatInterface);
    //工具条
    m_gameTabBarMenu = GameTabBarMenu::create();
    m_gameTabBarMenu->setPosition(Point(m_winSize.width-960, -100));
    this->addChild(m_gameTabBarMenu);
    
    return true;
}

void GameInfoUIController::initWithHeadUI()
{
    /*
     //获取我们的Cocos Studio   UIdemo
     //这里我们创建了一个widget，这里说明下貌似cocostudio有自己的一套代码风格和api
     //有些功能能和cocos2dx混合着用
     //这里的widget有点类似一个layer，反正我是这样理解的
     ui::Widget * pNode=cocostudio::GUIReader::getInstance()->widgetFromJsonFile("DemoLogin.json");
     this->addChild(pNode);
     
     //我们从widget中找到那个button的名字然后实例出来（跟android有点像）
     ui::Button * button=(ui::Button *)ui::Helper::seekWidgetByName(pNode,"login_Button");
     //给这个按钮增加一个touch的侦听(这边就和cocos2dx有不同了)
     button->addTouchEventListener(this,toucheventselector(HelloWorld::menuCloseCallback));
     */
    Layer* layer = Layer::create();
    layer->setScale(1.0f);
    layer->setPosition(Point(-480*(1-layer->getScale()), m_winSize.height-320));
    this->addChild(layer);
    
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

    m_headIcon = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(pNode,"ImageView"));
    if (m_headIcon)
    {
        m_headIcon->setTouchEnabled(true);
    }
      //TextAtlas
    m_levelTTF = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(pNode,"level"));
    if (m_levelTTF)
    {
        m_levelTTF->setString("3");
    }

    m_bloodPro = dynamic_cast<LoadingBar*>(ui::Helper::seekWidgetByName(pNode,"redBar"));
    if (m_bloodPro)
    {
        m_bloodPro->setPercent(100);
    }
    
    m_magicPro = dynamic_cast<LoadingBar*>(ui::Helper::seekWidgetByName(pNode,"blueBar"));
    if (m_magicPro)
    {
        m_magicPro->setPercent(100);
    }

    m_headBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(pNode,"head"));
    if (m_headBtn)
    {
        m_headBtn->setTouchEnabled(true);
        m_headBtn->addTouchEventListener(this, toucheventselector(GameInfoUIController::modeSwitch));
    }
    
}

void GameInfoUIController::modeSwitch(cocos2d::Ref *sender)
{
    if (this->getActionByTag(0xacff))
        return;
    
    if (m_isShowTabBar)
    {
        m_isShowTabBar = false;
        m_operationMenu->hideOperationMenu();
        m_chatInterface->hideChatInterface();
        
        DelayTime* delay = DelayTime::create(0.3f);
        CallFunc* finish = CallFunc::create(m_gameTabBarMenu, callfunc_selector(GameTabBarMenu::showGameTabBarMenu));
        Sequence* actions = Sequence::create(delay, finish, NULL);
        actions->setTag(0xacff);
        this->runAction(actions);
    }
    else
    {
        m_isShowTabBar = true;
        m_gameTabBarMenu->hideGameTabBarMenu();
    
        DelayTime* delay = DelayTime::create(0.3f);
        CallFunc* finish = CallFunc::create(m_operationMenu, callfunc_selector(LOperationMenu::showOperationMenu));
        CallFunc* finish2 = CallFunc::create(m_chatInterface, callfunc_selector(ChatInterface::showChatInterface));
        Sequence* actions = Sequence::create(delay, finish, finish2, NULL);
        actions->setTag(0xacff);
        this->runAction(actions);
    }
}

void GameInfoUIController::updateBloodPro()
{
    m_bloodPro->setPercent(100*Player::sharePlayer()->getBlood()/Player::sharePlayer()->getBloodCap());
}

void GameInfoUIController::updateMagicPro()
{
    m_bloodPro->setPercent(100*Player::sharePlayer()->getMagic()/Player::sharePlayer()->getMagicCap());
}

void GameInfoUIController::updateLevelTTF()
{

}

void GameInfoUIController::addSmallMenu(cocos2d::Node *node)
{
    this->removeBigMenuAndButton(this,Control::EventType::TOUCH_UP_INSIDE);
    
    for (std::deque<Node*>::iterator itr=m_pMenuSmall.begin(); itr!=m_pMenuSmall.end(); itr++)
    {
        if (typeid(*node) == typeid(**itr))
            return;
    }
    
    if (m_pMenuSmall.size() >= 2)
    {
        m_pMenuSmall.at(0)->removeFromParent();
        m_pMenuSmall.pop_front();
        m_pMenuSmall.at(0)->setPosition(Point(m_winSize.width/4, (m_winSize.height+80)/2));
    }
    
    node->setPosition(Point(m_winSize.width/4 * (m_pMenuSmall.size()*2+1), (m_winSize.height+80)/2));
    this->addChild(node);
    m_pMenuSmall.push_back(node);
    
    this->setTouchEnabled(true);
}

void GameInfoUIController::removeSmallMenu(Node *node)
{
    if (node == NULL)
        return;
    
    std::deque<Node*>::iterator* pItr = NULL;
    for (std::deque<Node*>::iterator itr=m_pMenuSmall.begin(); itr!=m_pMenuSmall.end(); itr++)
    {
        if (node==(*itr))
        {
            pItr = &itr;
            break;
        }
    }
    
    if (pItr)
    {
        m_pMenuSmall.erase(*pItr);
        node->removeFromParent();
    }
    
    if (m_pMenuSmall.size() > 0)
    {
        m_pMenuSmall.at(0)->setPosition(Point(m_winSize.width/4, (m_winSize.height+80)/2));
    }
    else
    {
        this->setTouchEnabled(false);
    }
}

void GameInfoUIController::removeSmallMenuAndButton(Ref *node, Control::EventType controlEvent)
{
    this->removeSmallMenu((Node*)node);
}

void GameInfoUIController::addBigMenu(cocos2d::Node *node)
{
    this->removeBigMenuAndButton(this,Control::EventType::TOUCH_UP_INSIDE);
    
    if (m_pMenuSmall.size() > 0)
    {
        for (std::deque<Node*>::iterator itr=m_pMenuSmall.begin(); itr!=m_pMenuSmall.end(); itr++)
            (*itr)->removeFromParent();
        
        m_pMenuSmall.clear();
    }
    
    
    node->setPosition(Point(m_winSize.width/2, (m_winSize.height+80)/2));
    this->addChild(node);
    m_pMenuBig = node;
    
    this->setTouchEnabled(true);
}

void GameInfoUIController::removeBigMenuAndButton(Ref* node, Control::EventType controlEvent)
{
    if (m_pMenuBig)
    {
        m_pMenuBig->removeFromParent();
        m_pMenuBig = NULL;
    }
    this->setTouchEnabled(false);
}

bool GameInfoUIController::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}
