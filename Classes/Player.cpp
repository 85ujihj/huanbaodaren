//
//  Player.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//




#include "Player.h"
#include "GameScene.h"
#include "GameLoading.h"
#include "GameSocket.h"
#include "OtherPlayer.h"
#include "Enemy.h"
#include "PlayerController.h"
#include "PropIconShow.h"
#include "SimpleAudioEngine.h"

static Player* _player = NULL;

Player::Player()
:m_isMoveActions(false)
,m_willGoPoint(MapPointZero)
{
    
}

Player::~Player()
{
    
}

Player* Player::sharePlayer()
{
    if (_player == NULL)
    {
        _player = new Player();
        _player->initWithPlayer();
    }
    return _player;
}

void Player::initWithPlayer()
{
    if (!Monomer::init())
        return;
    PlayerController* _playerController = PlayerController::sharePlayerController();
    this->setFigure(TexturePathFigure, _playerController->getPlayerNumber());
    this->setHair(_playerController->getHairNumber());
    this->setWeapon(_playerController->getWeaponsNumber());
    this->setTray();
    this->setBlood(100000);
    this->setBloodCap(100000);
    this->setTheAttack(8000);
    this->setVisualRange(100);
    this->addAttackSkill(1002);
   
    
}

void Player::setTray()
{
    
}

void Player::detectionOfEachOther()
{
    Monomer::detectionOfEachOther();
    
    if (m_attackMonomerMajor)
    {
        this->detectionWhetherCounter();
    }
}

void Player::detectionWhetherCounter()
{
    this->followAttack();
}

MoveInfo Player::runBy(MapPoint point)
{
    MapPoint position = MapPoint(this->getPosition());
    
    MoveInfo relust = Monomer::runBy(point);
    
    if (relust.time != 0)
    {
        GAME_SCENE->insterMapPoint(this, relust.point);
    }
    
    do
    {
        if (relust.time == 0 && relust.isCanNotFineTheWay == false)
        {
            m_willGoPoint = point;
            this->stopActionByTag(TAG_MOVETWAIT);
            this->delayCallBack(1 / 60.0f, callfunc_selector(Player::waitRunBy))->setTag(TAG_MOVETWAIT);
            break;
        }
        
        if (m_isMoveActions == false)
        {
            GAME_SCENE->getCurrBgMap()->schedule(schedule_selector(BgMap::updateMapPosition));
            this->schedule(schedule_selector(Player::detectionReplaceBgMap), 0.1f);
            m_isMoveActions = true;
        }
    }
    while (0);
    
    return relust;
}

void Player::waitRunBy()
{
    this->runBy(m_willGoPoint);
}

MoveInfo Player::walkBy(MapPoint point)
{
    MapPoint position = MapPoint(this->getPosition());
    
    MoveInfo relust = Monomer::walkBy(point);
    
    if (relust.time != 0)
    {
        GAME_SCENE->insterMapPoint(this, relust.point);
    }
    
    do
    {
        if (relust.time == 0 && relust.isCanNotFineTheWay == false)
        {
            m_willGoPoint = point;
            this->stopActionByTag(TAG_MOVETWAIT);
            this->delayCallBack(1 / 60.0f, callfunc_selector(Player::waitWalkBy))->setTag(TAG_MOVETWAIT);
            break;
        }
        
        if (m_isMoveActions == false)
        {
            GAME_SCENE->getCurrBgMap()->schedule(schedule_selector(BgMap::updateMapPosition));
            this->schedule(schedule_selector(Player::detectionReplaceBgMap), 0.1f);
            m_isMoveActions = true;
        }
    }
    while (0);
    
    return relust;
}

void Player::waitWalkBy()
{
    this->walkBy(m_willGoPoint);
}

MoveInfo Player::goTo(MapPoint point)
{
    MapPoint position = MapPoint(this->getPosition());
    
    MoveInfo relust = Monomer::goTo(point);
    
    if (relust.time != 0 && relust.isCanNotFineTheWay == false)
    {
        GAME_SCENE->insterMapPoint(this, relust.point);
    }
    else
    {
        m_willGoPoint = point;
        this->stopActionByTag(TAG_MOVETWAIT);
        this->delayCallBack(1 / 60.0f, callfunc_selector(Player::waitGoTo))->setTag(TAG_MOVETWAIT);
    }
    
    return relust;
}

void Player::waitGoTo()
{
    this->goTo(m_willGoPoint);
}

void Player::moveByEnd()
{
    if (GAME_SCENE->getPlayerMoveActions() == false)
    {
        Monomer::moveByEnd();
    }
    else
    {
        GAME_SCENE->playerRunning();
    }
}

void Player::goBegin()
{
    Monomer::goBegin();
    do
    {
        CC_BREAK_IF(m_isMoveActions);
        
        GAME_SCENE->getCurrBgMap()->schedule(schedule_selector(BgMap::updateMapPosition));
        this->schedule(schedule_selector(Player::detectionReplaceBgMap), 0.1f);
        m_isMoveActions = true;
    }
    while (0);
    
}

