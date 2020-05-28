#ifndef SIMPLE_DPAD_H
#define SIMPLE_DPAD_H

#include "cocos2d.h"
using namespace cocos2d;
class SimpleDPad;

class SimpleDPadDelegate
{
public:
	virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::Vec2 direction) = 0;
	virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::Vec2 direction) = 0;
	virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad) = 0;
};

/************************************************************************/
/* 
/* 对于SimpleDPad类，使用了委托模式。
/* 意味着一个委托类(并非SimpleDPad)，将会处理由被委托类(SimpleDPad)启动的任务。
/* 在某些你指定的点上，主要是当涉及到处理任何游戏相关的东西，SimpleDPad将会将职责传递给委托类。
/* 这使得SimpleDPad无需知道任何游戏逻辑，从而允许你在开发任何其他游戏时，可以进行重用 
/* 当SimpleDPad检测到在方向键内的触摸，它会计算触摸的方向，然后发送消息到委托类指明方向。
/* 在这之后的任何事情都不是SimpleDPad所关心的了。为了实施这个模式，SimpleDPad需要至少了解
/* 其委托的有关信息，特别是将触摸方向传递给委托的方法。这是另一种设计模式：协议。
/* 可以看到SimpleDPad的委托定义了所需的方法，在这种方式中，SimpleDPad强制其委托有三个指定的方法，
/* 以便确保每当它想传递东西放到委托中时，它能调用这些方法中的任何一种。
/* 事实上，SimpleDPad也遵循一种协议，即CCTargetedTouchDelegate。
/* 当SimpleDPad被触摸时，进行处理触摸事件，而GameLayer将不会得到触摸。
/* 否则的话，在触摸方向键的时候，英雄就会出拳攻击，显然，这不是希望看到的。
/************************************************************************/
class SimpleDPad : public cocos2d::Sprite
{
public:
	SimpleDPad(void);
	virtual ~SimpleDPad(void);

	static SimpleDPad *dPadWithFile(const char *rockerImageName, const char *rockerBGImageName, cocos2d::Vec2 position);
	virtual bool rockerInit(const char *rockerImageName, const char *rockerBGImageName,
                            cocos2d::Vec2 position);

	void update(float dt);
	// 启动摇杆 （显示摇杆，监听摇杆触屏事件）
	void startRocker(bool _isStopOther);

	// 停止摇杆（隐藏摇杆，取消摇杆的触屏事件）
	void stopRocker();

	void onExit();

	// 获取当前摇杆与用户触屏点的角度
	float getRad(cocos2d::Vec2 pos1, cocos2d::Vec2 pos2);

	// 触屏事件
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void updateDirectionForTouchLocation(cocos2d::Vec2 location);

	CC_SYNTHESIZE(SimpleDPadDelegate*, m_pDelegate, Delegate);				// 委托
	CC_SYNTHESIZE(bool, m_bIsHeld, IsHeld);									// 布尔值表示玩家触摸着方向键。

protected:
	float							m_fRadius;					// 圆形方向键的半径
	cocos2d::Vec2				    m_sDirection;			    // 当前所按下的方向。这是一个矢量，(-1.0, -1.0)是左下方向，(1.0, 1.0)是右上方向。
	bool							m_bIsCanMove;				// 是否可操作摇杆
	cocos2d::Vec2				    m_sRockerBGPosition;		// 摇杆背景的坐标
	float							m_fRockerBGR;				// 摇杆背景的半径

};

//用于标识摇杆与摇杆的背景
typedef enum{
	tag_rocker,
	tag_rockerBG,

}tagForHRocker;

#endif //SIMPLE_DPAD_H
