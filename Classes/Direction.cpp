


#include "Direction.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

static Direction* direction = NULL;

Direction::Direction()
:m_delegate(NULL)
,m_backGround(NULL)
,m_rocker(NULL)
,m_rect(Rect::ZERO)
,m_bgRadius(0)
,m_rockerRadius(0)
,m_touchEnabled(true)
{

}

Direction::~Direction()
{

}

void Direction::onEnter()
{
	CCNode::onEnter();
	//this->registerWithTouchDispatcher();
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(Direction::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(Direction::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(Direction::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Direction::onExit()
{
	CCNode::onExit();
	//this->unRegisterWithTouchDispatcher();
}

//void Direction::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//	touchDispatcher->addTargetedDelegate(this, 0, true);
//}
//
//void Direction::unRegisterWithTouchDispatcher()
//{
//	CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//	touchDispatcher->removeDelegate(this);
//}

Direction* Direction::shareDirection(Sprite* backGround, Sprite* rocker)
{
	if (direction==NULL)
	{
		direction = new Direction();
		if (!direction->initWithDirection(backGround, rocker))
		{
			CC_SAFE_DELETE(direction);
			return NULL;
		}
	}
	return direction;
}

bool Direction::initWithDirection(Sprite* backGround, Sprite* rocker)
{
	if (!Node::init())
	{
		return false;
	}

	if (backGround)
	{
		this->addChild(backGround);
		m_backGround = backGround;
		m_rect.origin = Size(m_backGround->getContentSize() / -2);
		m_rect.size = m_backGround->getContentSize();
	}

	if (rocker)
	{
		this->addChild(rocker);
		m_rocker = rocker;
	}


	return true;
}

bool Direction::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = this->convertToNodeSpace(point);

    if (!m_touchEnabled)
    {
        return false;
    }
	if (!this->isVisible())
	{
		return false;
	}
    if (point.getLength() > m_bgRadius)
    {
        return false;
    }
    
	m_point = point / point.getLength();

	this->setRocker(point);
	this->schedule(schedule_selector(Direction::readyActions));

   // this->scheduleOnce(schedule_selector(Direction::touchBegin), 1/60.f);
    
	return true;
}

void Direction::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = this->convertToNodeSpace(point);
	m_point = point / point.getLength();

	this->setRocker(point);
}

void Direction::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = this->convertToNodeSpace(point);

	m_rocker->stopAllActions();
    MoveTo* moveTo = MoveTo::create(0.2, Point::ZERO);
	EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    m_rocker->runAction(easeBack);
    
    this->rockerHoming();

    m_point = Point::ZERO;
	this->unschedule(schedule_selector(Direction::readyActions));
    
    //this->scheduleOnce(schedule_selector(Direction::touchBegin), 1/60.f);
}



void Direction::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = this->convertToNodeSpace(point);
}

void Direction::setRocker(Point point)
{
	Point rockerPoint = point;
	float lenght = rockerPoint.getLength();
	float lenghtB = m_bgRadius - m_rockerRadius;
	if (lenght > lenghtB)
	{
		rockerPoint.x = rockerPoint.x * lenghtB / lenght;
		rockerPoint.y = rockerPoint.y * lenghtB / lenght;
	}
	m_rocker->setPosition(rockerPoint);
}

void Direction::rockerHoming()
{
}

void Direction::touchBegin()
{
    if (!m_delegate)
		return;

    m_delegate->beginMoveActions();
}

void Direction::touchEnd()
{
    if (!m_delegate)
		return;
    
    m_delegate->stopMoveActions();
}

void Direction::readyActions(float delay)
{
	if (!m_delegate)
		return;

	float angle_X = acosf(m_point.x) * 180 / M_PI;
	float angle_Y = acosf(m_point.y) * 180 / M_PI;
    
    float angle = angle_X;
    if (angle_Y > 90)
    {
        angle = 360 - angle_X;
    }
    angle *= M_PI / 180;
    
    float x = cosf(angle);
    float y = sinf(angle);
    float tan = fabsf(tanf(angle));
    float tanMin = tanf(22.5 * M_PI / 180);
    float tanMax = tanf(67.5 * M_PI / 180);
    MapPoint point;
    if (tanMin <= tan && tan < tanMax)
    {
        point = MapPoint(x / fabsf(x), y / fabsf(y));
    }
    else if (tan < tanMin)
    {
        point = MapPoint(x / fabsf(x), 0);
    }
    else
    {
        point = MapPoint(0, y / fabsf(y));
    }
    
    m_delegate->AccurateMoveActions(point);
}
