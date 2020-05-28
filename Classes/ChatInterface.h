

#ifndef __mir9__ChatInterface__
#define __mir9__ChatInterface__

#include <iostream>
#include <deque>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LyfTableView.h"
#include "LyfTextFieldTTF.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define CHAT_TAG_WORLD 0Xf01
#define CHAT_TAG_GUILD 0Xf02
#define CHAT_TAG_SYSTEM 0Xf03

class ChatInfoObject
{
public:
    
    string nickName;
    
    string content;
    
    unsigned int userID;
    
    unsigned int chatType;
    
    ChatInfoObject(unsigned int chatType, unsigned int userID, string nickName, string content)
    :chatType(chatType)
    ,userID(userID)
    ,nickName(nickName)
    ,content(content)
    {;}
    
    ~ChatInfoObject()
    {;}
};

class ChatInterface:
public LayerColor,
public LyfTableViewDataSource,
public TextFieldDelegate
{
    //初始化cell回调
    virtual LyfTableViewCell* tableCellAtIndex(LyfTableView *table, unsigned int idx);
    
    //获取cell总个数回调
    virtual unsigned int numberfRowsInTableView(LyfTableView *table);
    
    virtual bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF *sender);
    
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
protected:
    
    Node* m_selectedBtn;
    
    Sprite* m_selected;
    
    Scale9Sprite* m_tooBar;
    
    deque<ChatInfoObject>* m_dataWorldArray;
    
    //deque<ChatInfoObject>* m_dataGuildArray;
    
    //deque<ChatInfoObject>* m_dataSystemArray;
    
    deque<ChatInfoObject>* m_dataArray;
    
    LyfTableView* m_worldTableView;
    
    //LyfTableView* m_guildTtableView;
    
    //LyfTableView* m_systemTableView;
    
    LyfTableView* m_tableView;
    
   // LyfTextFieldTTF* m_textField;
    
    ControlButton* m_worldBtn;
    
//    ControlButton* m_guildBtn;
//    
//    ControlButton* m_systemBtn;
//    
//    ControlButton* m_sendNewsBtn;
//    
//    ControlButton* m_switchSizeBtn;
    
protected:
    
    virtual void onEnterTransitionDidFinish();
    
    
    void switchInterface(Ref* sender);
    
    void switchSize(Ref* sender);
    
    void sendNews(Ref* sender);
    
public:
    
    ChatInterface();
    
    ~ChatInterface();
    
    CREATE_FUNC(ChatInterface);
    
    bool init();
    
    void hideChatInterface();
    
    void showChatInterface();
    
protected:
    
    ControlButton* createBtn(const char* title, cocos2d::Vec2 point, int tag);
    
    LyfTableView* createTableView();
    
    void setSizeMin();
    
    void setSizeMax();
    
};




#endif /* defined(__mir9__ChatInterface__) */
