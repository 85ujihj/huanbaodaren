//
//  HRocker.h
//  guyue
//
//  Created by dsh on 15/1/13.
//
//

#ifndef __guyue__HRocker__
#define __guyue__HRocker__

#include "cocos2d.h"

using namespace cocos2d;

//用于标识摇杆与摇杆的背景
typedef enum{
    tag_rocker,
    tag_rockerBG,
}tagForHRocker;
//用于标识摇杆方向
//typedef enum{
//    FDirUp              = 8,
//    FDirLeftAndUp       = 7,
//    FDirLeft            = 6,
//    FDirDownAndLeft     = 5,
//    FDirDown            = 4,
//    FDirRightAndDown    = 3,
//    FDirRight           = 2,
//    FDirUpAndRight      = 1,
//    FDirNone            = 0
//}tagDirecton;
class HRocker:public Layer
{
public:
    HRocker(void);
    ~HRocker(void);
    //回调方法
    typedef std::function<void(cocos2d::Vec2 pTD)> JoystickHandler;
    //TouchEnded
    void setJoystickPositionChangeHandler(JoystickHandler);
    //void update(float delta);
    //创建摇杆(摇杆的操作题图片资源名，摇杆背景图片资源名，起始坐标)
    static HRocker* createHRocker(const char *rockerImageName,const char *rockerBGImageName,cocos2d::Vec2 position);
    //启动摇杆(显示摇杆、监听摇杆触屏事件)
    void startRocker(bool _isStopOther);
    //停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
    //void stopRocker();
    //判断控制杆方向，用来判断精灵上、下、左、右运动
    int rocketDirection;
    //当前人物行走方向,用来判断精灵的朝向，精灵脸朝右还是朝左
    //bool rocketRun;
    CREATE_FUNC(HRocker);
private:
    JoystickHandler _handler;
    //EventListenerTouchOneByOne* _touchEventListener;
    //自定义初始化函数
    void rockerInit(const char* rockerImageName,const char* rockerBGImageName,cocos2d::Vec2 position);
    //是否可操作摇杆
    bool isCanMove;
    //获取当前摇杆与用户触屏点的角度
    float getRad(cocos2d::Vec2 pos1,cocos2d::Vec2 pos2);
    //摇杆背景的坐标
    cocos2d::Vec2 rockerBGPosition;
    //摇杆背景的半径
    float rockerBGR;
    cocos2d::Vec2 m_sDirection;
    // 当前所按下的方向。这是一个矢量，(-1.0, -1.0)是左下方向，(1.0, 1.0)是右上方向。
    //触屏事件
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    void updateDirectionForTouchLocation(cocos2d::Vec2 location);
    
};


#endif /* defined(__guyue__HRocker__) */
