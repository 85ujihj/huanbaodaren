

#ifndef __mir9__PropIconShow__
#define __mir9__PropIconShow__

#include <iostream>
#include "PropSystem.h"

class PropIconShow: public Sprite {
    
    CC_SYNTHESIZE_READONLY(PropInfo*, m_propInfo, PropInfo);
    
    string m_path;
    
public:
    
    PropIconShow(PropInfo* propInfo);
    
    virtual ~PropIconShow();
    
    static PropIconShow* create(PropInfo* propInfo);
    
};


#endif /* defined(__mir9__PropIconShow__) */
