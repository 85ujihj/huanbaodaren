

#ifndef __mir9__GameInfoUIController11111__
#define __mir9__GameInfoUIController11111__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LOperationMenu.h"
#include "GameTabBarMenu.h"
#include "AttackEnemyInfo.h"
#include "ChatInterface.h"
#include "MapThumbnailLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;
#define GAME_UILAYER GAME_SCENE->getGameInfoUIController()

class GameInfoUIController: public Layer {
    
    CC_SYNTHESIZE_READONLY(LOperationMenu*, m_operationMenu, OperationMenu);
    
    CC_SYNTHESIZE_READONLY(GameTabBarMenu*, m_gameTabBarMenu, GameTabBarMenu);
    
    CC_SYNTHESIZE_READONLY(AttackEnemyInfo*, m_attackEnemyInfo, AttackEnemyInfo);
    
    CC_SYNTHESIZE_READONLY(ChatInterface*, m_chatInterface, ChatInterface);
    
    CC_SYNTHESIZE(MapThumbnailMenu*, m_mapThumbnailMenu, MapThumbnailMenu);
    
    std::deque<Node*> m_pMenuSmall;
    
    Node* m_pMenuBig;
    
public:
    
    GameInfoUIController();
    
    virtual ~GameInfoUIController();
    
    CREATE_FUNC(GameInfoUIController);
    
    bool init();
    
    void modeSwitch(Ref* sender = NULL);
    
    void updateBloodPro();
    
    void updateMagicPro();
    
    void updateLevelTTF();
    
    void addSmallMenu(Node* node);
    
    void addBigMenu(Node* node);
    
    void removeSmallMenu(Node* node);
    
    void removeSmallMenuAndButton(Ref* node, Control::EventType controlEvent);
    
    void removeBigMenuAndButton(Ref* node, Control::EventType controlEvent);
    
private:
    
  //  void registerWithTouchDispatcher(void);
    
    void initWithHeadUI();
    
    void initWithMapThumbnail();
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
private:
    
    cocos2d::Size m_winSize;
    
    bool m_isShowTabBar;
    
    ImageView* m_headIcon;
    
    Text* m_nickNameTTF;
    
    TextAtlas* m_levelTTF;
    
    LoadingBar* m_bloodPro;
    
    LoadingBar* m_magicPro;
    
    Button* m_headBtn;
};


#endif /* defined(__mir9__GameInfoUIController__11111) */
