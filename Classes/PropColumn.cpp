

#include "PropColumn.h"
#include "GameScene.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
#define GRID_WIDTH 39
#define GRID_HEIGHT 35
#define PROP_X 88
#define PROP_Y 80
#define COL 10
#define ROW 6

PropColumnMenu::PropColumnMenu()
:m_editProp(NULL)
{
    m_propVec = new PropIconShow*[COL*ROW];
    for (unsigned int i=0; i<COL*ROW; i++)
    {
        m_propVec[i] = NULL;
    }
}

PropColumnMenu::~PropColumnMenu()
{
    delete[] m_propVec;
}

void PropColumnMenu::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    this->updatePropVecPoint();
}

bool PropColumnMenu::init()
{
    if (!Layer::init())
        return false;
    
    this->setTouchEnabled(true);
    
    Sprite* bg = Sprite::create("ui/prop_column.png");
    this->addChild(bg);
    
    m_propColumn = LayerColor::create(ccc4(255, 255, 255, 0), GRID_WIDTH*COL, GRID_HEIGHT*ROW);
    m_propColumn->setContentSize(Size(GRID_WIDTH*COL, GRID_HEIGHT*ROW));
    m_propColumn->setPosition(Point(PROP_X, PROP_Y));
    bg->addChild(m_propColumn);
    
    ControlButton* button = ControlButton::create(Scale9Sprite::create("ui/closed_normal.png"));
    button->setBackgroundSpriteForState(Scale9Sprite::create("ui/closed_selected.png"), Control::State::HIGH_LIGHTED);
    button->setPreferredSize(Size(57, 58));
    button->setPosition(ccpSub(ccpAdd(bg->getPosition(), bg->getContentSize()/2), button->getContentSize()/2));
    this->addChild(button);
    button->addTargetWithActionForControlEvents(GAME_UILAYER, cccontrol_selector(GameInfoUIController::removeSmallMenuAndButton), Control::EventType::TOUCH_UP_INSIDE);
   // button->setTouchPriority(0);
    
    for (int i=0; i<12; i++)
    {
        m_propVec[i] = PropIconShow::create(PropSystem::getPropInfo(i+1));
        m_propColumn->addChild(m_propVec[i]);
    }

    return true;
}

void PropColumnMenu::updatePropVecPoint()
{
    for (unsigned int j=0; j<ROW; j++)
    {
        for (unsigned int i=0; i<COL; i++)
        {
            if (m_propVec[j*COL+i])
            {
                m_propVec[j*COL+i]->setPosition(Point(GRID_WIDTH * (i+0.5f), GRID_HEIGHT * (ROW-j-0.5f)));
            }
        }
    }
}

Rect PropColumnMenu::getPropRect(int i)
{
    Rect rect;
    int x = i % COL;
    int y = i / COL;
    rect.origin = Point(GRID_WIDTH * x, GRID_HEIGHT * (ROW-y-1));
    rect.origin = m_propColumn->convertToWorldSpace(rect.origin);
    rect.size = Size(GRID_WIDTH, GRID_HEIGHT);
    return rect;
}

Point PropColumnMenu::getPropPoint(int i)
{
    int x = i % COL;
    int y = i / COL;
    return Point(GRID_WIDTH * (x+0.5f), GRID_HEIGHT * (ROW-y-0.5f));
}

void PropColumnMenu::propHoming(int i)
{
    if (i < COL*ROW && m_propVec[i])
    {
        Point point = this->getPropPoint(i);
        m_propVec[i]->setPosition(point);
    }
}

void PropColumnMenu::swapProp(int a, int b)
{
    if (a < COL*ROW && b < COL*ROW)
    {
        PropIconShow* tmp = m_propVec[a];
        m_propVec[a] = m_propVec[b];
        m_propVec[b] = tmp;
    }
    
    this->propHoming(a);
    this->propHoming(b);
}

//void PropColumnMenu::registerWithTouchDispatcher()
//{
//    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//	touchDispatcher->addTargetedDelegate(this, 0, true);
//}