void Player::goEnd()
{
    Monomer::goEnd();
    GAME_SCENE->getCurrBgMap()->unschedule(schedule_selector(BgMap::updateMapPosition));
    this->unschedule(schedule_selector(Player::detectionReplaceBgMap));
    m_isMoveActions = false;
}

void Player::stand()
{
    Monomer::stand();
    m_isMoveActions = false;
    this->detectionPropItems();
}

void Player::addAgainstMe(Monomer* var, float blood)
{
    Monomer::addAgainstMe(var, blood);
    if (dynamic_cast<OtherPlayer*>(var))
    {
        this->hurt();
    }
    if (m_attackMonomerMajor == NULL)
    {
        GAME_SCENE->showSelected(var);
    }
}

void Player::CallBackAttackEnemy()
{
    this->stopActionByTag(TAG_COOLINGTIMEATTACK);
    ProgressAutomatic* progress = GAME_UILAYER->getOperationMenu()->getCurrProgress();
    m_isattackEnemy=false;
    do
    {
        CC_BREAK_IF(m_isCooling);
        
        CC_BREAK_IF(Monomer::attackEnemy() == false);
        
        if (progress)
            progress->RunCoolingAction(m_attackSkillInfo->getCoolingTime());
        
        m_isattackEnemy= true;
        return;
    }
    while (0);
    
    this->delayCallBack(0.1f, callfunc_selector(Player::CallBackAttackEnemy))->setTag(TAG_COOLINGTIMEATTACK);
}


bool Player::attackEnemy()
{
    this->CallBackAttackEnemy();
    
    return m_isattackEnemy;
}

void Player::followAttack()
{
    Monomer::followAttack();
}

void Player::death()
{
    Monomer::death();
}

void Player::detectionReplaceBgMap(float delay)
{
    BgMap* bgMap = GAME_SCENE->getCurrBgMap();
    PortalInformation mapPassageway = bgMap->isMapPassageway(this);
    if (!mapPassageway.equals(PortalInformationZero))
    {
        this->goTo(MapPoint(mapPassageway.point));
        GameLoading::runGameLoading(mapPassageway.mapID, mapPassageway.born);
    }
}

void Player::detectionPropItems(float delay)
{
    MapPoint position = MapPoint(this->getPosition());
    
    PropIconShow* show = GAME_SCENE->getMapPointForProp(position);
    if (show)
    {
        GAME_SCENE->eraseMapPointForProp(position);
        show->removeFromParent();
    }
}

void Player::setBlood(int var)
{
    Monomer::setBlood(var);
    if (GAME_SCENE->getGameInfoUIController())
    {
        GAME_SCENE->getGameInfoUIController()->updateBloodPro();
    }
}

void Player::setMagic(int var)
{
    Monomer::setMagic(var);
    if (GAME_SCENE->getGameInfoUIController())
    {
        GAME_SCENE->getGameInfoUIController()->updateMagicPro();
    }
}

void Player::clearData()
{
    Monomer::clearData();
    m_isMoveActions = false;
}

void Player::attacking()
{
    Monomer::attacking();
        switch (m_stateType)
        {
            case FStateAttack:
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/610000054.mp3", false);
            }
                break;
            case FStateCaster:
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/610000017.mp3", false);
            }
                break;
            default:
                break;
        }
}

void Player::underAttack()
{
    
}

void Player::setFigureState(FigureStateType stateType, FigureDirectionType directionType)
{
    Monomer::setFigureState(stateType, directionType);
    
    //    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(m_currEffectID);
    //
    //    switch (stateType)
    //    {
    //        case FStateStand:
    //        {
    //
    //        }
    //            break;
    //        case FStateWalk:
    //        {
    //            m_currEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/default/800701.wav", true);
    //        }
    //            break;
    //        case FStateRun:
    //        {
    //            m_currEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/default/800701.wav", true);
    //        }
    //            break;
    //        case FStateDeath:
    //        {
    //            
    //        }
    //            break;
    //        default:
    //            break;
    //    }
}

//void Player::RockerRun(int dir)
//{
//    //1向右走  2 向上  3 向左  4 向下
//    switch (dir) {
//        case 1:
//        {
//            Monomer::runRight();
//            _player->setPositionX(_player->getPositionX()+1);
//        }
//            break;
//        case 2:
//        {
//           Monomer::runUp();
//            _player->setPositionY(_player->getPositionY()+1);
//        }
//            break;
//        case 3:
//        {
//            Monomer::runLeft();
//            _player->setPositionX(_player->getPositionX()-1);
//        }
//            break;
//        case 4:
//        {
//            Monomer::runDown();
//             _player->setPositionY(_player->getPositionY()-1);
//        }
//            break;
//            
//        
//    }
//    goBegin();
//}
//void Player::RockerEnd()
//{
//    goEnd();
//    stand();
//}
