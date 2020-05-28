#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class JoyStick;

class JoyStickDelegate
{
public:
    virtual void didChangeDirectionTo(JoyStick *JoyStick, cocos2d::Vec2 direction) = 0;
	virtual void isHoldingDirection(JoyStick *JoyStick, cocos2d::Vec2 direction) = 0;
	virtual void JoyStickTouchEnded(JoyStick *JoyStick) = 0;
};

class JoyStick : public Sprite
{
public:
	JoyStick(void);
	~JoyStick(void);

	static JoyStick* createWithFile(const std::string &fileName, float radius);
	bool initWithFile(const std::string &filename, float radius);

	void onEnterTransitionDidFinish();
	void onExit();
	void update(float dt);

    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    

	void updateDirectionForTouchLocation(cocos2d::Vec2 location);

	CC_SYNTHESIZE(JoyStickDelegate*, _delegate, Delegate);
	CC_SYNTHESIZE(bool, _isHeld, IsHeld);

protected:
	float _radius;
	cocos2d::Vec2 _direction;
};
