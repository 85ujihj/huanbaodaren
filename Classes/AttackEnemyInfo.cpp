

#include "AttackEnemyInfo.h"
#include "GameScene.h"

static int AttackEnemyInfoCount = 0;

AttackEnemyInfo::AttackEnemyInfo()
:m_bloodPro(NULL)
,m_level(NULL)
,m_nickName(NULL)
,m_widget(NULL)
{
   // CCLog("AttackEnemyInfo:%d",AttackEnemyInfoCount++);
}

AttackEnemyInfo::~AttackEnemyInfo()
{
   // CCLog("AttackEnemyInfo:%d",AttackEnemyInfoCount--);
}

bool AttackEnemyInfo::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    m_widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/enemyInfo/enemyInfo.json"));
    if (m_widget)
    {
        this->addChild(m_widget);
    }

    m_nickName =static_cast<ui::Text *>(ui::Helper::seekWidgetByName(m_widget,"Label_EnemyName"));
    if (m_nickName)
    {
        m_nickName->setString("暂无信息");
    }

    m_level = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(m_widget,"enemy_Level"));
    if (m_level)
    {
        m_level->setString("100");
    }
    
    m_bloodPro = dynamic_cast<LoadingBar*>(ui::Helper::seekWidgetByName(m_widget,"LoadingBar_blood"));
    if (m_bloodPro)
    {
        
    }
    
    return true;
}

void AttackEnemyInfo::updateAttackInfo(float delay)
{
    if (GAME_SCENE->getSelected() == NULL)
        return;
    
    if (m_bloodPro)
    {
        int progress = 100*GAME_SCENE->getSelected()->getBlood()/GAME_SCENE->getSelected()->getBloodCap();
        if (progress < 0)
            progress = 0;
        m_bloodPro->setPercent(progress);
    }
}

void AttackEnemyInfo::showAttackInfo(Monomer* sender)
{
    if (GAME_SCENE->getSelected() == NULL)
        return;
        
    if (!this->isVisible())
    {
        this->setVisible(true);
        m_widget->setTouchEnabled(true);
    }
    
    this->schedule(schedule_selector(AttackEnemyInfo::updateAttackInfo));
    
}

void AttackEnemyInfo::hide()
{
    this->unschedule(schedule_selector(AttackEnemyInfo::updateAttackInfo));
    this->setVisible(false);
    m_widget->setTouchEnabled(false);
}