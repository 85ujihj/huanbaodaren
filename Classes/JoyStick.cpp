#include "JoyStick.h"
using namespace cocos2d;
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
JoyStick::JoyStick(void)
{
	_delegate = NULL;
}

JoyStick::~JoyStick(void)
{
}

JoyStick* JoyStick::createWithFile(const std::string &fileName, float radius)
{
	JoyStick *pRet = new JoyStick();
	if (pRet && pRet->initWithFile(fileName, radius))
	{
        pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool JoyStick::initWithFile(const std::string &filename, float radius)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Sprite::initWithFile(filename));
		
		_radius = radius;
		_direction = Vec2::ZERO;
		_isHeld = false;
		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void JoyStick::onEnterTransitionDidFinish()
{
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    
    EventListenerTouchOneByOne *_touchEventListener=EventListenerTouchOneByOne::create();
    _touchEventListener->onTouchBegan=CC_CALLBACK_2(JoyStick::onTouchBegan, this);
    _touchEventListener->onTouchMoved=CC_CALLBACK_2(JoyStick::onTouchMoved, this);
    _touchEventListener->onTouchEnded=CC_CALLBACK_2(JoyStick::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);
}

void JoyStick::onExit()
{
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void JoyStick::update(float dt)
{
	if (_isHeld)
	{
		_delegate->isHoldingDirection(this, _direction);
	}
}

bool JoyStick::onTouchBegan(Touch *pTouch, Event *unused_event)
{
	Point location = pTouch->getLocation();

	float distanceSQ = ccpDistanceSQ(location, this->getPosition());
	if (distanceSQ <= _radius * _radius)
	{
		this->updateDirectionForTouchLocation(location);
		_isHeld = true;
		return true;
	}
	return false;
}

void JoyStick::onTouchMoved(Touch *pTouch, Event *unused_event)
{
	Point location = pTouch->getLocation();
	this->updateDirectionForTouchLocation(location);
}

void JoyStick::onTouchEnded(Touch *pTouch, Event *unused_event)
{
	_direction = Vec2::ZERO;
	_isHeld = false;
	_delegate->JoyStickTouchEnded(this);
}

void JoyStick::updateDirectionForTouchLocation(Point location)
{
	//radians »¡¶È
	//float radians = ccpToAngle(ccpSub(location, this->getPosition()));
	//degrees ½Ç¶È
	//float degrees =  CC_RADIANS_TO_DEGREES(radians);

    
    Point point = ccpSub(location,this->getPosition());
    Point t_direction = point.getNormalized();
    
	_delegate->didChangeDirectionTo(this, t_direction);
}
