//
//  Figure.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#include "Figure.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect


static float getFrameRate(FigureStateType stateType, TexturePathType type)
{
    float frameRate = 0;
    
    if (type == TexturePathFigure)
    {
        if (stateType == FStateStand)
        {
            frameRate = 1/3.f;
        }
        else if (stateType == FStateWalk)
        {
            frameRate = 0.6/16.f;
        }
        else if (stateType == FStateRun)
        {
            frameRate = 0.6/16.f;
        }
        else if (stateType == FStateAttack)
        {
            frameRate = 1/8.f;
        }
        else if (stateType == FStateCaster)
        {
            frameRate = 1/8.f;
        }
        else if (stateType == FStateHurt)
        {
            frameRate = 1/8.f;
        }
        else if (stateType == FStateDeath)
        {
            frameRate = 1/4.f;
        }
        
    }
    
    if (type == TexturePathMonster)
    {
        if (stateType == FStateStand)
        {
            frameRate = 1/5.f;
        }
        else if (stateType == FStateWalk)
        {
            frameRate = 0.6/8.f;
        }
        else if (stateType == FStateRun)
        {
            frameRate = 0.6f/8.f;
        }
        else if (stateType == FStateAttack)
        {
            frameRate = 1/8.f;
        }
        else if (stateType == FStateCaster)
        {
            frameRate = 1/8.f;
        }
        else if (stateType == FStateHurt)
        {
            frameRate = 1/8.f;
        }
        else if (stateType == FStateDeath)
        {
            frameRate = 1/8.f;
        }
    }
    return frameRate;
}

void FigureDelegate::removeDelegate(Figure* var)
{
    var->setDelegate(NULL);
}

Figure::Figure(TexturePathType type, unsigned int figureNumber)
:m_stateType(FStateStand)
,m_directionType(FDirDown)
,m_frameRate(0.5f)
,m_pathType(type)
,m_figureNumber(figureNumber)
,m_hairNumber(0)
,m_weaponNumber(0)
,m_hairSprite(NULL)
,m_weaponSprite(NULL)
,m_pDelegate(NULL)
{
    TextureController::addSpriteFrames(m_pathType, m_figureNumber, this, callfunc_selector(Figure::updateFigure));
}

Figure::~Figure()
{
    TextureController::subSpriteFrames(m_pathType, m_figureNumber);
    TextureController::subSpriteFrames(TexturePathHair, m_hairNumber);
    TextureController::subSpriteFrames(TexturePathWeapon, m_weaponNumber);
}

void Figure::onEnter()
{
    Sprite::onEnter();
}

void Figure::onExit()
{
    Sprite::onExit();
}

Figure* Figure::create(TexturePathType type, unsigned int figureNumber)
{
    string figurePath = TextureController::getTexturePath(type, figureNumber);
    Figure* figure = new Figure(type, figureNumber);
    if (figure && figure->init())
    {
        figure->autorelease();
        return figure;
    }
    CC_SAFE_DELETE(figure);
    return NULL;
}

bool Figure::init()
{
    if (!Sprite::init())
        return false;
    
    this->setContentSize(Size(256, 256)* 0.8f);
    
   // this->setContentSize(Size(256, 256)* 2);
    
    return true;
}

void Figure::setHair(unsigned int hairNumber)
{
    if (m_hairSprite != NULL)
    {
        m_hairSprite->removeFromParent();
        TextureController::subSpriteFrames(TexturePathHair, m_hairNumber);
        m_hairSprite = NULL;
        m_hairNumber = 0;
    }
    
    do
    {
        CC_BREAK_IF(m_pathType == TexturePathMonster);
        m_hairNumber = hairNumber * 10 + m_figureNumber % 10;
        CC_BREAK_IF(0 == m_hairNumber);
        
        TextureController::addSpriteFrames(TexturePathHair, m_hairNumber);
        
        m_hairSprite = CCSprite::create();
        m_hairSprite->setPosition(Vec2(128, 128) * 0.8f);
        this->addChild(m_hairSprite, 1, 999);
    }
    while (0);
}

