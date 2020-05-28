#include "SimpleDPad.h"
#include <math.h>
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
using namespace cocos2d;

SimpleDPad::SimpleDPad(void)
{
	m_pDelegate = NULL;
	
}

SimpleDPad::~SimpleDPad(void)
{
	
}

SimpleDPad* SimpleDPad::dPadWithFile(const char *rockerImageName, const char *rockerBGImageName, Point position)
{
	SimpleDPad *pRet = new SimpleDPad();
	if (pRet && pRet->rockerInit(rockerImageName, rockerBGImageName, position))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool SimpleDPad::rockerInit(const char *rockerImageName, const char *rockerBGImageName, Point position)
{
	bool bRet = false;
	do 
	{	// 如果是这两句初始化代码，摇杆位置显示不正确！
		//CC_BREAK_IF(!CCSprite::initWithFile(rockerImageName));
		//CC_BREAK_IF(!CCSprite::initWithFile(rockerBGImageName));
		 
		CC_BREAK_IF(!CCSprite::init());
		Sprite * spRockerBG = Sprite::create(rockerBGImageName);
		CC_BREAK_IF(!spRockerBG);
		spRockerBG->setPosition(position);
		//spRockerBG->setScale(0.5f);
		spRockerBG->setVisible(false);
		this->addChild(spRockerBG, 0, tag_rockerBG);

		Sprite * spRocker = Sprite::create(rockerImageName);
		CC_BREAK_IF(!spRocker);
		spRocker->setPosition(position);
		//spRocker->setScale(0.7f);
		spRocker->setVisible(false);
		this->addChild(spRocker, 1, tag_rocker);

		m_sRockerBGPosition = position;

		m_fRockerBGR = spRockerBG->getContentSize().width*0.3;
		
		
		m_bIsHeld = false;
		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

//启动对摇杆的操作
void SimpleDPad::startRocker(bool _isStopOther)
{
	Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
	rocker->setVisible(true);

	Sprite * rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
	//rockerBG->setVisible(true);

	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, _isStopOther);
    EventListenerTouchOneByOne * _touchEventListener=EventListenerTouchOneByOne::create();
    _touchEventListener->onTouchBegan=CC_CALLBACK_2(SimpleDPad::onTouchBegan, this);
    _touchEventListener->onTouchMoved=CC_CALLBACK_2(SimpleDPad::onTouchMoved, this);
    _touchEventListener->onTouchEnded=CC_CALLBACK_2(SimpleDPad::onTouchEnded, this);
    _touchEventListener->setSwallowTouches(true);
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_touchEventListener,-1);

}

//停止对摇杆的操作
void SimpleDPad::stopRocker()
{
	Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
	rocker->setVisible(false);

	Sprite * rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
	rockerBG->setVisible(false);
}

void SimpleDPad::onExit()
{
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}

// 得到摇杆与触屏点所形成的角度
float SimpleDPad::getRad(Point pos1, Point pos2)
{
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;
	// 得到两点x的距离
	float x = px2 - px1;
	// 得到两点y的距离
	float y = py2 - py1;
	// 计算出斜边
	float xie = sqrt(pow(x, 2) + pow(y, 2));
	// 得到这个角度的余弦值
	float cosAngle = x / xie;
	// 通过反余弦定理获取其角度的弧度
	float rad = acos(cosAngle);
	// 注意：当触屏的位置y坐标 < 摇杆的y坐标，我们要取反值（-0，-180）
	if (py2 < py1)
	{
		rad = -rad;
	}
	return rad;
}

//得到半径为r的圆周运动上一个角度应对应的x、y坐标
Vec2 getAnglePosition(float r,float angle)
{ 
	return  Vec2(r*cos(angle),r*sin(angle));
}

// 当方向键被触摸时，传递方向值到委托类。逐帧检测更新主角色位置
void SimpleDPad::update(float dt)
{
	if (m_bIsHeld)
	{
		m_pDelegate->isHoldingDirection(this, m_sDirection);
	}
}


// 检测触摸位置是否在方向键圆内，如果是，则将isHeld置为true，
// 并更新方向值，返回true以拥有触摸事件优先权。
bool SimpleDPad::onTouchBegan(Touch *pTouch, Event *unused_event)
{
	Vec2 location = pTouch->getLocation();
	Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
	if(rocker->boundingBox().containsPoint(location))
	{
		m_bIsCanMove = true;
		this->updateDirectionForTouchLocation(location);
		m_bIsHeld = true;
		return true;
	}
	return false;
}

// ccTouchMoved当触摸点移动时，更新方向值
void SimpleDPad::onTouchMoved(Touch *pTouch, Event *unused_event)
{
	if(!m_bIsCanMove)
	{
		return;
	}
	Vec2 location = pTouch->getLocation();
	Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);

	//判断两个圆的圆心距是否大于摇杆背景的半径
	if (sqrt(pow((m_sRockerBGPosition.x - location.x), 2) + pow((m_sRockerBGPosition.y - location.y), 2)) >= m_fRockerBGR) 
	{
		//得到摇杆与触屏点所形成的角度
		float angle = getRad(m_sRockerBGPosition, location);

		//保证内部小圆运动的长度限制
		rocker->setPosition(ccpAdd(getAnglePosition(m_fRockerBGR, angle), Point(m_sRockerBGPosition.x, m_sRockerBGPosition.y)));
	}else
	{
		//当没有超过，让摇杆跟随用户触屏点移动即可
		rocker->setPosition(location);
	}

	this->updateDirectionForTouchLocation(location);
}

// 将isHeld置为false，重置方向，并通知委托触摸结束。
void SimpleDPad::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(!m_bIsCanMove)
	{
		return;
	}
	Sprite * rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
	Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
	rocker->stopAllActions();
	rocker->runAction(CCMoveTo::create(0.08f, rockerBG->getPosition()));
	m_bIsCanMove = false;

    m_sDirection = Point::ZERO;
	m_bIsHeld = false;
	m_pDelegate->simpleDPadTouchEnded(this);
}

// 计算触摸点到方向键中心距离值，转换成角度，得到正确的方向值，然后传递值到委托
void SimpleDPad::updateDirectionForTouchLocation(Point location)
{
	float radians = (location-m_sRockerBGPosition).getAngle();
	float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);

	if (degrees <= 22.5 && degrees >= -22.5) 
	{
		//right
		m_sDirection = Point(1.0, 0.0);
	}
	else if (degrees > 22.5 && degrees < 67.5)
	{
		//bottomright
		m_sDirection = Point(1.0, -1.0);
	}
	else if (degrees >= 67.5 && degrees <= 112.5)
	{
		//bottom
		m_sDirection = Point(0.0, -1.0);
	}
	else if (degrees > 112.5 && degrees < 157.5)
	{
		//bottomleft
		m_sDirection = Point(-1.0, -1.0);
	}
	else if (degrees >= 157.5 || degrees <= -157.5)
	{
		//left
		m_sDirection = Point(-1.0, 0.0);
	}
	else if (degrees < -22.5 && degrees > -67.5)
	{
		//topright
		m_sDirection = Point(1.0, 1.0);
	}
	else if (degrees <= -67.5 && degrees >= -112.5)
	{
		//top
		m_sDirection = Point(0.0, 1.0);
	}
	else if (degrees < -112.5 && degrees > -157.5)
	{
		//topleft
		m_sDirection = Point(-1.0, 1.0);
	}
    
   // m_sDirection = (location-this->getPosition()).getNormalized();
	m_pDelegate->didChangeDirectionTo(this, m_sDirection);
}