bool PropColumnMenu::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    
    Rect rect = Rect::ZERO;
    rect.origin = m_propColumn->convertToWorldSpace(Point::ZERO);
    rect.size = m_propColumn->getContentSize();
    
    if (!rect.containsPoint(point))
        return false;

    for (int i=0; i<COL*ROW; i++)
    {
        if (m_propVec[i] && this->getPropRect(i).containsPoint(point))
        {
            m_propVec[i]->setVisible(false);
            m_editProp = PropIconShow::create(m_propVec[i]->getPropInfo());
            m_editProp->setPosition(point);
            GAME_UILAYER->addChild(m_editProp);
            m_editProp->setTag(i);
            m_editProp->setOpacity(127);
            break;
        }
    }
    
    if (m_editProp == NULL)
        return false;

    return true;
}

void PropColumnMenu::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    
    m_editProp->setPosition(point);
    
    do
    {
        ControlButton* btn = GAME_UILAYER->getOperationMenu()->getDrugsBtn();
        
        Rect rect;
        rect.origin = btn->convertToWorldSpace(Point::ZERO);
        rect.size = btn->getContentSize();
        if (rect.containsPoint(point))
        {
            CC_BREAK_IF(m_editProp->getOpacity() == 255);
            m_editProp->setOpacity(255);
            btn->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.1f);
            btn->runAction(scaleTo);
        }
        else
        {
            CC_BREAK_IF(m_editProp->getOpacity() == 127);
            m_editProp->setOpacity(127);
            btn->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            btn->runAction(scaleTo);
        }
    }
    while (0);
}

void PropColumnMenu::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    
    Rect rect = Rect::ZERO;
    rect.origin = m_propColumn->convertToWorldSpace(Point::ZERO);
    rect.size = m_propColumn->getContentSize();
    
    if (rect.containsPoint(point))
    {
        point = m_propColumn->convertToNodeSpace(point);
        int x = (int)point.x / GRID_WIDTH;
        int y = (int)point.y / GRID_HEIGHT;
        y = ROW - y - 1;
        m_propVec[m_editProp->getTag()]->setVisible(true);
        this->swapProp(m_editProp->getTag(), y*COL+x);
    }
    else
    {
        if (m_editProp->getOpacity() == 255)
        {
            GAME_UILAYER->getOperationMenu()->addDrugs(2001);
            ControlButton* btn = GAME_UILAYER->getOperationMenu()->getDrugsBtn();
            btn->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            btn->runAction(scaleTo);
            m_propVec[m_editProp->getTag()]->removeFromParent();
            m_propVec[m_editProp->getTag()] = NULL;
        }
        else
        {
            float r = CCRANDOM_0_1();
            if (r ==1)
                r = 0;
            
            BgMap* bgMap = GAME_SCENE->getCurrBgMap();
            MapPoint playerPosition = MapPoint(Player::sharePlayer()->getPosition());
            
            MapPoint point = MapPointZero;
            std::vector<MapPoint> mapVec;
            M_INT lenght = 1;
            do
            {
                mapVec = playerPosition.getMapPointVectorForDistance(lenght);
                unsigned int index = 0;
                for (; index<mapVec.size(); index++)
                {
                    if (GAME_SCENE->getMapPointForProp(MapPoint(mapVec.at(index))) == NULL)
                    {
                        point = MapPoint(mapVec.at(index));
                    }
                }
                
                CC_BREAK_IF(!point.equals(MapPointZero));
                
                lenght++;
            }
            while (1);

            
            PropIconShow* show = m_propVec[m_editProp->getTag()];
            m_propVec[m_editProp->getTag()] = NULL;
            show->setVisible(true);
            show->retain();
            show->removeFromParent();
            show->setPosition(point.getCCPointValue());
            bgMap->addChild(show, BgMap::getZOrderZero(bgMap));
            show->release();
            show->setScale(0.8f);

            GAME_SCENE->insterMapPointForProp(show, point);
            
            show->setOpacity(0);
            FadeIn* fadeIn = FadeIn::create(0.1f);
            JumpBy* jumpBy = JumpBy::create(0.3f, Point::ZERO, 30, 1);
            Spawn* spawn = Spawn::create(fadeIn, jumpBy, NULL);
            show->runAction(spawn);
            
        }
    }
    m_editProp->removeFromParent();
    m_editProp = NULL;
}
