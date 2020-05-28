

#include "NpcFigure.h"
//#include "LAlertView.h"
#include "PopupLayer.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

NpcFigure::NpcFigure(unsigned int roleNumber, NPCDirectionType type)
:m_npcInfo(NULL)
,m_eTpye(type)
,m_fileName(NULL)
,m_plistName(NULL)
,m_sprite(NULL)
,m_nIsDisplay(false)
,m_fAnchorPoint(Vec2(0.5f, 3/8.0f))
,m_fSpriteRect(Rect(80, 64, 50, 96))
{
    m_npcInfo = NpcInfoSystem::getNpcInfo(roleNumber);
    this->setNpcFileName(CCString::createWithFormat("texture_npc/npc_%u_1_%d.png", m_npcInfo->getRID(), m_eTpye));
    this->setNpcPlistName(CCString::createWithFormat("texture_npc/npc_%u_1_%d.plist", m_npcInfo->getRID(), m_eTpye));
    //m_pArray = new CCArray();
}

NpcFigure::~NpcFigure()
{
    CC_SAFE_DELETE(m_fileName);
    CC_SAFE_DELETE(m_plistName);
    m_pArray.clear();
    //CC_SAFE_DELETE(m_pArray);
}

void NpcFigure::onEnter()
{
    CCSprite::onEnter();
//    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//    touchDispatcher->addTargetedDelegate(this, 100, true);
    
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(NpcFigure::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(NpcFigure::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(NpcFigure::onTouchEnded, this);
    listener->onTouchCancelled=CC_CALLBACK_2(NpcFigure::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void NpcFigure::onExit()
{
    CCSprite::onExit();
//    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//    touchDispatcher->removeDelegate(this);
    
    //Director::getInstance()->getEventDispatcher()->removeEventListener(this);
    this->hideImageView();
}

NpcFigure* NpcFigure::create(unsigned int roleNumber, NPCDirectionType type)
{
    NpcFigure* npc = new NpcFigure(roleNumber, type);
    if (npc && npc->init())
    {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return NULL;
}

void NpcFigure::displayImageView()
{
    if (m_nIsDisplay == false)
    {
        m_nIsDisplay = true;
        
       // TextureCache::sharedTextureCache()->addImageAsync(m_fileName->getCString(), this, callfuncO_selector(NpcFigure::initWithImageView));
        Director::getInstance()->getTextureCache()->addImageAsync(m_fileName->getCString(), CC_CALLBACK_1(NpcFigure::initWithImageView, this));
    }
}

void NpcFigure::hideImageView()
{
    if (m_nIsDisplay)
    {
        m_nIsDisplay = false;

        if (m_sprite)
        {
            m_sprite->removeFromParent();
            m_sprite = NULL;
        }
        
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(m_plistName->getCString());
        Director::getInstance()->getTextureCache()->removeTextureForKey(m_fileName->getCString());
        //m_pArray->removeAllObjects();
        m_pArray.clear();
    }
}

void NpcFigure::initWithImageView(Texture2D* texture)
{
    if (m_sprite == NULL)
    {
        m_sprite = CCSprite::create();
        m_sprite->setAnchorPoint(m_fAnchorPoint);
        this->addChild(m_sprite);
        
        m_spriteHigh = CCSprite::create();
        m_spriteHigh->setAnchorPoint(m_fAnchorPoint);
        this->addChild(m_spriteHigh);
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_plistName->getCString(), texture);
        
        m_sprite->runAction(this->getActions());
        m_spriteHigh->runAction(this->getActions());
        
        m_spriteHigh->setVisible(false);
    }
}

void NpcFigure::IntelligentDisplay(Rect& showRect, Rect& hideRect)
{
    if (showRect.containsPoint(this->getPosition()))
    {
        this->displayImageView();
    }
    
    if (!hideRect.containsPoint(this->getPosition()))
    {
        this->hideImageView();
    }
}

Action* NpcFigure::getActions()
{
   // if (m_pArray->count() == 0)
    if (m_pArray.size() == 0)
    {
        int flag = 0;
        do
        {
            CCString* string = CCString::createWithFormat("npc_%u_1_%d_%02d.png", m_npcInfo->getRID(), m_eTpye, flag);
            SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(string->getCString());
            CC_BREAK_IF(spriteFrame == NULL);
           // m_pArray->addObject(spriteFrame);
            m_pArray.pushBack(spriteFrame);
            flag++;
        }
        while (1);
    }
    
    //if (m_pArray->count() > 0)
    if (m_pArray.size() > 0)
    {
        Animation* movie = Animation::createWithSpriteFrames(m_pArray, 1 / (float)5);
        Animate* animate = Animate::create(movie);
        RepeatForever* repeatForever = CCRepeatForever::create(animate);
        return repeatForever;
    }
    return NULL;
}

void NpcFigure::setHighlight()
{
    if (m_spriteHigh->isVisible() == false)
    {
        ccBlendFunc blend = {GL_DST_COLOR, GL_ONE};
        
        m_spriteHigh->setBlendFunc(blend);
        
        m_spriteHigh->setVisible(true);
    }
}

void NpcFigure::setNormal()
{
    if (m_spriteHigh->isVisible() == true)
    {
//        ccBlendFunc blend = {CC_BLEND_SRC, CC_BLEND_DST};
//        
//        m_spriteHigh->setBlendFunc(blend);
        
        m_spriteHigh->setVisible(false);
    }
}

bool NpcFigure::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (m_sprite == NULL)
        return false;
    
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    if (!m_fSpriteRect.containsPoint(point))
    {
        return false;
    }
    
    this->setHighlight();
    
    return true;
}

void NpcFigure::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    if (!m_fSpriteRect.containsPoint(point))
    {
        this->setNormal();
    }
    else
    {
        this->setHighlight();
    }
    
}

void NpcFigure::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    this->setNormal();
    
    //CCLog("%s,%s",m_npcInfo->getNpcName().c_str(), m_npcInfo->getSentence().c_str());
    
    //LAlertView* alertView = LAlertView::create("", m_npcInfo->getSentence().c_str());
    //alertView->show(this, Alert_selector(NpcFigure::alertCallBack));
    
    // 定义一个弹出层，传入一张背景图片
    PopupLayer* popup = PopupLayer::create("ui/alert_backGround.png");
    // ContentSize是可选的设置，可以不设置，如果设置则把它当做9图缩放
    popup->setContentSize(Size(300, 200));
    popup->setTitle("提示");
    popup->setContentText(m_npcInfo->getSentence().c_str(), 20, 50, 150);
    // 设置回调函数，回调传回一个CCNode以获取tag判断点击的按钮
    // 这只是作为一种封装实现，如果使用delegate那就能够更灵活的控制参数了
    popup->setCallBackFunc(this, callfuncN_selector(NpcFigure::buttonCallback));
    //添加按钮，设置图片、文字，tag信息
    popup->addButton("ui/queding01.png", "ui/queding02.png", "Ok", 0);
    //popup->addButton("ui/btn_normal.png", "ui/btn_selected.png", "Cancel", 1);
    popup->show();
    
    
    
}
void NpcFigure::buttonCallback(cocos2d::Node *pNode){
    // 打印 tag 0， 确定，1 ，取消
    //CCLog("button call back. tag: %d", pNode->getTag());
    int tag=pNode->getTag();
    if (tag==0) {
        pNode->removeFromParent();
    }
}

void NpcFigure::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

void NpcFigure::alertCallBack(const int number)
{
    if (number == 0)
    {
        
    }
}
