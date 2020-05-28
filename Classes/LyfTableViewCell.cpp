

#include "LyfTableViewCell.h"
#include "SimpleAudioEngine.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

#define DELBTNICON "ui/indicator.png"
#define DELBTNPOINT Point(this->getContentSize().width-m_delBtn->getContentSize().height/2, this->getContentSize().height/2)

LyfTableViewCell::LyfTableViewCell()
:m_sign(LyfIndexPathZero)
,m_textTag("")
,m_imageView(NULL)
,m_textLabel(NULL)
,m_detailTextLabel(NULL)
,m_touchBeginUIPoint(Point::ZERO)
,m_bIsTouchEnabled(true)
,m_pitchOn(false)
,m_touchBegin(false)
,m_specially(false)
,m_delBtn(NULL)
,m_delegate(NULL)
,m_delRect(Rect::ZERO)
,m_touchModel(tCell)
,m_nTouchPriority(0)
{
}

LyfTableViewCell::~LyfTableViewCell()
{

}

void LyfTableViewCell::onExit()
{
    
    if (m_delegate)
    {
        m_delegate->removeDelegate(this);
        m_delegate=NULL;
    }
    
    //this->unRegisterWithTouchDispatcher();
    Sprite::onExit();
}

void LyfTableViewCell::onEnter()
{
    
    Sprite::onEnter();
    //this->registerWithTouchDispatcher();
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(LyfTableViewCell::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(LyfTableViewCell::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(LyfTableViewCell::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void LyfTableViewCellDelegate::removeDelegate(LyfTableViewCell *cell)
{
    cell->setDelegate(NULL);
}

LyfTableViewCell* LyfTableViewCell::create(const char *fileName)
{
    
    LyfTableViewCell* tableViewCell=new LyfTableViewCell();
    if (tableViewCell && tableViewCell->initWithFile(fileName))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

LyfTableViewCell* LyfTableViewCell::createWithSpriteFrameName(const char *fileName)
{
    
    LyfTableViewCell* tableViewCell=new LyfTableViewCell();
    if (tableViewCell && tableViewCell->initWithSpriteFrameName(fileName))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

LyfTableViewCell* LyfTableViewCell::createTmp()
{
    
    LyfTableViewCell* tableViewCell=new LyfTableViewCell();
    if (tableViewCell && tableViewCell->init())
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

LyfTableViewCell* LyfTableViewCell::createWithTexture(Texture2D *pTexture)
{
    
    LyfTableViewCell *tableViewCell = new LyfTableViewCell();
    if (tableViewCell && tableViewCell->initWithTexture(pTexture))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

void LyfTableViewCell::setImageView(Sprite* sprite)
{
    
    if (sprite==NULL) return;

    if (m_imageView!=NULL)
    {
        m_imageView->removeFromParentAndCleanup(true);
    }
    
    m_imageView=sprite;
    
    if (m_imageView->getContentSize().height>this->getContentSize().height)
    {
        m_imageView->setScale(this->getContentSize().height/m_imageView->getContentSize().height);
    }
    
    m_imageView->setPosition(Point(m_imageView->getContentSize().width/2*m_imageView->getScale(),
                                    this->getContentSize().height/2));
    this->addChild(m_imageView);
}

Sprite* LyfTableViewCell::getImageView(){

    return m_imageView;
}

void LyfTableViewCell::setTextLabel(LabelTTF* textLabel)
{
    
    if (m_textLabel)
    {
        m_textLabel->removeFromParentAndCleanup(true);
    }
    
    m_textLabel=textLabel;
    
    this->addChild(m_textLabel);
}

LabelTTF* LyfTableViewCell::getTextLabel()
{
    
    return m_textLabel;
}

void LyfTableViewCell::setDetailTextLabel(LabelTTF* detailTextLabel)
{
    
    if (m_detailTextLabel)
    {
        m_detailTextLabel->removeFromParentAndCleanup(true);
    }
    
    m_detailTextLabel=detailTextLabel;
    
    this->addChild(m_detailTextLabel);
}

LabelTTF* LyfTableViewCell::getDetailTextLabel()
{
    
    return m_detailTextLabel;
}

void LyfTableViewCell::setTouchModel(tTouchModel tEnum)
{
    
    m_touchModel=tEnum;
}

Rect LyfTableViewCell::getDelRect()
{
    
    if (!m_delBtn)
        return Rect::ZERO;

    Rect rect=Rect::ZERO;
    float x=m_delBtn->getPositionX()-m_delRect.size.width;
    float y=m_delBtn->getPositionY()-m_delRect.size.height/2;
    Point point=Point(x, y);
    Size size=Size(m_delRect.size.width, m_delRect.size.height);
    rect.origin=point;
    rect.size=size;
    return rect;
    
}

Rect LyfTableViewCell::getBtnRect(Sprite* sprite)
{
    
    if (!sprite) return Rect::ZERO;
    
    return Rect( 1, 1, sprite->getContentSize().width-2, sprite->getContentSize().height-2);
}

Rect LyfTableViewCell::getRect(){
    
    return Rect( 1, 1, getContentSize().width-2, getContentSize().height-2);
}

bool LyfTableViewCell::isOverstep(Point point)
{
    
    if (!this->getParent()->getParent()) return false;

    if (!this->getParent()) return false;

    if ((this->getPositionY()+this->getParent()->getPositionY()-this->getContentSize().height/2+point.y)<0)
    {
        return true;
    }
    
    if ((this->getPositionY()+this->getParent()->getPositionY()
         -this->getContentSize().height/2+point.y)
        > (this->getParent()->getParent()->getContentSize().height))
    {
        return true;
    }
    
    return false;
}

bool LyfTableViewCell::onTouchBegan(Touch *pTouch,Event *pEvent)
{
    
    m_touchBeginUIPoint=pTouch->getLocationInView();
    
    Point uipoint=pTouch->getLocationInView();
    Point point=Director::getInstance()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (isOverstep(point)) return false;

    if (!this->isVisible()) return false;

    if (m_delegate && !m_delegate->isCanSelected()) return false;
    
    if (m_delegate && m_delegate->isCanEdit())
    {
        if (getDelRect().containsPoint(point))
        {
            
            m_touchModel=tDel;
            return true;
        }
        else {
            if (m_delBtn)
            {
                this->removeDelBtnParent();
                return false;
            }
        }
    }
    
    if (!getRect().containsPoint(point)) return false;
    
    if (getBtnRect(m_imageView).containsPoint(point))
    {
        m_touchModel=tLead;
        return true;
    }
    
    if (!m_bIsTouchEnabled) return false;

    m_touchModel = tCell;
    
    do
    {
        CC_BREAK_IF(m_touchBegin == true);
        
        m_touchBegin = true;
        
        DelayTime* delayTime = DelayTime::create(0.1f);
        CallFunc* func = CallFunc::create(this, callfunc_selector(LyfTableViewCell::setTouchedEffect));
        Sequence* actions = Sequence::create(delayTime, func, NULL);
        this->runAction(actions);
        actions->setTag(0xffff);
    }
    while (0);
    
    return true;
}

void LyfTableViewCell::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    
    float lenght=ccpDistance(pTouch->getLocationInView(), m_touchBeginUIPoint);
    if (lenght<7) return;
    
    Point uipoint=pTouch->getLocationInView();
    Point point=Director::getInstance()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point); 
    
    Point uipointpre=pTouch->getPreviousLocationInView();
    Point pointpre=Director::getInstance()->convertToGL(uipointpre);
    pointpre=this->convertToNodeSpace(pointpre);
    
    if (m_delegate && m_delegate->isEditing()) return;

    if (m_touchModel==tCell && fabsf(point.y-pointpre.y)/ccpDistance(point, pointpre)<0.4f)
    {
        m_delegate->joinEditing(this);
    }

    m_touchModel = tNone;
    
    do
    {
        CC_BREAK_IF(m_touchBegin == false);
        
        m_touchBegin = false;
        
        if (this->getActionByTag(0xffff) == NULL)
        {
            DelayTime* delayTime = DelayTime::create(0.1f);
            CallFunc* func = CallFunc::create(this, callfunc_selector(LyfTableViewCell::setNoTouchedEffect));
            Sequence* actions = Sequence::create(delayTime, func, NULL);
            this->runAction(actions);
            actions->setTag(0xffff);
        }
        else
        {
            this->stopActionByTag(0xffff);
        }
    }
    while (0);
}

void LyfTableViewCell::onTouchEnded(Touch *pTouch,Event *pEvent)
{
    
    m_touchBeginUIPoint=Point::ZERO;
    
    Point uipoint=pTouch->getLocationInView();
    Point point=Director::getInstance()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    do
    {
        CC_BREAK_IF(m_touchBegin == false);
        
        m_touchBegin = false;
        
        DelayTime* delayTime = DelayTime::create(0.1f);
        CallFunc* func = CallFunc::create(this, callfunc_selector(LyfTableViewCell::setNoTouchedEffect));
        Sequence* actions = Sequence::create(delayTime, func, NULL);
        this->runAction(actions);
        actions->setTag(0xffff);
    }
    while (0);
    
    if (getRect().containsPoint(point) && m_touchModel==tCell)
    {
        m_delegate->setTouchPitchOn(this);
    }
    
    if (getBtnRect(m_imageView).containsPoint(point) && m_touchModel==tLead)
    {
        m_delegate->setTouchImageView(this);
    }
    
    if (getDelRect().containsPoint(point) && m_touchModel==tDel)
    {
        m_delBtn->setVisible(false);
        this->removeThisAndActions();
    }
}

void LyfTableViewCell::onTouchCancelled(Touch *pTouch,Event *pEvent)
{
    
    this->onTouchEnded(pTouch, pEvent);
}



void LyfTableViewCell::setPriority(int nPriority)
{
    
    m_nTouchPriority=nPriority;
    
//    if (m_IsRehisterTouchDispatcher)
//    {
//        CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//        touchDispatcher->setPriority(m_nTouchPriority, this);
//    }
}

void LyfTableViewCell::setDelegate(LyfTableViewCellDelegate *tableView)
{
    
    m_delegate=tableView;
}

void LyfTableViewCell::setTouchEnabled(bool enabled)
{
    
    m_bIsTouchEnabled=enabled;
    
    if (m_bIsTouchEnabled)
    {
        this->setClickEffect();
    }
    else
    {
        this->setNoClickEffect();
    }
}

bool LyfTableViewCell::getTouchEnabled() const
{
    return m_bIsTouchEnabled;
}


#pragma mark -
#pragma mark 选中特效模块

void LyfTableViewCell::setPitchOn(bool var)
{
    do
    {
        CC_BREAK_IF(m_pitchOn == var);
        m_pitchOn = var;
        
        if (m_pitchOn)
        {
            this->setPitchOnEffect();
        }
        else
        {
            this->setNoPitchOnEffect();
        }
        
    }
    while (0);
}

bool LyfTableViewCell::getPitchOn() const
{
    return m_pitchOn;
}

#pragma mark -
#pragma mark 删除按钮模块

void LyfTableViewCell::showDelBtn()
{
    
    if (m_delBtn) return;
    
    m_delBtn=Sprite::create(DELBTNICON);
    m_delBtn->setAnchorPoint(Point(1.0f, 0.5f));
    m_delBtn->setPosition(DELBTNPOINT);
    this->addChild(m_delBtn);
    
    m_delRect=m_delBtn->getTextureRect();
    m_delBtn->setTextureRect(Rect(m_delRect.origin.x, m_delRect.origin.y, 0, m_delRect.size.height));
    Node* node=Node::create();
    node->setPosition(Point::ZERO);
    m_delBtn->addChild(node);
    
    MoveTo* moveTo=MoveTo::create(0.3f, Point(m_delRect.size.width, 0));
    node->runAction(moveTo);
    this->schedule(schedule_selector(LyfTableViewCell::delShowAction));
}

void LyfTableViewCell::delShowAction(float t)
{
    
    //float width=((Node*)m_delBtn->getChildren()->objectAtIndex(0))->getPositionX();
    float width=((Node*)m_delBtn->getChildren().at(0))->getPositionX();
    
    Rect rect=Rect::ZERO;
    Point point=Point(m_delRect.size.width-width+m_delRect.origin.x, m_delRect.origin.y);
    Size size=Size(width, m_delRect.size.height);
    rect.origin=point;
    rect.size=size;
    m_delBtn->setTextureRect(rect);
    
    if (width==m_delRect.size.width)
    {
        m_delBtn->setContentSize(Size(m_delRect.size.width, m_delRect.size.height));
        this->unschedule(schedule_selector(LyfTableViewCell::delShowAction));
        return;
    }
}

void LyfTableViewCell::delHideAction(float t)
{
    
    float width=((Node*)m_delBtn->getChildren().at(0))->getPositionX();
    
    Rect rect=Rect::ZERO;
    Point point=Point(m_delRect.size.width-width+m_delRect.origin.x,m_delRect.origin.y);
    Size size=Size(width,m_delRect.size.height);
    rect.origin=point;
    rect.size=size;
    m_delBtn->setTextureRect(rect);
    
    if (width==0)
    {
        m_delBtn->setContentSize(Size::ZERO);
        m_delBtn->removeFromParentAndCleanup(true);
        m_delBtn=NULL;
        m_delegate->closeEditing();
        this->unschedule(schedule_selector(LyfTableViewCell::delHideAction));
        return;
    }
}

void LyfTableViewCell::removeDelBtnParent()
{

    if (!m_delBtn) return;

    Node* node=((Node*)m_delBtn->getChildren().at(0));
    node->stopAllActions();
    MoveTo* moveTo=MoveTo::create(0.3f*node->getPositionX()/m_delRect.size.width, Point::ZERO);
    node->runAction(moveTo);
    this->unschedule(schedule_selector(LyfTableViewCell::delShowAction));
    this->schedule(schedule_selector(LyfTableViewCell::delHideAction));
}

void LyfTableViewCell::removeThis(float t){
    
    m_delegate->removeTableCell(this);
}

void LyfTableViewCell::removeThisAndActions()
{
    
    MoveBy* moveby=MoveBy::create(0.15f, Point(-this->getContentSize().width, 0));
    this->runAction(moveby);
    FadeOut* fadeOut=FadeOut::create(0.15f);
    this->runAction(fadeOut);

    this->scheduleOnce(schedule_selector(LyfTableViewCell::removeThis), 0.25f);
}

void LyfTableViewCell::setOpacity(GLubyte opacity)
{
    
    CCSprite::setOpacity(opacity);
    
    if (m_imageView)
    {
        m_imageView->setOpacity(opacity);
    }
    
    if (m_textLabel)
    {
        m_textLabel->setOpacity(opacity);
    }
    
    if (m_detailTextLabel)
    {
        m_detailTextLabel->setOpacity(opacity);
    }
}

bool LyfTableViewCell::isTextTagEqual(const char *text)
{
    
    return (strcmp(m_textTag.c_str(),text)==0);
}

bool LyfTableViewCell::isInTheRangeOfTableView(Point point)
{
    
    Rect rect=Rect::ZERO;
    float width=this->getContentSize().width;
    float height=this->getParent()->getParent()->getContentSize().height;
    rect.size=Size(width, height);
    
    float point_X=point.x;
    float point_Y=point.y+this->getPositionY()+this->getParent()->getPositionY()-this->getContentSize().height/2;
    
    return rect.containsPoint(Point(point_X, point_Y));
}