void Figure::setWeapon(unsigned int weaponNumber)
{
    if (m_weaponSprite != NULL)
    {
        m_weaponSprite->removeFromParent();
        TextureController::subSpriteFrames(TexturePathWeapon, m_weaponNumber);
        m_weaponSprite = NULL;
        m_weaponNumber = 0;
    }
    
    do
    {
        CC_BREAK_IF(m_pathType == TexturePathMonster);
        m_weaponNumber = weaponNumber;
        CC_BREAK_IF(0 == m_weaponNumber);
        
        TextureController::addSpriteFrames(TexturePathWeapon, m_weaponNumber);
        
        m_weaponSprite = CCSprite::create();
        m_weaponSprite->setPosition(Vec2(128, 128) * 0.8f);
        this->addChild(m_weaponSprite, 0 , 888);
    }
    while (0);
}

void Figure::setFigureState(FigureStateType stateType, FigureDirectionType directionType)
{
    bool isChange = false;
    
    if (stateType != FStateNone && stateType != m_stateType)
    {
        m_stateType = stateType;
        isChange = true;
    }
    if (directionType != FDirNone && directionType != m_directionType)
    {
        m_directionType = directionType;
        isChange = true;
    }
    
    if (isChange)
    {
        this->updateFigure();
    }
}

void Figure::updateFigure()
{
    m_frameRate = getFrameRate(m_stateType, m_pathType);
    
    if (m_directionType == FDirLeft || m_directionType == FDirLeftAndUp || m_directionType == FDirDownAndLeft)
    {
        this->setRotationY(180);
    }
    else
    {
        this->setRotationY(0);
    }
    
    if (m_weaponSprite)
    {
        if ((int)m_directionType == 8)
        {
            m_weaponSprite->setZOrder(-1);
        }
        else
        {
            m_weaponSprite->setZOrder(0);
        }
    }
    
    this->runActions();
}

void Figure::runActions()
{
    this->stopActionByTag(TAG_ANIMATE);
    
    string figurePath = TextureController::getTexturePath(m_pathType, m_figureNumber);
    ActionInterval* animate = this->getRunActionsFromSprite(this, figurePath);
    
    if (m_stateType == FStateAttack)
    {
        DelayTime* delayTime = DelayTime::create(animate->getDuration()/2);
        //CallFunc* callFunc1 = CallFunc::create(this, callfunc_selector(Figure::attacking));
        //CallFunc* callFunc2 = CallFunc::create(this, callfunc_selector(Figure::attackCompleted));
        CallFunc* callFunc1 = CallFunc::create(CC_CALLBACK_0(Figure::attacking, this));
        CallFunc* callFunc2 = CallFunc::create(CC_CALLBACK_0(Figure::attackCompleted, this));
        Sequence* sequence = Sequence::create(delayTime, callFunc1, delayTime, callFunc2, NULL);
        this->runAction(sequence);
        
        animate->setTag(TAG_ANIMATE);
        this->runAction(animate);
    }
    else if (m_stateType == FStateCaster)
    {
        DelayTime* delayTime = DelayTime::create(animate->getDuration()/2);
        CallFunc* callFunc1 = CallFunc::create(CC_CALLBACK_0(Figure::attacking,this));
        CallFunc* callFunc2 = CallFunc::create(CC_CALLBACK_0(Figure::attackCompleted,this));
        Sequence* sequence = Sequence::create(delayTime, callFunc1, delayTime, callFunc2, NULL);
        this->runAction(sequence);
        
        animate->setTag(TAG_ANIMATE);
        this->runAction(animate);
    }
    else if (m_stateType == FStateHurt)
    {
        CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(Figure::hurtCompleted,this));
        Sequence* sequence = Sequence::create(animate, callFunc, NULL);
        this->runAction(sequence);
        sequence->setTag(TAG_ANIMATE);
    }
    else if (m_stateType == FStateDeath)
    {
        CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(Figure::deathCompleted,this));
        Sequence* sequence = Sequence::create(animate, callFunc, NULL);
        this->runAction(sequence);
        
    }
    else
    {
        RepeatForever* repeatForever = RepeatForever::create(animate);
        this->runAction(repeatForever);
        repeatForever->setTag(TAG_ANIMATE);
    }
    
    do
    {
        CC_BREAK_IF(m_hairSprite == NULL);
        
        m_hairSprite->stopAllActions();
        
        string hairPath = TextureController::getTexturePath(TexturePathHair, m_hairNumber);
        ActionInterval* animateHair = this->getRunActionsFromSprite(m_hairSprite, hairPath);
        if ((int)m_stateType > 3)
        {
            m_hairSprite->runAction(animateHair);
        }
        else
        {
            RepeatForever* repeatForever = RepeatForever::create(animateHair);
            m_hairSprite->runAction(repeatForever);
        }
    }
    while (0);
    
    do
    {
        CC_BREAK_IF(m_weaponSprite == NULL);
        
        m_weaponSprite->stopAllActions();
        
        string weaponPath = TextureController::getTexturePath(TexturePathWeapon, m_weaponNumber);
        if (m_stateType == FStateDeath)
        {
            weaponPath = weaponPath + figurePath.at(figurePath.length()-1);
        }
        else
        {
            weaponPath = weaponPath + '0';
        }
        
        ActionInterval* animateWeapon = this->getRunActionsFromSprite(m_weaponSprite, weaponPath);
        if ((int)m_stateType > 3)
        {
            m_weaponSprite->runAction(animateWeapon);
        }
        else
        {
            RepeatForever* repeatForever = RepeatForever::create(animateWeapon);
            m_weaponSprite->runAction(repeatForever);
        }
    }
    while (0);
}

