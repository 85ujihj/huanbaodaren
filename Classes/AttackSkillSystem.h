

#ifndef __mir_mobile__AttackSkillSystem__
#define __mir_mobile__AttackSkillSystem__

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace rapidjson;
/*
 攻击技能 
 封装类
 */
class AttackSkillSystem;
class AttackSkillInfo {
    
    friend class AttackSkillSystem;
   
    //技能编号
    CC_SYNTHESIZE_READONLY(unsigned int, m_skillNumber, SkillNumber);
    
    //技能名称
    CC_SYNTHESIZE_READONLY(string, m_skillName, SkillName);
    
    //技能类型
    CC_SYNTHESIZE_READONLY(unsigned int, m_skillType, SkillType);
    
    //攻击类型
    CC_SYNTHESIZE_READONLY(unsigned int, m_attackType, AttackType);
    
    //技能等级
    CC_SYNTHESIZE_READONLY(unsigned int, m_skillLevel, SkillLevel);
    
    //升级所需修炼点
    CC_SYNTHESIZE_READONLY(unsigned int, m_upgradeTrainingPoint, UpgradeTrainingPoint);
    
    //开启等级
    CC_SYNTHESIZE_READONLY(unsigned int, m_openLevel, OpenLevel);
    
    //魔法消耗
    CC_SYNTHESIZE_READONLY(int, m_magicConsumption, MagicConsumption);
    
    //冷却时间 float
    CC_SYNTHESIZE_READONLY(unsigned int, m_coolingTime, CoolingTime);
    
    //技能团发射距离
    CC_SYNTHESIZE_READONLY(unsigned int, m_attackDistance, AttackDistance);
    
    //技能是否瞄准身体
    CC_SYNTHESIZE_READONLY(bool, m_isAttackBody, IsAttackBody);
    
    //技能团发射速度
    CC_SYNTHESIZE_READONLY(unsigned int, m_flightSpeed, FlightSpeed);
    
    //技能团发射中是否有拖尾
    CC_SYNTHESIZE_READONLY(bool, m_isTailing, IsTailing);
    
    //技能团爆炸半径
    CC_SYNTHESIZE_READONLY(unsigned int, m_explosionRadius, ExplosionRadius);
    
    //技能团爆炸扇形角度
    CC_SYNTHESIZE_READONLY(unsigned int, m_explosionFanAngle, ExplosionFanAngle);
    
    //攻击到目标前是否对第三方起作用
    CC_SYNTHESIZE_READONLY(bool, m_isThirdParty, IsThirdParty);
    
    //BUFF持续时间，常规为0
    CC_SYNTHESIZE_READONLY(unsigned int, m_effectiveTime, EffectiveTime);
    
    //技能有效作用方 己方，对方，全部
    CC_SYNTHESIZE_READONLY(unsigned int, m_effectOfCamp, EffectOfCamp) // 1，2，3
    
    //施法者特效
    CC_SYNTHESIZE_READONLY(unsigned int, m_casterSpecificID, CasterSpecificID);
    
    //轨迹特效
    CC_SYNTHESIZE_READONLY(unsigned int, m_locusSpecificID, LocusSpecificID);
    
    //受击特效
    CC_SYNTHESIZE_READONLY(unsigned int, m_explosionSpecificID, ExplosionSpecificID);
    
public:
    
    AttackSkillInfo(rapidjson::Value & json);
    
    virtual ~AttackSkillInfo();
    
};

class AttackSkillSystem {
    
    map<unsigned int, AttackSkillInfo*> m_attackSkillMap;
    
    void addAttackSkillInfo(rapidjson::Value & json);
    
public:

    AttackSkillSystem();
    
    virtual ~AttackSkillSystem();
    
    static AttackSkillSystem* shareAttackSkillSystem();
    
    static AttackSkillInfo* getAttackSkillInfo(const unsigned int skillID);

};

#endif /* defined(__mir_mobile__AttackSkillSystem__) */
