//
//  GameScene.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//
/*
 * 段生辉 20150107
 */

#ifndef __guyue__GameScene__
#define __guyue__GameScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Player.h"
#include "Enemy.h"
#include "OtherPlayer.h"
#include "BgMap.h"
#include "AttackSkillSystem.h"
#include "NpcInfoSystem.h"
#include "PropSystem.h"
#include "GameLoading.h"
#include "LAlertView.h"
#include "CCTouchMouse.h"
#include "GameInfoUIController.h"
#include "PropIconShow.h"
#include "SimpleDPad.h"
//#include "HudLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define GAME_SCENE GameScene::shareGameScene()

class GameScene: public Layer, public BgMapDelegate,public SimpleDPadDelegate
{

    
    cocos2d::Vec2 m_playerDirection;
    
    Sprite* m_selected;
    
    CCTouchMouse* m_touchMouse;
    
    bool m_touchProtected;
    
   
    
    CC_SYNTHESIZE_READONLY(bool, m_isPlayerMoveActions, PlayerMoveActions);
    
    CC_SYNTHESIZE(BgMap*, m_bgMap, CurrBgMap);
    
    CC_SYNTHESIZE_READONLY(GameInfoUIController*, m_gameInfoUIController, GameInfoUIController);
    
    CC_SYNTHESIZE_READONLY(CCDictionary*, m_enemyDictionary, EnemyDictionary);
    
    CC_SYNTHESIZE_READONLY(CCDictionary*, m_otherDictionary, OtherDictionary);
    
    std::map<Monomer*, int> m_gMapPoint;
    
    std::multimap<unsigned int, MapPoint> m_waitReincarnationEnemy;
    
    std::map<int, PropIconShow*> m_gPropMap;
    
    
    //EventListenerTouchOneByOne* _touchEventListener;
    
public:
    
    static Scene* createScene();
    
    GameScene();
    
    virtual ~GameScene();
    
    static GameScene* shareGameScene();
    
    static GameScene* create();
    
    bool init();
    
    void replaceBgMap(int mapID, const int born);
    
    void removeEnemy(Enemy* enemy);
    
    void addEnemy();
    
    vector<Monomer*> getMonmerVecIsLenght(cocos2d::Vec2 point, int lenght);
    
    void OutOfCombat();
    
    void showSelected(Monomer* var);
    
    Monomer* getSelected();
    
    bool isPointValid(const MapPoint& point);
    
    std::deque<MapPoint> getPath(const MapPoint& begin, const MapPoint& end);
    
    std::deque<MapPoint> getPathNextRunGrid(const MapPoint& begin, const MapPoint& end);
    
    std::deque<MapPoint> getPathNextWalkGrid(const MapPoint& begin, const MapPoint& end);
    
    void insterMapPoint(Monomer* var, const MapPoint& point);
    
    void eraseMapPoint(Monomer* var);
    
    bool getMapPoint(const MapPoint& point);
    
    bool insterMapPointForProp(PropIconShow* var, const MapPoint& point);
    
    void eraseMapPointForProp(const MapPoint& point);
    
    PropIconShow* getMapPointForProp(const MapPoint& point);
    
protected:
    
    virtual void onEnterTransitionDidFinish();
    
    //void registerWithTouchDispatcher(void);
    
    void unRegisterWithTouchDispatcher(void);
    
     bool onTouchBegan(Touch *pTouch, Event *pEvent);
     void onTouchMoved(Touch *pTouch, Event *pEvent);
     void onTouchEnded(Touch *pTouch, Event *pEvent);
     void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    void touchProtected();
    
    void untouchProtected();
    
    void updateImageDisplay(cocos2d::Rect& showRect, cocos2d::Rect& hideRect);
    
    void playerMovement(float t);
    
    MapPoint getPlayerDirection();
    
public:
    
    void playerRunning(float delay = 0);
    
    virtual void beginMoveActions();
    
    virtual void stopMoveActions();
    
    virtual void AccurateMoveActions(const MapPoint& point);
    
public:
    
   // virtual void keyBackClicked();
    
   // virtual void keyMenuClicked();
    
    void alertCallBack(const int number);
    
    void log(float dt);
    
    //void HrockerCallBack(Point pTD);
    //实现SimpDPad 接口
    int touchCount;
    CC_SYNTHESIZE(SimpleDPad*, _dPad, DPad);
    //CC_SYNTHESIZE(HudLayer*, _hudLayer, HudLayer);
    virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::Vec2 direction);
    virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::Vec2 direction);
    virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad);
};

#endif /* defined(__guyue__GameScene__) */
