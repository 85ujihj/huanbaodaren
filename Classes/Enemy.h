//
//  Enemy.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#ifndef __guyue__Enemy__
#define __guyue__Enemy__

#include "Monomer.h"

class Enemy: public Monomer {
    
    CC_SYNTHESIZE(MapPoint, m_denPos, DenPos);
    
    CC_SYNTHESIZE_READONLY(int, m_enemyID, EnemyID);
    
    CC_SYNTHESIZE(bool, m_isActive, Active);
    
public:
    
    Enemy();
    
    virtual ~Enemy();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static Enemy* create(int enemyID, unsigned int roleNumber, unsigned int weaponNumber = 0);
    
    bool initWithEnemy(unsigned int roleNumber, unsigned int weaponNumber);
    
    void patrol(float delay = 0);
    
    virtual void death();
    
    virtual void addAgainstMe(Monomer* var, float blood);
    
    void addExp();
    
    virtual void showFigure();
    
    virtual void hideFigure();
    
    virtual MoveInfo runBy(MapPoint point);
    
    virtual MoveInfo walkBy(MapPoint point);
    
    virtual MoveInfo goTo(MapPoint point);
    
    unsigned int getRoleNumber();
    
protected:
    
    virtual void detectionOfEachOther();
    
    virtual void detectionWhetherCounter();
    
    virtual void moveByEnd();
    
    virtual void followAttack();
    
    virtual void followTheTracks();
    
    virtual void beyondVisualRange();
    
    void removeThis();
    
    virtual void attacking();
    
    virtual void underAttack();
    
    virtual void setFigureState(FigureStateType stateType, FigureDirectionType directionType);
    
};


#endif /* defined(__guyue__Enemy__) */
