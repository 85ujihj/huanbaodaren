

#include "ChatInterfaceCell.h"
#include "ChatInterface.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

ChatInterfaceCell::ChatInterfaceCell(unsigned int lenght)
:m_lenght(lenght)
,m_chatTypeTTF(NULL)
,m_speakerTTF(NULL)
,m_contentTTF(NULL)
{

}

ChatInterfaceCell::~ChatInterfaceCell()
{

}

ChatInterfaceCell* ChatInterfaceCell::create(unsigned int lenght)
{
    ChatInterfaceCell* cell = new ChatInterfaceCell(lenght);
    if (cell && cell->init())
    {
        cell->setTouchEnabled(false);
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

void ChatInterfaceCell::setString(ChatInfoObject& object)
{
    m_object = &object;
    
    unsigned int ttfSize = 20;
    
    if (m_chatTypeTTF == NULL)
    {
        m_chatTypeTTF = CCLabelTTF::create("", "Arial", ttfSize);
        m_chatTypeTTF->setAnchorPoint(Point(0, 1.0f));
        this->addChild(m_chatTypeTTF);
    }
    if (m_speakerTTF == NULL)
    {
        m_speakerTTF = CCLabelTTF::create("", "Arial", ttfSize);
        m_speakerTTF->setAnchorPoint(Point(0, 1.0f));
        this->addChild(m_speakerTTF);
    }

    string _speaker = string(m_object->nickName);
    if (_speaker.compare("") != 0)
    {
        _speaker = _speaker + "：";
    }
    
    switch (m_object->chatType)
    {
        case CHAT_TAG_GUILD:
        m_chatTypeTTF->setString("【行会】");
        break;
        case CHAT_TAG_SYSTEM:
        {
        m_chatTypeTTF->setString("【系统】");
        _speaker = "";
        }
        break;
        case CHAT_TAG_WORLD:
        m_chatTypeTTF->setString("【世界】");
        break;
        default:
        m_chatTypeTTF->setString("");
        break;
    }
    
    
    m_speakerTTF->setString(_speaker.c_str());
    
    if (m_contentTTF)
    {
        m_contentTTF->removeFromParent();
        m_contentTTF = nullptr;
    }
    unsigned int lenght = m_lenght;
    unsigned int height = ttfSize+1;
    unsigned int row = 1;
    lenght = lenght - m_chatTypeTTF->getContentSize().width;
    if (m_speakerTTF)
    {
        lenght = lenght - m_speakerTTF->getContentSize().width;
    }
    
    LabelTTF* tmp = LabelTTF::create(m_object->content.c_str(), "Arial", ttfSize);
    row = (unsigned int)ceilf(tmp->getContentSize().width/lenght);
    height = MAX(height, row * (tmp->getContentSize().height));
    m_contentTTF = LabelTTF::create(m_object->content.c_str(), "Arial", ttfSize, Size(lenght, height+1), kCCTextAlignmentLeft);
    m_contentTTF->setAnchorPoint(Point(0, 1.0f));
    this->addChild(m_contentTTF);
    
    if (m_object->content.compare("") != 0)
    {
        
    }
    
    if (m_chatTypeTTF)
    {
        m_chatTypeTTF->setPosition(Point(0, height));
    }
    if (m_speakerTTF)
    {
        m_speakerTTF->setPosition(Point(m_chatTypeTTF->getContentSize().width, height));
    }
    if (m_contentTTF)
    {
        m_contentTTF->setPosition(Point(m_chatTypeTTF->getContentSize().width+m_speakerTTF->getContentSize().width, height));
    }
    this->setContentSize(Size(m_lenght, m_contentTTF->getContentSize().height));
    
    ccColor3B color;;
    
    if (m_object->chatType == CHAT_TAG_GUILD)
    {
        color = ccBLUE;
    }
    else if (m_object->chatType == CHAT_TAG_SYSTEM)
    {
        color = ccRED;
    }
    else
    {
        color = ccWHITE;
    }
    
    m_chatTypeTTF->setColor(color);
    m_speakerTTF->setColor(color);
    m_contentTTF->setColor(color);
}
