

#ifndef __mir9__AttackEnemyInfo__
#define __mir9__AttackEnemyInfo__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/cocosgui.h";
#include "cocostudio/CocoStudio.h"
#include "Monomer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;
class AttackEnemyInfo: public Layer {
    
    LoadingBar* m_bloodPro;
    
    TextAtlas* m_level;
    
    Text * m_nickName;
    
    Layout* m_widget;

public:
    
    AttackEnemyInfo();
    
    virtual ~AttackEnemyInfo();
    
    CREATE_FUNC(AttackEnemyInfo);
    
    bool init();
    
    void hide();
    
    void showAttackInfo(Monomer* sender);
    
protected:
    
    void updateAttackInfo(float delay);
};

#endif /* defined(__mir9__AttackEnemyInfo__) */
