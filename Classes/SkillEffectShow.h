

#ifndef __mir_mobile__SkillEffectShow__
#define __mir_mobile__SkillEffectShow__

#include <iostream>
#include "AttackSkillSystem.h"
#include "Monomer.h"
//技能特效
class SkillEffectShow: public Node {
    
    AttackSkillInfo m_skillInfo;

    Node* m_attacker;//攻击者
    
    Node* m_victim;//被攻击者

    Sprite* m_showSprite;
    
    Sprite* m_explosionSprite;
    
    Scale9Sprite* m_tailSprite;//跟踪

    float m_hitDelay;
    
public:
    
    SkillEffectShow(AttackSkillInfo& skillInfo, float hitDelay);
    
    virtual ~SkillEffectShow();
    
    static SkillEffectShow* playSkillEffect(AttackSkillInfo& skillInfo, Monomer* attacker, Monomer* victim, float hitDelay);
    
    void onEnterTransitionDidFinish();
    
    void onExitTransitionDidStart();
    
protected:

    bool init(Monomer* attacker, Monomer* victim);
    
    void emission(float delay = 0);
    
    void track(float delay = 0);
    
    void hit();

    void playTailing();
    
    void updateTailing(float delay = 0);
    
    void initWithShowSprite();

    void playCasterSpecific(const char *path);
    
    void playLocusSpecific(const char *path);
    
    void playExplosionSpecific(const char *path);
    
    void sendMessage();
    
    void releaseThis();
    
protected:
    
    int getRotationWithLocusSpecific();
    
    inline const cocos2d::Vec2 getDestination();
    
    inline const cocos2d::Vec2 getDeparture();
};

#endif /* defined(__mir_mobile__SkillEffectShow__) */
