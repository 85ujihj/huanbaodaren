//
//  GameLoading.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//




#include "GameLoading.h"
#include "GameScene.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

GameLoading* _gameLoading = NULL;

GameLoading::GameLoading(const int mapID, const int born)
:m_mapID(mapID)
,m_born(born)
{
    
}

GameLoading::~GameLoading()
{
    _gameLoading = NULL;
}

void GameLoading::onEnter()
{
    Sprite::onEnter();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->addTargetedDelegate(this, kCCMenuHandlerPriority-999, true);
    
    if (GAME_SCENE == NULL)
    {
        this->scheduleOnce(schedule_selector(GameLoading::initGameScene), 0.5f);
    }
    
    this->scheduleOnce(schedule_selector(GameLoading::updateBgMap), 1.0f);
}

//void GameLoading::onExit()
//{
//    Layer::onExit();
//    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//    //touchDispatcher->removeDelegate(this);
//}

GameLoading* GameLoading::runGameLoading(int mapID, const int born)
{
    if (_gameLoading == NULL) {
        _gameLoading = new GameLoading(mapID, born);
        _gameLoading->init();
        Director::getInstance()->getRunningScene()->addChild(_gameLoading, 100);
        _gameLoading->release();
    }
    return _gameLoading;
}

bool GameLoading::init()
{
    if (!Sprite::init())
        return false;
    
    this->setAnchorPoint(Vec2::ZERO);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    m_sprite = CCSprite::create("map/loading.jpg");
    m_sprite->setPosition(winSize/2);
    this->addChild(m_sprite, -1);
    
    LabelTTF* ttf = LabelTTF::create("加载中……", "Arial", 30);
    ttf->setPosition(Point(winSize.width-70, 20));
    this->addChild(ttf);
    
    if (GAME_SCENE && GAME_SCENE->getCurrBgMap())
    {
        this->setOpacity(0);
        FadeIn* fadeIn = FadeIn::create(0.5f);
        this->runAction(fadeIn);
    }
    
    return true;
}

void GameLoading::initGameScene(float delay)
{
    Director::getInstance()->getRunningScene()->addChild(GameScene::create());
}

void GameLoading::updateBgMap(float delay)
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GAME_SCENE->replaceBgMap(m_mapID, m_born);
    this->updateFinish();
}

void GameLoading::updateFinish()
{
    FadeOut* fadeOut = FadeOut::create(0.3f);
    CallFunc* callFunc = CallFunc::create(this, callfunc_selector(GameLoading::removeFromParent));
    Sequence* actions = Sequence::create(fadeOut, callFunc, NULL);
    this->runAction(actions);
}




//bool GameLoading::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//    
//    return true;
//}

void GameLoading::setOpacity(GLubyte opacity)
{
    //    if (this->getChildren())
    //    {
    //        for (int i=0; i<this->getChildren()->count(); i++)
    //        {
    //            CCNode* node=(CCNode*)this->getChildren()->objectAtIndex(i);
    //
    //            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
    //            {
    //                _children->setOpacity(opacity);
    //            }
    //        }
    //    }
}
