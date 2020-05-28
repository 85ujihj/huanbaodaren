//
//  Figure.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//
/*
 * 段生辉 20150108
 */
#ifndef __guyue__Figure__
#define __guyue__Figure__

#include "cocos2d.h"
#include "TextureController.h"

#define TAG_ANIMATE 0xfffffff0

using namespace cocos2d;
//方向
typedef enum
{
    FDirUp              = 8,
    FDirLeftAndUp       = 7,
    FDirLeft            = 6,
    FDirDownAndLeft     = 5,
    FDirDown            = 4,
    FDirRightAndDown    = 3,
    FDirRight           = 2,
    FDirUpAndRight      = 1,
    FDirNone            = 0
}FigureDirectionType;
//状态
typedef enum
{
    FStateDeath     = 7,     //死亡
    FStateHurt      = 6,     //受伤
    FStateCaster    = 5,     //施法
    FStateAttack    = 4,     //攻击
    FStateRun       = 3,     //跑
    FStateWalk      = 2,     //走
    FStateStand     = 1,     //站着
    FStateNone      = 0
}FigureStateType;

class Figure;
//接口
class FigureDelegate
{
public:
    //攻击中
    virtual void attacking() = 0;
    //攻击完成
    virtual void attackCompleted() = 0;
    //受到攻击
    virtual void underAttack() = 0;
    //死亡
    virtual void deathActionFinish() = 0;
    //删除Delegate
    void removeDelegate(Figure* var);
};
//角色
class Figure: public Sprite
{
    
    float m_frameRate;//等级
    
    unsigned int m_figureNumber;//编号
    
    unsigned int m_hairNumber;//头像
    
    unsigned int  m_weaponNumber;//武器
    
    TexturePathType m_pathType; //纹理路径
    
    FigureDelegate* m_pDelegate;//代理
    
public:
    
    Figure(TexturePathType type, unsigned int figureNumber);
    
    virtual ~Figure();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    static Figure* create(TexturePathType type, unsigned int figureNumber);
    
    bool init();
    
    void setHair(unsigned int hairNumber);
    
    void setWeapon(unsigned int weaponNumber);
    
    void setFigureState(FigureStateType stateType, FigureDirectionType directionType);
    
    void setDelegate(FigureDelegate* delegate);
    //状态
    CC_SYNTHESIZE_READONLY(FigureStateType, m_stateType, StateType);
    //方向
    CC_SYNTHESIZE_READONLY(FigureDirectionType, m_directionType, DirectionType);
    //头像
    CC_SYNTHESIZE_READONLY(Sprite*, m_hairSprite, HairSprite);
    //武器
    CC_SYNTHESIZE_READONLY(Sprite*, m_weaponSprite, WeaponSprite);
    //脚下－－点
    cocos2d::Vec2 getAnchorPointWithFoot();
    //头－－点
    cocos2d::Vec2 getAnchorPointWithHand();
    //中间－－点
     cocos2d::Vec2 getAnchorPointCenter();
    //透明度
    void setOpacity(GLubyte opacity);
    
private:
    
    void updateFigure();
    
    void runActions();
    
    ActionInterval* getRunActionsFromSprite(Sprite* sprite, std::string& path);
    //------------接口方法------
    void attackCompleted();
    
    void hurtCompleted();
    
    void deathCompleted();
    
    void attacking();
    
};



#endif /* defined(__guyue__Figure__) */
