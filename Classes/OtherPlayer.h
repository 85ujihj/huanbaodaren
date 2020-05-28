//
//  OtherPlayer.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#ifndef __guyue__OtherPlayer__
#define __guyue__OtherPlayer__

#include "Monomer.h"

class OtherPlayer: public Monomer {
    
public:
    
    OtherPlayer();
    
    virtual ~OtherPlayer();
    
    CREATE_FUNC(OtherPlayer);
    
    bool initWithOtherPlayer(const char* roleFile, const char* weaponFile){return true;}
    
};

#endif /* defined(__guyue__OtherPlayer__) */
