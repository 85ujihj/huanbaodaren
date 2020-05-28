

#ifndef __mir_mobile__LOperationMenu__
#define __mir_mobile__LOperationMenu__

#include <iostream>
#include "ProgressAutomatic.h"
#include "extensions/cocos-ext.h" 
USING_NS_CC_EXT;
#define TAG_PROGRESS_SKILL 0xfffffff

class LOperationMenu: public Layer {
    
    bool m_isHangUpAttack;

    int m_attackMethods;

    Node* m_direction;
    
    Node* m_operation;
    
    ControlButton* m_commonAttackBtn;
    
    ControlButton* m_skillAttackBtn[3];
    //药-加血
    CC_SYNTHESIZE_READONLY(ControlButton*, m_drugsBtn, DrugsBtn);
    //传送
    CC_SYNTHESIZE_READONLY(ControlButton*, m_deliveryBtn, DeliveryBtn);
    
    CC_SYNTHESIZE_READONLY(ProgressAutomatic*, m_currProgress, CurrProgress);
    
    CC_SYNTHESIZE_READONLY(int, m_moveMethods, MoveMethods);
    
public:
    
    LOperationMenu();
    
    virtual ~LOperationMenu();
    
    CREATE_FUNC(LOperationMenu);
    
    bool init();
    
    void attack();
    
    void hangUpAttack(Ref* sender, Control::EventType controlEvent);
    
    void moveMethods(Ref* sender, Control::EventType controlEvent);
    
    void intelligentSearch();
    
    void cancelHangUP();
    
    void hideOperationMenu();
    
    void showOperationMenu();
    
    void joinEditState();
    
    void commonAttack();
    
    void commonAttack(Ref* sender, Control::EventType controlEvent);
    
    void skillAttack(Ref* sender, Control::EventType controlEvent);
    
    void useDrugs(Ref* sender, Control::EventType controlEvent);
    
    void usedeliveryProp(Ref* sender, Control::EventType controlEvent);
    
    ControlButton* getSkillAttackBtn(unsigned int number);
    
    void addSkillIcon(ControlButton* btn, int skillNumber);
    
    void addSkillIcon(int btnNumber, int skillNumber);
    
    void addDrugs(int drugsNumber);
    
    void skillAttackCooling(ProgressTimer* progress);
    
};

#endif /* defined(__mir_mobile__LOperationMenu__) */
