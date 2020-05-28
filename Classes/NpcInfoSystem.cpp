

#include "NpcInfoSystem.h"

static NpcInfoSystem* _npcInfoSystem = NULL;

NpcInfo::NpcInfo(rapidjson::Value & json)
:m_nID(json["ID"].GetUint())
,m_nSID(json["SID"].GetUint())
,m_nQID(json["QID"].GetUint())
,m_nRID(json["RID"].GetUint())
,m_sSentence(json["Sentence"].GetString())
,m_sName(json["Name"].GetString())
{
    printf("NpcInfoSystem\n %s,%s",getNpcName().c_str(), json["Sentence"].GetString());
}

NpcInfo::~NpcInfo()
{

}

NpcInfoSystem* NpcInfoSystem::shareNpcInfoSystem()
{
    if (_npcInfoSystem == NULL)
    {
        _npcInfoSystem = new NpcInfoSystem();
    }
    return _npcInfoSystem;
}

NpcInfoSystem::NpcInfoSystem()
{
    rapidjson::Document m_doc;
    std::string jsonpath = FileUtils::getInstance()->fullPathForFilename("game_data/npc_info.json");
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
    m_doc.Parse<0>(contentStr.c_str());
    printf("%s\n",contentStr.c_str());
    

    if (m_doc.HasParseError()) {
        CCAssert(false, "Json::npc_info.json Reader Parse error!");
    }
    
    
    if (!m_doc.IsNull() && m_doc.IsArray()) 
    {
    
        for(int i=0;i<m_doc.Size();++i)
        {
            rapidjson::Value & json=m_doc[i];
            this->addNpcInfo(json);

        }
    }

    
}

NpcInfoSystem::~NpcInfoSystem()
{
    map<unsigned int, NpcInfo*>::iterator itr;
    for (itr=m_npcInfoMap.begin(); itr!=m_npcInfoMap.end(); itr++)
    {
        delete itr->second;
        m_npcInfoMap.erase(itr);
    }
    _npcInfoSystem = NULL;
}

void NpcInfoSystem::addNpcInfo(rapidjson::Value & json)
{
    NpcInfo* _npcInfo = new NpcInfo(json);
    unsigned int key = _npcInfo->getID();
    m_npcInfoMap[key] = _npcInfo;
}

NpcInfo* NpcInfoSystem::getNpcInfo(const unsigned int npcID)
{
    if (_npcInfoSystem == NULL)
    {
        NpcInfoSystem::shareNpcInfoSystem();
    }
    
    map<unsigned int, NpcInfo*>::iterator itr;
    
    itr = _npcInfoSystem->m_npcInfoMap.find(npcID);
    
    if (itr != _npcInfoSystem->m_npcInfoMap.end())
    {
        return (*itr).second;
    }
    
    return NULL;
}

