

#ifndef __mir9__ProgressAutomatic__
#define __mir9__ProgressAutomatic__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ProgressAutomatic: public ProgressTimer {
    
    float m_fInterval;
    
protected:
    
    void updateCoolingAction(float t);
    
    void setCoolingFalse();
    
public:
    
    static ProgressAutomatic* create(Sprite* sp);
    
    void RunCoolingAction(float delay);
    
    void RunCoolingNotAction(float delay);
};

#endif /* defined(__mir9__ProgressAutomatic__) */
