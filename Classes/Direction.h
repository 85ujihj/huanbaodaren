
#include "cocos2d.h"
#include "MapPoint.h"
#include "GameScene.h"
using namespace cocos2d;
class Direction;
struct DirectionDelegate
{
    virtual void beginMoveActions() = 0;
    
    virtual void stopMoveActions() = 0;
    
	virtual void AccurateMoveActions(const MapPoint& point) = 0;
};

class Direction : public Node
{
	Sprite* m_backGround;

	Sprite* m_rocker;

    cocos2d::Rect m_rect;

    cocos2d::Vec2 m_point;

    CC_SYNTHESIZE(GameScene*, m_delegate, Delegate);
    
    CC_SYNTHESIZE(float, m_bgRadius, Radius);
    
    CC_SYNTHESIZE(float, m_rockerRadius, rockerRadius);
    
    CC_SYNTHESIZE(float, m_touchEnabled, TouchEnabled);
    
public:

	Direction();

	virtual~Direction();

	void onEnter();

	void onExit();

	static Direction* shareDirection(Sprite* backGround, Sprite* rocker);
    
	bool initWithDirection(Sprite* backGround, Sprite* rocker);

private:

	void registerWithTouchDispatcher(void);

	void unRegisterWithTouchDispatcher(void);

	bool onTouchBegan(Touch *pTouch, Event *pEvent);

	void onTouchMoved(Touch *pTouch, Event *pEvent);

	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void onTouchCancelled(Touch *pTouch, Event *pEvent);

    void setRocker(cocos2d::Vec2 point);

	void readyActions(float delay = 0.0f);
    
    void rockerHoming();

    void touchBegin();
    
    void touchEnd();
    
};