ActionInterval* Figure::getRunActionsFromSprite(Sprite* sprite, std::string& path)
{
    //CCArray* array = CCArray::createWithCapacity(8);
    Vector<SpriteFrame*>array;
    int flag = 0;
    do
    {
        int dirType = m_directionType;
        if (8 > dirType && dirType > 4)
        {
            dirType = 8 - dirType;
        }
        
       // CCString* string = CCString::createWithFormat("%s_%d_%d_%02d.png",path.c_str(), m_stateType, dirType, flag);
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s_%d_%d_%02d.png",path.c_str(), m_stateType, dirType, flag));
        CC_BREAK_IF(spriteFrame == NULL);
        //array->addObject(spriteFrame);
        array.pushBack(spriteFrame);
        flag++;
    }
    while (1);
    
    Animation* movie = Animation::createWithSpriteFrames(array, m_frameRate);
    Animate* animate = Animate::create(movie);
    return animate;
}

void Figure::attacking()
{
    if (m_pDelegate)
    {
        m_pDelegate->attacking();
    }
}

void Figure::attackCompleted()
{
    if (m_pDelegate)
    {
        m_pDelegate->attackCompleted();
    }
}

void Figure::hurtCompleted()
{
    if (m_pDelegate)
    {
        m_pDelegate->underAttack();
    }
}

void Figure::deathCompleted()
{
    if (m_pDelegate)
    {
        m_pDelegate->deathActionFinish();
    }
}

void Figure::setDelegate(FigureDelegate* delegate)
{
    m_pDelegate = delegate;
}

Vec2 Figure::getAnchorPointWithFoot()
{
    float x = this->getContentSize().width/2 / this->getContentSize().width;
    float y = (this->getContentSize().height - 120) / this->getContentSize().height;
    return Vec2(x, y);
}

Vec2 Figure::getAnchorPointWithHand()
{
    float x = this->getContentSize().width/2 / this->getContentSize().width;
    float y = (this->getContentSize().height - 64) / this->getContentSize().height;
    return Vec2(x, y);
}

Vec2 Figure::getAnchorPointCenter()
{
    return Vec2(0.5, 0.5);
}

void Figure::setOpacity(GLubyte opacity)
{
    Sprite::setOpacity(opacity);
    
    if (m_hairSprite)
    {
        m_hairSprite->setOpacity(opacity);
    }
    
    if (m_weaponSprite)
    {
        m_weaponSprite->setOpacity(opacity);
    }
}
