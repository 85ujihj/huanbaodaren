

#include "LAlertView.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
LAlertView::LAlertView(const char* title, const char* messageText)
:m_title(title)
,m_messageText(messageText)
,m_target(NULL)
,m_callBack(NULL)
{
    m_btnArray = CCArray::create();
    
}

LAlertView::~LAlertView()
{
    CC_SAFE_RELEASE_NULL(m_btnArray);
}

void LAlertView::onEnter()
{
    Scale9Sprite::onEnter();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->addTargetedDelegate(this, kCCMenuHandlerPriority-1000, true);
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(LAlertView::onTouchBegan, this);
    //listener->onTouchMoved=CC_CALLBACK_2(GameScene::onTouchMoved, this);
    //listener->onTouchEnded=CC_CALLBACK_2(GameScene::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void LAlertView::onExit()
{
    Scale9Sprite::onExit();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->removeDelegate(this);
}

LAlertView* LAlertView::create(const char* title, const char* messageText)
{
    LAlertView* _alertView = new LAlertView(title, messageText);
    if (_alertView && _alertView->init())
    {
        _alertView->autorelease();
    }
    return _alertView;
}

bool LAlertView::init()
{
    if (!Scale9Sprite::initWithFile("ui/alert_backGround.png"))
        return false;
    
    LabelTTF* title = LabelTTF::create(m_title, "Helvetica-Bold", 22);
    title->setAnchorPoint(Point(0.5, 1));
    title->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height-52));
    this->addChild(title);
    
    CCLabelTTF* message = CCLabelTTF::create(m_messageText, "Helvetica-Bold", 20);
    message->setPosition(ccpAdd(this->getContentSize()/2, Point(0, 30)));
    this->addChild(message);
    
    return true;
}

void LAlertView::addButtonWithTitle(const char *btnTitle)
{
    CCLabelTTF* message = CCLabelTTF::create(btnTitle, "Helvetica-Bold", 25);
    ControlButton* btn = ControlButton::create(message, Scale9Sprite::create("ui/btn_normal.png"));
    btn->setBackgroundSpriteForState(Scale9Sprite::create("ui/btn_selected.png"), Control::State::SELECTED);
    btn->setPreferredSize(Size(140, 50));
   // btn->addTargetWithActionForControlEvents(this, cccontrol_selector(LAlertView::touchUpInSide), Control::EventType::TOUCH_UP_INSIDE);
    btn->setTag(0xff+m_btnArray->count());
   // btn->setTouchPriority(kCCMenuHandlerPriority-1001);
    m_btnArray->addObject(btn);
}

void LAlertView::show(Ref *target, SEL_Alert selector)
{
    m_target = target;
    m_callBack = selector;

    if (m_btnArray->count() > 2)
    {
        this->setPreferredSize(Size((m_btnArray->count() + 1) * 150, this->getContentSize().height));
    }
    
    if (m_btnArray->count() == 0)
    {
        LabelTTF* message = LabelTTF::create("确定", "Helvetica-Bold", 18);
        ControlButton* btn = ControlButton::create(message, Scale9Sprite::create("ui/btn_normal.png"));
        btn->setBackgroundSpriteForState(Scale9Sprite::create("ui/btn_selected.png"), Control::State::SELECTED);
        btn->setPreferredSize(Size(130, 43));
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(LAlertView::touchUpInSide), Control::EventType::TOUCH_UP_INSIDE);
        btn->setTag(0xff);
        //btn->setTouchPriority(-128-1001);
        btn->setPosition(Point(this->getContentSize().width/2, 100));
        this->addChild(btn);
    }
    else
    {
        for (int i=0; i<m_btnArray->count(); i++)
        {
            ControlButton* btn = (ControlButton*)m_btnArray->objectAtIndex(i);
            float x = this->getContentSize().width / (m_btnArray->count()+1) * (i + 1);
            btn->setPosition(CCPoint(x, 100));
            this->addChild(btn);
        }
    }
    this->setPosition(Director::getInstance()->getWinSize()/2);
    Director::getInstance()->getRunningScene()->addChild(this, 0xffff);
}

void LAlertView::touchUpInSide(Ref* sender, Control::EventType controlEvent)
{
//    ControlButton* btn = (ControlButton*)sender;
//    
//    if (m_target && m_callBack)
//    {
//        ((Ref *)m_target->*m_callBack)(btn->getTag()-0xff);
//    }
    this->removeFromParent();
}

bool LAlertView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}
