

#include "PropSystem.h"

static PropSystem* _propSystem = NULL;

PropInfo::PropInfo(rapidjson::Value & json)
:m_propNumber(json["ID"].GetUint())
,m_iconNumber(json["Icon"].GetUint())
,m_avatarNumber(json["AvatarID"].GetUint())
,m_propName(json["Name"].GetString())
,m_propType(json["Type"].GetUint())
//,m_depict(json["Depict"].GetString())
,m_levelRequirements(json["Nlevel"].GetUint())
,m_attackRequirements(json["Nattack"].GetUint())
,m_magicRequirements(json["Nmaige"].GetUint())
,m_taoismRequirements(json["Ntaoism"].GetUint())
,m_gender(json["Gender"].GetUint())
,m_lasting(json["Lasting"].GetUint())
,m_weight(json["Weight"].GetUint())
,m_specialRequirements(json["Nspecial"].GetUint())
,m_coin(json["Coin"].GetUint())
,m_accurate(json["Accurate"].GetUint())
,m_dodge(json["Dodge"].GetUint())
,m_magicDodge(json["Mdodge"].GetUint())
,m_defenseMax(json["MaxDefense"].GetUint())
,m_defenseMin(json["MinDefense"].GetUint())
,m_magicDefenseMax(json["MaxMDefense"].GetUint())
,m_magicDefenseMin(json["MinMDefense"].GetUint())
,m_attackMax(json["MaxAttack"].GetUint())
,m_attackMin(json["MinAttack"].GetUint())
,m_magicMax(json["MaxMaige"].GetUint())
,m_magicMin(json["MinMaige"].GetUint())
,m_taoismMax(json["MaxTaoism"].GetUint())
,m_taoismMin(json["MinTaoism"].GetUint())
,m_lucky(json["Lucky"].GetUint())
,m_SE(json["SE"].GetUint())
,m_JS(json["JS"].GetUint())
{
   printf("PropSystem\n %d,%s",m_propNumber, m_propName.c_str());
}

PropInfo::~PropInfo()
{

}

PropSystem* PropSystem::sharePropSystem()
{
    if (_propSystem == NULL)
    {
        _propSystem = new PropSystem();
    }
    return _propSystem;
}

PropSystem::PropSystem()
{
    
    rapidjson::Document m_doc;
    std::string jsonpath = FileUtils::getInstance()->fullPathForFilename("game_data/prop_info.json");
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
    m_doc.Parse<0>(contentStr.c_str());
    printf("%s\n",contentStr.c_str());

    

    if (m_doc.HasParseError()) {
        CCAssert(false, "Json::skill_info.json Reader Parse error!");
    }
    
    
    if (!m_doc.IsNull() && m_doc.IsArray())
    {
       
        for(int i=0;i<m_doc.Size();++i)
        {
            rapidjson::Value & json=m_doc[i];
            this->addPropInfo(json);
            
//            printf("%d\n",json["ID"].GetUint());
//            printf("%d\n",json["Icon"].GetUint());
//            printf("%d\n",json["AvatarID"].GetUint());
//            printf("%s\n",json["Name"].GetString());
//            printf("%d\n",json["Type"].GetUint());
//            //printf("%s\n",json["Depict"].GetString());
//            printf("%d\n",json["Nlevel"].GetUint());
//            printf("%d\n",json["Nattack"].GetUint());
//            printf("%d\n",json["Nmaige"].GetUint());
//            printf("%d\n",json["Ntaoism"].GetUint());
//            printf("%d\n",json["Gender"].GetUint());
//            printf("%d\n",json["Lasting"].GetUint());
//            printf("%d\n",json["Weight"].GetUint());
//            printf("%d\n",json["Nspecial"].GetUint());
//            printf("%d\n",json["Coin"].GetUint());
//            printf("%d\n",json["Accurate"].GetUint());
//            printf("%d\n",json["Dodge"].GetUint());
//            printf("%d\n",json["Mdodge"].GetUint());
//            printf("%d\n",json["MaxDefense"].GetUint());
//            printf("%d\n",json["MinDefense"].GetUint());
//            printf("%d\n",json["MaxMDefense"].GetUint());
//            printf("%d\n",json["MinMDefense"].GetUint());
//            printf("%d\n",json["MaxAttack"].GetUint());
//            printf("%d\n",json["MinAttack"].GetUint());
//            printf("%d\n",json["MaxMaige"].GetUint());
//            printf("%d\n",json["MinMaige"].GetUint());
//            printf("%d\n",json["MaxTaoism"].GetUint());
//            printf("%d\n",json["MinTaoism"].GetUint());
//            printf("%d\n",json["Lucky"].GetUint());
//            printf("%d\n",json["SE"].GetUint());
//            printf("%d\n",json["JS"].GetUint());

            
        }
    }
    
}

PropSystem::~PropSystem()
{
    map<unsigned int, PropInfo*>::iterator itr;
    for (itr=m_propMap.begin(); itr!=m_propMap.end(); itr++)
    {
        delete itr->second;
        m_propMap.erase(itr);
    }
    _propSystem = NULL;
}

void PropSystem::addPropInfo(rapidjson::Value & json)
{
    PropInfo* _propInfo = new PropInfo(json);

    unsigned int key = _propInfo->getPropNumber();
    m_propMap[key] = _propInfo;
}

PropInfo* PropSystem::getPropInfo(const unsigned int propID)
{
    if (_propSystem == NULL)
    {
        PropSystem::sharePropSystem();
    }
    
    map<unsigned int, PropInfo*>::iterator itr;
    
    itr = _propSystem->m_propMap.find(propID);
    
    if (itr != _propSystem->m_propMap.end())
    {
        return (*itr).second;
    }
    
    return NULL;
}
