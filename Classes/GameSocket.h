//
//  GameSocket.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#ifndef __guyue__GameSocket__
#define __guyue__GameSocket__

#include "Monomer.h"

class GameSocket {
    
public:
    
    static void attack(Monomer* one, Monomer* two, int skillNumber);
    
    static void attackGroup(Monomer* one, vector<Monomer*> two, int skillNumber);
    
    static void sendRoleCreate(Node* node, int roleID, const char* nickName);
};

#endif /* defined(__guyue__GameSocket__) */
