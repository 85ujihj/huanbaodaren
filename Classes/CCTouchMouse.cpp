

#include "CCTouchMouse.h"
#include "GameScene.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

CCTouchMouse::CCTouchMouse()
{
    //m_effectArray = new CCArray();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/touch_mouse.plist");
    if (m_effectArray.size() == 0)
    {
        int flag = 1;
        do
        {
            //CCString* string = CCString::createWithFormat("touch_mouse_%02d.png",flag);
            SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("touch_mouse_%02d.png",flag));
            CC_BREAK_IF(spriteFrame == NULL);
            m_effectArray.pushBack(spriteFrame);
            flag++;
        }
        while (1);
    }
}

CCTouchMouse::~CCTouchMouse()
{
    m_effectArray.clear();
    //CC_SAFE_RELEASE_NULL(m_effectArray);
}

void CCTouchMouse::playEffect(const Point& point)
{
    this->setVisible(true);
    this->setPosition(point);
	Animation* movie = Animation::createWithSpriteFrames(m_effectArray, 1 / (float)15);
	Animate* animate = Animate::create(movie);
    CallFunc* finish =CallFunc::create(CC_CALLBACK_0(CCTouchMouse::setHide, this));
    //CCCallFunc::create(this, callfunc_selector(CCTouchMouse::setHide));
    Sequence* actions = Sequence::create(animate, finish, NULL);
    this->stopAllActions();
    this->runAction(actions);
}

void CCTouchMouse::setHide()
{
    this->setVisible(false);
}
