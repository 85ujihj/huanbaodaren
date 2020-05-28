

#include "SkillTableView.h"
#include "GameScene.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
SkillTableView::SkillTableView()
:m_pEditingSkill(NULL)
,m_pEditingBtn(NULL)
{

}

SkillTableView::~SkillTableView()
{

}

void SkillTableView::onEnter()
{
    Node::onEnter();

    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(SkillTableView::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(SkillTableView::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(SkillTableView::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);}

void SkillTableView::onExit()
{
    Node::onExit();
    
}

bool SkillTableView::init()
{
    if (!Node::init())
        return false;

    LayerColor* bg = LayerColor::create(Color4B(255, 255, 255, 127), 450, 500);
    bg->setPosition(bg->getContentSize()/-2);
    this->addChild(bg, -1);
    
    CCLabelTTF* title = CCLabelTTF::create("技能列表", "fonts/Marker Felt.ttf", 40);
    title->setPosition(Point(bg->getContentSize().width/2, bg->getContentSize().height-30));
    bg->addChild(title);
    
    ControlButton* button = ControlButton::create(Scale9Sprite::create("ui/closed_normal.png"));
    button->setBackgroundSpriteForState(Scale9Sprite::create("ui/closed_selected.png"), Control::State::HIGH_LIGHTED);
    button->setPreferredSize(Size(57, 58));
    button->setPosition(ccpSub(ccpAdd(bg->getPosition(), bg->getContentSize()), button->getContentSize()/2));
    this->addChild(button);
    button->addTargetWithActionForControlEvents(GAME_UILAYER, cccontrol_selector(GameInfoUIController::removeSmallMenuAndButton), Control::EventType::TOUCH_UP_INSIDE);
   // button->setTouchPriority(0);
    
    m_skillTableView = TableView::create(this, Size(420, 420));
    m_skillTableView->setPosition(Point(15, 15));
   // m_skillTableView->setDirection(kCCScrollViewDirectionVertical);
    m_skillTableView->setDelegate(this);
   // m_skillTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    bg->addChild(m_skillTableView);
    m_skillTableView->reloadData();
    return true;
}


Size SkillTableView::cellSizeForTable(TableView *table)
{
    return Size(420, 80);
}

TableViewCell* SkillTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    CCString* str = CCString::createWithFormat("ui/skill/skill_%d.png",2001 + (int)idx);
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
        Scale9Sprite* bg = Scale9Sprite::create("ui/cell.png");
        bg->setAnchorPoint(Point::ZERO);
        bg->setPosition(Point::ZERO);
        bg->setPreferredSize(Size(420, 80));
        cell->addChild(bg);
        
        Sprite* skill = Sprite::create(str->getCString());
        skill->setPosition(Point(50, 40));
        cell->addChild(skill, 0, 123);
        m_skillVec.push_back(skill);
    }
    else
    {
        Sprite* skill = (Sprite*)cell->getChildByTag(123);
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(str->getCString());
        skill->setTexture(texture);
    }
    cell->setTag(2001+(int)idx);
    return cell;
}

//unsigned int SkillTableView::numberOfCellsInTableView(TableView *table)
//{
//    return 9;
//}
ssize_t SkillTableView::numberOfCellsInTableView(TableView *table)
{
    return 9;
}

void SkillTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void SkillTableView::scrollViewDidScroll(extension::ScrollView* view)
{

}

void SkillTableView::scrollViewDidZoom(extension::ScrollView* view)
{

}

bool SkillTableView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    
    Rect tableViewRect;
    tableViewRect.origin = m_skillTableView->getParent()->convertToWorldSpace(m_skillTableView->getPosition());
    tableViewRect.size = m_skillTableView->getViewSize();
    
    if (!tableViewRect.containsPoint(point))
        return false;
    
    if (m_skillVec.size() == 0)
        return false;
    
    vector<Sprite*>::iterator itr = m_skillVec.begin();
    for (; itr!=m_skillVec.end(); itr++)
    {
        Size s = (*itr)->getContentSize();
        Point p = (*itr)->getPosition();
        p = (*itr)->getParent()->convertToWorldSpace(p);
        p = ccpSub(p, s/2);
        Rect rect = Rect(p.x, p.y, s.width, s.height);
        if (rect.containsPoint(point))
        {
            m_pEditingSkill = CCSprite::createWithTexture((*itr)->getTexture());
            m_pEditingSkill->setPosition(point);
            GAME_UILAYER->addChild(m_pEditingSkill);
            m_pEditingSkill->setTag((*itr)->getParent()->getTag());
            m_pEditingSkill->setScale(1.5f);
            m_pEditingSkill->setOpacity(127);
            return true;
        }
    }
    return false;
}

void SkillTableView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    
    if (m_pEditingSkill == NULL)
        return;
        
    m_pEditingSkill->setPosition(point);
    
    if (m_pEditingBtn)
    {
        Rect rect;
        rect.origin = m_pEditingBtn->convertToWorldSpace(Point::ZERO);
        rect.size = m_pEditingBtn->getContentSize();
        if (!rect.containsPoint(point))
        {
            m_pEditingSkill->setOpacity(127);
            m_pEditingBtn->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            m_pEditingBtn->runAction(scaleTo);
            m_pEditingBtn = NULL;
        }
    }
    
    if (m_pEditingBtn == NULL)
    {
        for (unsigned int i=0; i<3; i++)
        {
            ControlButton* btn = GAME_UILAYER->getOperationMenu()->getSkillAttackBtn(i);
            Rect rect;
            rect.origin = btn->convertToWorldSpace(Point::ZERO);
            rect.size = btn->getContentSize();
            
            if (rect.containsPoint(point))
            {
                m_pEditingSkill->setOpacity(255);
                m_pEditingBtn = btn;
                m_pEditingBtn->stopAllActions();
                ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.1f);
                m_pEditingBtn->runAction(scaleTo);
                break;
            }
        }
    }
}

void SkillTableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (m_pEditingSkill)
    {
        if (m_pEditingBtn)
        {
            GAME_UILAYER->getOperationMenu()->addSkillIcon(m_pEditingBtn, m_pEditingSkill->getTag());
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            m_pEditingBtn->runAction(scaleTo);
            m_pEditingBtn = NULL;
        }
        m_pEditingSkill->removeFromParent();
        m_pEditingSkill = NULL;
    }
}
