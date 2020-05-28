

#ifndef __mir9__SkillTableView__
#define __mir9__SkillTableView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;


class SkillTableView:
public Node,
//public TargetedTouchDelegate,
public TableViewDataSource,
public TableViewDelegate
{
   
    //TableViewDelegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
    //TableViewDataSource
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);

    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    //numberOfCellsInTableView(cocos2d::extension::TableView*)
    
    virtual cocos2d::Size cellSizeForTable(TableView *table);
    
    virtual void scrollViewDidScroll(extension::ScrollView* view);
    
    virtual void scrollViewDidZoom(extension::ScrollView* view);
   
   
    
protected:
    
    TableView* m_skillTableView;
    
    std::vector<Sprite*> m_skillVec;
    
    Sprite* m_pEditingSkill;
    
    ControlButton* m_pEditingBtn;
    
public:
    
    SkillTableView();
    
    ~SkillTableView();
    
    CREATE_FUNC(SkillTableView);
    
    bool init();
    
    void onEnter();
    
    void onExit();
    
protected:
    
    void registerWithTouchDispatcher(void);

    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    
	void onTouchEnded(Touch *pTouch, Event *pEvent);
};

#endif /* defined(__mir9__SkillTableView__) */
