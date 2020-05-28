

#ifndef __mir9__PropSystem__
#define __mir9__PropSystem__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace rapidjson;
class PropInfo {

    CC_SYNTHESIZE_READONLY(unsigned int, m_propNumber, PropNumber);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_iconNumber, IconNumber);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_avatarNumber, AvatarNumber);
    
    CC_SYNTHESIZE_READONLY(string, m_propName, PropName);
    
    CC_SYNTHESIZE_READONLY(char, m_propType, PropType);
    
    CC_SYNTHESIZE_READONLY(string, m_depict, Depict);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_levelRequirements, LevelRequirements);
    
    CC_SYNTHESIZE_READONLY(int, m_attackRequirements, AttackRequirements);
    
    CC_SYNTHESIZE_READONLY(int, m_magicRequirements, MagicRequirements);
    
    CC_SYNTHESIZE_READONLY(int, m_taoismRequirements, TaoismRequirements);
    
    CC_SYNTHESIZE_READONLY(char, m_gender, Gender);
    
    CC_SYNTHESIZE_READONLY(int, m_lasting, Lasting);
    
    CC_SYNTHESIZE_READONLY(int, m_weight, Weight);
    
    CC_SYNTHESIZE_READONLY(char, m_specialRequirements, SpecialRequirements);
    
    CC_SYNTHESIZE_READONLY(int, m_coin, Coin);
    
    CC_SYNTHESIZE_READONLY(int, m_accurate, Accurate);
    
    CC_SYNTHESIZE_READONLY(int, m_dodge, Dodge);
    
    CC_SYNTHESIZE_READONLY(int, m_magicDodge, MagicDodge);
    
    CC_SYNTHESIZE_READONLY(int, m_defenseMax, DefenseMax);
    
    CC_SYNTHESIZE_READONLY(int, m_defenseMin, DefenseMin);
    
    CC_SYNTHESIZE_READONLY(int, m_magicDefenseMax, MagicDefenseMax);
    
    CC_SYNTHESIZE_READONLY(int, m_magicDefenseMin, MagicDefenseMin);
    
    CC_SYNTHESIZE_READONLY(int, m_attackMax, AttackMax);
    
    CC_SYNTHESIZE_READONLY(int, m_attackMin, AttackMin);
    
    CC_SYNTHESIZE_READONLY(int, m_magicMax, MagicMax);
    
    CC_SYNTHESIZE_READONLY(int, m_magicMin, MagicMin);
    
    CC_SYNTHESIZE_READONLY(int, m_taoismMax, TaoismMax);
    
    CC_SYNTHESIZE_READONLY(int, m_taoismMin, TaoismMin);
    
    CC_SYNTHESIZE_READONLY(int, m_lucky, Lucky);
    
    CC_SYNTHESIZE_READONLY(int, m_SE, SE);
    
    CC_SYNTHESIZE_READONLY(int, m_JS, JS);
    
public:
    
    PropInfo(rapidjson::Value & json);
    
    virtual ~PropInfo();
};



class PropSystem {
    
    map<unsigned int, PropInfo*> m_propMap;
    
    void addPropInfo(rapidjson::Value & json);
    
public:
    
    PropSystem();
    
    virtual ~PropSystem();
    
    static PropSystem* sharePropSystem();
    
    static PropInfo* getPropInfo(const unsigned int propID);

};


#endif /* defined(__mir9__PropSystem__) */
