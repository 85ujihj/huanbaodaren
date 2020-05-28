

#include "AttackSkillSystem.h"

static AttackSkillSystem* _attackSkillSystem = NULL;

AttackSkillInfo::AttackSkillInfo(rapidjson::Value & json)
:m_skillNumber(json["skillNumber"].GetUint())
,m_attackDistance(json["attackDistance"].GetUint())
,m_skillName(json["skillName"].GetString())
,m_skillType(json["skillType"].GetUint())
,m_attackType(json["attackType"].GetUint())
,m_skillLevel(json["skillLevel"].GetUint())
,m_upgradeTrainingPoint(json["upgradeTrainingPoint"].GetUint())
,m_openLevel(json["openLevel"].GetUint())
,m_magicConsumption(json["magicConsumption"].GetUint())
,m_coolingTime(json["coolingTime"].GetUint())
//,m_isAttackBody(json["isAttackBody"].GetUint())
,m_flightSpeed(json["flightSpeed"].GetUint())
,m_isTailing(json["isTailing"].GetUint())
,m_explosionRadius(json["explosionRadius"].GetUint())
,m_explosionFanAngle(json["explosionFanAngle"].GetUint())
,m_isThirdParty(json["isThirdParty"].GetUint())
,m_effectiveTime(json["effectiveTime"].GetUint())
,m_effectOfCamp(json["effectOfCamp"].GetUint())
,m_casterSpecificID(json["casterSpecificID"].GetUint())
,m_locusSpecificID(json["locusSpecificID"].GetUint())
,m_explosionSpecificID(json["explosionSpecificID"].GetUint())
{
    m_isAttackBody=false;
    printf("AttackSkillInfo\n %d,%s",m_skillNumber, m_skillName.c_str());
}

AttackSkillInfo::~AttackSkillInfo()
{

}

AttackSkillSystem* AttackSkillSystem::shareAttackSkillSystem()
{
    if (_attackSkillSystem == NULL)
    {
        _attackSkillSystem = new AttackSkillSystem();
    }
   
    return _attackSkillSystem;
}

AttackSkillSystem::AttackSkillSystem()
{

    rapidjson::Document m_doc;
    std::string jsonpath = FileUtils::getInstance()->fullPathForFilename("game_data/skill_info.json");
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
    m_doc.Parse<0>(contentStr.c_str());
     printf("%s\n",contentStr.c_str());
    

    if (m_doc.HasParseError()) {
        CCAssert(false, "Json::skill_info.json Reader Parse error!");
    }
    
    
    if (!m_doc.IsNull() && m_doc.IsArray())  //判断是不是数组
    {
        
        for(unsigned int i=0;i<m_doc.Size();++i)
        {
            rapidjson::Value & json=m_doc[i];
            this->addAttackSkillInfo(json);
            
        }
    }

    
}

AttackSkillSystem::~AttackSkillSystem()
{
    map<unsigned int, AttackSkillInfo*>::iterator itr;
    for (itr=m_attackSkillMap.begin(); itr!=m_attackSkillMap.end(); itr++)
    {
        delete itr->second;
        m_attackSkillMap.erase(itr);
    }
    _attackSkillSystem = NULL;
}

void AttackSkillSystem::addAttackSkillInfo(rapidjson::Value & json)
{
    AttackSkillInfo* _attackSkillInfo = new AttackSkillInfo(json);
    
    _attackSkillInfo->m_coolingTime = 1.5f;
    
    unsigned int key = _attackSkillInfo->getSkillNumber();
    m_attackSkillMap[key] = _attackSkillInfo;
}

AttackSkillInfo* AttackSkillSystem::getAttackSkillInfo(const unsigned int skillID)
{
    if (_attackSkillSystem == NULL)
    {
        AttackSkillSystem::shareAttackSkillSystem();
    }
    
    map<unsigned int, AttackSkillInfo*>::iterator itr;
    
    itr = _attackSkillSystem->m_attackSkillMap.find(skillID);
    
    if (itr != _attackSkillSystem->m_attackSkillMap.end())
    {
        return (*itr).second;
    }
    
    return NULL;
}
