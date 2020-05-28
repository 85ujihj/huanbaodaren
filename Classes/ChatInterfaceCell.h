

#ifndef __mir9__ChatInterfaceCell__
#define __mir9__ChatInterfaceCell__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LyfTableViewCell.h"
#include "ChatInterface.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
class ChatInterfaceCell: public LyfTableViewCell
{
    
    CC_SYNTHESIZE(unsigned int, m_lenght, Lenght);
    
    CCLabelTTF* m_chatTypeTTF;
    
    CCLabelTTF* m_speakerTTF;
    
    CCLabelTTF* m_contentTTF;
    
    ChatInfoObject* m_object;

public:
    
    ChatInterfaceCell(unsigned int lenght);
    
    ~ChatInterfaceCell();
    
    static ChatInterfaceCell* create(unsigned int lenght);
    
    void setString(ChatInfoObject& object);
    
};


#endif /* defined(__mir9__ChatInterfaceCell__) */
