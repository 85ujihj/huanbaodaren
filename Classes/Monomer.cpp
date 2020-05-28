//
//  Monomer.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#include "Monomer.h"
#include "GameSocket.h"
#include "GameScene.h"
#include "TextureController.h"
#include "SkillEffectShow.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

Monomer::Monomer()
:m_nMonomer(NULL)
,m_directionType(FDirDown)
,m_stateType(FStateStand)
,m_attackMonomerMajor(NULL)
,m_againstMeSet(NULL)
,m_attackTime(0.5f)
,m_fReactionInterval(0)
,m_isCanMoved(true)
,m_runSpeed(1)
,m_visualRange(5)
,m_isCooling(false)
,m_attackSkillInfo(NULL)
,m_blood(0)
,m_magic(0)
,m_pTheAttack(0)
,m_texturePathType(TexturePathFigure)
,m_currRoleNumber(0)
,m_currWeaponNumber(0)
,m_currEffectID(0)
{
    m_againstMeSet = new std::set<Monomer*>();
    
    m_attackSkillMap = new std::map<unsigned int, AttackSkillInfo*>();
    this->addAttackSkill(1001);
    m_attackSkillInfo = AttackSkillSystem::getAttackSkillInfo(1001);
    
    m_flutteringFairyDeque = new std::deque<FlutteringFairy*>();
}

Monomer::~Monomer()
{
    std::map<unsigned int ,AttackSkillInfo*>::iterator itr;
    for (itr=m_attackSkillMap->begin(); itr!=m_attackSkillMap->end(); itr++)
    {
        unsigned int casterSpecificID = itr->second->getCasterSpecificID();
        TextureController::subSpriteFrames(TexturePathSkillCaster, casterSpecificID);
        unsigned int locusSpecificID = itr->second->getLocusSpecificID();
        TextureController::subSpriteFrames(TexturePathSkillLocus, locusSpecificID);
        unsigned int explosionSpecificID = itr->second->getExplosionSpecificID();
        TextureController::subSpriteFrames(TexturePathSkillExplosion, explosionSpecificID);
    }
    delete m_againstMeSet;
    delete m_attackSkillMap;
    //这里有问题 不能删
     if(m_flutteringFairyDeque)
     {
        // m_flutteringFairyDeque.~deque<FlutteringFairy*>();
        // delete m_flutteringFairyDeque;
        // m_flutteringFairyDeque = nullptr;
     }
    
}

void Monomer::onEnterTransitionDidFinish()
{
    Sprite::onEnterTransitionDidFinish();
    this->showFigure();
}

void Monomer::onExitTransitionDidStart()
{
    Sprite::onExitTransitionDidStart();
}

bool Monomer::init()
{
    if (!Sprite::init())
        return false;
    
    this->setScale(1.0f);
    
    return true;
}

void Monomer::setFigure(TexturePathType type, unsigned int roleNumber)
{
    m_texturePathType = type;
    
    m_currRoleNumber = roleNumber;
    
    if (m_texturePathType != TexturePathFigure)
    {
        m_currWeaponNumber = 0;
    }
    
    if (m_nMonomer)
    {
        this->hideFigure();
        this->showFigure();
    }
}

void Monomer::setHair(unsigned int hairNumber)
{
    do
    {
        CC_BREAK_IF(m_texturePathType != TexturePathFigure);
        
        m_currHairNumber = hairNumber;
        
        if (m_nMonomer)
        {
            m_nMonomer->setHair(m_currHairNumber);
        }
    }
    while (0);
}

void Monomer::setWeapon(unsigned int weaponNumber)
{
    do
    {
        CC_BREAK_IF(m_texturePathType != TexturePathFigure);
        
        m_currWeaponNumber = weaponNumber;
        
        if (m_nMonomer)
        {
            m_nMonomer->setWeapon(m_currWeaponNumber);
        }
    }
    while (0);
}

void Monomer::showFigure()
{
    do
    {
        CC_BREAK_IF(0 == m_currRoleNumber);
        CC_BREAK_IF(m_nMonomer);
        
        m_nMonomer = Figure::create(m_texturePathType, m_currRoleNumber);
        this->setContentSize(m_nMonomer->getContentSize());
        this->setAnchorPoint(m_nMonomer->getAnchorPointWithFoot());
        m_nMonomer->setPosition(this->getContentSize()/2);
        this->addChild(m_nMonomer);
        m_nMonomer->setDelegate(this);
        m_nMonomer->setHair(m_currHairNumber);
        m_nMonomer->setWeapon(m_currWeaponNumber);
        
        m_nMonomer->setFigureState(m_stateType, m_directionType);
        
        Point bloodPoint = Point(this->getContentSize().width * this->getAnchorPoint().x, this->getContentSize().height * 0.8);
        
        bloodBg = Sprite::create("blood_box.png");
        bloodBg->setAnchorPoint(Vec2(0.5, 0.5));
        bloodBg->setPosition(bloodPoint);
        this->addChild(bloodBg);
        bloodBg->setScale(0.3);
        
        bloodIn = Sprite::create("blood_in.png");
        bloodIn->setAnchorPoint(Vec2::ZERO);
        bloodBg->addChild(bloodIn);
        
    }
    while (0);
}

void Monomer::hideFigure()
{
    do
    {
        CC_BREAK_IF(m_nMonomer == NULL);
        m_nMonomer->removeFromParent();
        m_nMonomer = NULL;
        bloodBg->removeFromParent();
    }
    while (0);
}

void Monomer::setFigureState(FigureStateType stateType, FigureDirectionType directionType)
{
    do
    {
        CC_BREAK_IF(m_stateType == FStateDeath);
        
        if (stateType != FStateNone)
        {
            m_stateType = stateType;
        }
        
        if (directionType != FDirNone)
        {
            m_directionType = directionType;
        }
        
        if (m_nMonomer)
        {
            m_nMonomer->setFigureState(stateType, directionType);
        }
    }
    while (0);
    
    
}

Point Monomer::getHandPointRelativeFootOffset()
{
    Point anchorPoint;
    if (m_nMonomer)
    {
        anchorPoint = ccpSub(m_nMonomer->getAnchorPointWithHand(), this->getAnchorPoint());
    }
    else
    {
        anchorPoint = Point(0.5f, 0.5f);
    }
    Point point = Point(this->getContentSize().width * anchorPoint.x, this->getContentSize().height * anchorPoint.y);
    return point;
}

Point Monomer::getHandPoint()
{
    Point handRelativeFoot = this->getHandPointRelativeFootOffset();
    Point point = ccpAdd(this->getPosition(), handRelativeFoot);
    return point;
}

Point Monomer::getHurtPointRelativeFootOffset()
{
    Point anchorPoint;
    if (m_nMonomer)
    {
        anchorPoint = ccpSub(m_nMonomer->getAnchorPointCenter(), this->getAnchorPoint());
    }
    else
    {
        anchorPoint = Point(0.5f, 0.5f);
    }
    Point point = Point(this->getContentSize().width * anchorPoint.x, this->getContentSize().height * anchorPoint.y);
    return point;
}

Point Monomer::getHurtPoint()
{
    Point hurtRelativeFoot = this->getHurtPointRelativeFootOffset();
    Point point = ccpAdd(this->getPosition(), hurtRelativeFoot);
    return point;
}

Monomer* Monomer::getAgainstMeOfFirst()
{
    if (m_againstMeSet->empty())
        return NULL;
    
    return *m_againstMeSet->begin();
}

Rect Monomer::getRect()
{
    int off_x = this->getContentSize().width * 0.4f;
    int off_y = this->getContentSize().height * this->getAnchorPoint().y * 0.5f;
    
    int width = this->getContentSize().width * 0.3f;
    int height = this->getContentSize().height * 0.6f;
    
    Rect rect;
    rect.origin = Point(off_x, off_y);
    rect.size = Size(width, height);
    
    int x = this->getContentSize().width * this->getAnchorPoint().x;
    int y = this->getContentSize().height * this->getAnchorPoint().y;
    
    Point point = Point(x, y);
    point = ccpSub(this->getPosition(), point);
    
    Rect relustRect = rect;
    relustRect.origin = ccpAdd(point, rect.origin);;
    
    return relustRect;
}

int Monomer::getAlphaWithPoint(Point point)
{
    return 0;
}

void Monomer::walkLeft()
{
    this->setFigureState(FStateWalk, FDirLeft);
}

void Monomer::walkRight()
{
    this->setFigureState(FStateWalk, FDirRight);
}

void Monomer::walkUp()
{
    this->setFigureState(FStateWalk, FDirUp);
}

void Monomer::walkDown()
{
    this->setFigureState(FStateWalk, FDirDown);
}

void Monomer::walkLeftAndUp()
{
    this->setFigureState(FStateWalk, FDirLeftAndUp);
}

void Monomer::walkUpAndRight()
{
    this->setFigureState(FStateWalk, FDirUpAndRight);
}

void Monomer::walkRightAndDown()
{
    this->setFigureState(FStateWalk, FDirRightAndDown);
}

void Monomer::walkDownAndLeft()
{
    this->setFigureState(FStateWalk, FDirDownAndLeft);
}

void Monomer::runLeft()
{
    this->setFigureState(FStateRun, FDirLeft);
}

void Monomer::runRight()
{
    this->setFigureState(FStateRun, FDirRight);
}

void Monomer::runUp()
{
    this->setFigureState(FStateRun, FDirUp);
}

void Monomer::runDown()
{
    this->setFigureState(FStateRun, FDirDown);
}

void Monomer::runLeftAndUp()
{
    this->setFigureState(FStateRun, FDirLeftAndUp);
}

void Monomer::runUpAndRight()
{
    this->setFigureState(FStateRun, FDirUpAndRight);
}

void Monomer::runRightAndDown()
{
    this->setFigureState(FStateRun, FDirRightAndDown);
}

void Monomer::runDownAndLeft()
{
    this->setFigureState(FStateRun, FDirDownAndLeft);
}

void Monomer::stand()
{
    this->setFigureState(FStateStand, FDirNone);
}

void Monomer::hurt()
{
    do
    {
        CC_BREAK_IF(getStateType() == FStateDeath);
        this->setFigureState(FStateHurt, FDirNone);
    }
    while (0);
}

void Monomer::death()
{
    do
    {
        CC_BREAK_IF(m_stateType == FStateDeath);
        
        m_nMonomer->stopAllActions();
        m_nMonomer->unscheduleAllSelectors();
        this->stopAllActions();
        this->unscheduleAllSelectors();
        this->setFigureState(FStateDeath, FDirNone);
        this->clearData();
    }
    while (0);
}

void Monomer::playEffect(Node* node, const char *path)
{
    do
    {
        CC_BREAK_IF(strcmp("", path) == 0);
        CC_BREAK_IF(node == NULL);
        
        //CCArray* array = CCArray::createWithCapacity(64);
        Vector<SpriteFrame*>array;
        int flag = 4;
        
        do
        {
            //CCString* string = CCString::createWithFormat("%s%04d.png", path, flag);
            SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s%04d.png", path, flag));
            CC_BREAK_IF(spriteFrame == NULL);
            array.pushBack(spriteFrame);
            flag++;
        }
        while (1);
        
        CC_BREAK_IF(array.size() == 0);
        
        Sprite* sprite = Sprite::createWithSpriteFrame((SpriteFrame*)array.at(0));
        float x = this->getContentSize().width * this->getAnchorPoint().x;
        float y = this->getContentSize().height * 0.65;
        sprite->setPosition(Point(x, y));
        node->addChild(sprite,1000);
        
        Animation* movie = Animation::createWithSpriteFrames(array, 1 / (float)32);
        Animate* animate = Animate::create(movie);
        CallFunc* callFunc = CallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
        Sequence* actions = CCSequence::create(animate, callFunc, NULL);
        sprite->runAction(actions);
    }
    while (0);
}

void Monomer::bloodReturn(float addBlood)
{
    int blood = MIN(this->getBlood() + addBlood, this->getBloodCap());
    this->setBlood(blood);
    this->updateBloodProgress();
    
    FlutteringFairy* fairy = FlutteringFairy::addFairy(this, Point(0, this->getContentSize().height * 0.5), TypeAddBlood, addBlood, this, callfunc_selector(Monomer::flutteringFairyFinish));
    this->pushFlutteringFairy(fairy);
    this->playEffect(this, "addh-");
}

void Monomer::followAttack()
{
    this->stopActionByTag(TAG_FOLLOWATTACK);
    
    do
    {
        if (m_attackMonomerMajor == NULL)
        {
            this->beyondVisualRange();
            break;
        }
        
        if (m_attackMonomerMajor->getStateType() == FStateDeath)
        {
            this->beyondVisualRange();
            break;
        }
        
        if (this->isInTheAttackRange(m_attackMonomerMajor))
        {
            this->attackEnemy();
        }
        else if (this->isInTheFieldOfView(m_attackMonomerMajor))
        {
            this->followTheTracks();
        }
        else
        {
            this->beyondVisualRange();
        }
    }
    while (0);
    
}

void Monomer::followTheTracks()
{
    do
    {
        MapPoint point = m_attackMonomerMajor->ownAttackPoint(this,  m_attackSkillInfo->getAttackDistance());
        CC_BREAK_IF(point.equals(MapPointZero));
        MapPoint position = MapPoint(this->getPosition());
        point = mapSub(point, MapPoint(this->getPosition()));
        MoveInfo relust = this->runBy(point);
        CC_BREAK_IF(relust.time == 0);
        float time = m_fReactionInterval + relust.time + 0.01;
        this->delayCallBack(time, callfunc_selector(Monomer::followAttack))->setTag(TAG_FOLLOWATTACK);
    }
    while (0);
}

bool Monomer::isInTheAttackRange(Monomer *var)
{
    MapPoint point = MapPoint(this->getPosition());
    MapPoint varPoint = MapPoint(var->getPosition());
    
    unsigned int attackRange = m_attackSkillInfo->getAttackDistance();
    
    do
    {
        CC_BREAK_IF(point.getDistance(varPoint) > (M_INT)attackRange);
        CC_BREAK_IF(point.equals(varPoint));
        CC_BREAK_IF(this->isMoveRunning());
        
        return true;
    }
    while (0);
    
    return false;
}

bool Monomer::isInTheFieldOfView(Monomer *var)
{
    MapPoint point = MapPoint(this->getPosition());
    MapPoint varPoint = MapPoint(var->getPosition());
    
    if (point.getDistance(varPoint) <= m_visualRange)
    {
        return true;
    }
    
    return false;
}

void Monomer::beyondVisualRange()
{
    this->setAttackMonomerMajor(NULL);
}

void Monomer::clearData()
{
    m_flutteringFairyDeque->clear();
    
    std::set<Monomer*>::iterator itr;
    for (itr=m_againstMeSet->begin(); itr!=m_againstMeSet->end(); itr++)
    {
        Monomer* mon = (*itr);
        mon->removeAgainstMe(this);
        mon->removeAttackMonomerMajor(this);
    }
    m_againstMeSet->clear();
    
    if (m_attackMonomerMajor)
    {
        m_attackMonomerMajor->removeAgainstMe(this);
        this->setAttackMonomerMajor(NULL);
    }
}

void Monomer::followAttackAndSetAttackMethods(Monomer *otherEnemy, int attackSkillNumber)
{
    if (attackSkillNumber != 0)
    {
        std::map<unsigned int, AttackSkillInfo*>::iterator itr;
        itr = m_attackSkillMap->find(attackSkillNumber);
        if (itr == m_attackSkillMap->end())
            return;
        
        m_attackSkillInfo = itr->second;
    }
    
    do
    {
        CC_BREAK_IF(otherEnemy == NULL);
        CC_BREAK_IF(getStateType() == FStateDeath);
        
        this->setAttackMonomerMajor(otherEnemy);
        this->followAttack();
    }
    while (0);
}

MapPoint Monomer::ownAttackPoint(Monomer *var, M_INT attackRange)
{
    MapPoint relust = MapPointZero;
    
    MapPoint position = MapPoint(this->getPosition());
    MapPoint varPosition = MapPoint(var->getPosition());
    M_INT lenght = MIN(position.getDistance(varPosition), attackRange);
    
    if (attackRange == 1)
    {
        relust = position;
    }
    else
    {
        M_INT x = -lenght, z = -lenght;
        vector<MapPoint> mapVec;
        while (1)
        {
            CC_BREAK_IF(mapVec.size() == 8*lenght);
            
            mapVec.push_back(MapPoint(x, z));
            
            if (mapVec.size() <= 2*lenght)
            {
                x++;
            }
            else if (2*lenght < mapVec.size() && mapVec.size() <= 4*lenght)
            {
                z++;
            }
            else if (4*lenght < mapVec.size() && mapVec.size() <= 6*lenght)
            {
                x--;
            }
            else if (6*lenght < mapVec.size() && mapVec.size() < 8*lenght)
            {
                z--;
            }
        }
        
        int l = 0xffff;
        for (std::vector<MapPoint>::iterator itr=mapVec.begin(); itr!=mapVec.end(); itr++)
        {
            MapPoint p = mapAdd(varPosition, *itr);
            if (!GAME_SCENE->isPointValid(p))
                continue;
            
            if (p.getDistance(position) >= l)
                continue;
            
            relust = p;
            l = p.getDistance(position);
        }
    }
    
    return relust;
}

void Monomer::standAndWatch(MapPoint point)
{
    M_INT lenghtX = mapSub(point, MapPoint(this->getPosition())).x;
    M_INT lenghtY = mapSub(point, MapPoint(this->getPosition())).z;
    float lenght = sqrtf(pow(lenghtX, 2) + pow(lenghtY, 2));
    float pointX = lenghtX / lenght;
    float pointY = lenghtY / lenght;
    float angle_X = acosf(pointX) * 180 / M_PI;
    float angle_Y = acosf(pointY) * 180 / M_PI;
    
    float angle = angle_X;
    if (angle_Y > 90)
    {
        angle = 360 - angle_X;
    }
    
    FigureDirectionType dirType = this->getDirectionType(angle);
    
    this->setFigureState(FStateStand, dirType);
}

bool Monomer::isMoveRunning()
{
    MapPoint position = MapPoint(this->getPosition());
    return (bool)(ccpDistance(position.getCCPointValue(), this->getPosition()) >= 5.0f);
}

CCArray* Monomer::actionsWithPoint(MapPoint& pStart, MapPoint& pFinish)
{
    SEL_CallFunc callfunc = NULL;
    
    CCArray* array = CCArray::create();
    
    if (pStart.equals(pFinish))
        return array;
    
    M_INT lenghtX = pFinish.x - pStart.x;
    M_INT lenghtY = pFinish.z - pStart.z;
    float lenght = sqrtf(lenghtX * lenghtX + lenghtY * lenghtY);
    
    M_INT gridNumber = pStart.getDistance(pFinish);
    
    float time = 0.6f * pStart.getDistance(pFinish) / m_runSpeed / gridNumber;
    
    float pointX = lenghtX / lenght;
    float pointY = lenghtY / lenght;
    
    float angle_X = acosf(pointX) * 180 / M_PI;
    float angle_Y = acosf(pointY) * 180 / M_PI;
    
    float angle = angle_X;
    if (angle_Y > 90)
    {
        angle = 360 - angle_X;
    }
    
    int type = (((int)(angle + 22.5)) % 360 ) / 45.0f;
    
    if (lenght < 2)
    {
        switch (type)
        {
            case 0:
                callfunc = callfunc_selector(Monomer::walkRight);
                break;
            case 1:
                callfunc = callfunc_selector(Monomer::walkUpAndRight);
                break;
            case 2:
                callfunc = callfunc_selector(Monomer::walkUp);
                break;
            case 3:
                callfunc = callfunc_selector(Monomer::walkLeftAndUp);
                break;
            case 4:
                callfunc = callfunc_selector(Monomer::walkLeft);
                break;
            case 5:
                callfunc = callfunc_selector(Monomer::walkDownAndLeft);
                break;
            case 6:
                callfunc = callfunc_selector(Monomer::walkDown);
                break;
            case 7:
                callfunc = callfunc_selector(Monomer::walkRightAndDown);
                break;
            default:
                break;
        }
    }
    else
    {
        switch (type)
        {
            case 0:
                callfunc = callfunc_selector(Monomer::runRight);
                break;
            case 1:
                callfunc = callfunc_selector(Monomer::runUpAndRight);
                break;
            case 2:
                callfunc = callfunc_selector(Monomer::runUp);
                break;
            case 3:
                callfunc = callfunc_selector(Monomer::runLeftAndUp);
                break;
            case 4:
                callfunc = callfunc_selector(Monomer::runLeft);
                break;
            case 5:
                callfunc = callfunc_selector(Monomer::runDownAndLeft);
                break;
            case 6:
                callfunc = callfunc_selector(Monomer::runDown);
                break;
            case 7:
                callfunc = callfunc_selector(Monomer::runRightAndDown);
                break;
            default:
                break;
        }
    }
    
    CallFunc* callFunc = CallFunc::create(this, callfunc);
    array->addObject(callFunc);
    
    MoveTo* moveTo = MoveTo::create(time, pFinish.getCCPointValue());
    array->addObject(moveTo);
    
    return array;
}

CCArray* Monomer::actionsWithMoveTo(std::deque<MapPoint>& pArray)
{
    CCArray* array = CCArray::createWithCapacity(8);
    
    do
    {
        CC_BREAK_IF(pArray.size() <= 1);
        
        CallFunc* callFunc1 = CallFunc::create(this, callfunc_selector(Monomer::scheduleUpdateVertexZ));
        CallFunc* callFunc2 = CallFunc::create(this, callfunc_selector(Monomer::unscheduleUpdateVertexZ));
        
        array->addObject(callFunc1);
        
        MapPoint startPoint = MapPoint(this->getPosition());
        
        for (std::deque<MapPoint>::iterator itr=(pArray.begin()+1); itr!=pArray.end(); itr++)
        {
            MapPoint pStart = *(itr-1);
            MapPoint pFinish = *itr;
            array->addObjectsFromArray(this->actionsWithPoint(pStart, pFinish));
        }
        
        array->addObject(callFunc2);
        
    }
    while (0);
    
    return array;
}

MoveInfo Monomer::runBy(MapPoint point)
{
    MoveInfo relust = {0.0f, MapPoint(this->getPosition()), false};
    
    do
    {
        CC_BREAK_IF(m_isCanMoved == false);
        CC_BREAK_IF(getStateType() == FStateDeath);
        CC_BREAK_IF(getStateType() == FStateAttack);
        CC_BREAK_IF(getStateType() == FStateCaster);
        CC_BREAK_IF(getStateType() == FStateHurt);
        CC_BREAK_IF(this->isMoveRunning());
        
        this->stopActionByTag(TAG_MOVET);
        this->stopActionByTag(TAG_FOLLOWATTACK);
        this->stopActionByTag(TAG_COOLINGTIMEATTACK);
        
        MapPoint position = MapPoint(this->getPosition());
        
        std::deque<MapPoint> pArray = GAME_SCENE->getPathNextRunGrid(position, mapAdd(position, point));
        
        if (pArray.size() <= 1)
        {
            this->stand();
            relust.isCanNotFineTheWay = true;
            break;
        }
        
        CCArray* array = Monomer::actionsWithMoveTo(pArray);
        
        CCCallFunc* moveByBegin = CCCallFunc::create(this, callfunc_selector(Monomer::moveByBegin));
        array->insertObject(moveByBegin, 0);
        
        CCCallFunc* moveByEnd = CCCallFunc::create(this, callfunc_selector(Monomer::moveByEnd));
        array->addObject(moveByEnd);
        //临时转换。。时间紧迫
        Vector<FiniteTimeAction *>arrayVector;
        for (int i = 0; i <array->count(); ++i)
        {
            FiniteTimeAction* pAction=(FiniteTimeAction*)array->getObjectAtIndex(i);
            arrayVector.pushBack(pAction);
        }
        Sequence* actions =Sequence::create(arrayVector);
        actions->setTag(TAG_MOVET);
        this->runAction(actions);
        
        relust.time = actions->getDuration();
        relust.point = pArray.back();
    }
    while (0);
    
    return relust;
}

MoveInfo Monomer::walkBy(MapPoint point)
{
    MoveInfo relust = {0.0f, MapPoint(this->getPosition()), false};
    
    do
    {
        CC_BREAK_IF(m_isCanMoved == false);
        CC_BREAK_IF(getStateType() == FStateDeath);
        CC_BREAK_IF(getStateType() == FStateAttack);
        CC_BREAK_IF(getStateType() == FStateCaster);
        CC_BREAK_IF(getStateType() == FStateHurt);
        CC_BREAK_IF(this->isMoveRunning());
        
        this->stopActionByTag(TAG_MOVET);
        this->stopActionByTag(TAG_FOLLOWATTACK);
        this->stopActionByTag(TAG_COOLINGTIMEATTACK);
        
        MapPoint position = MapPoint(this->getPosition());
        
        std::deque<MapPoint> pArray = GAME_SCENE->getPathNextWalkGrid(position, mapAdd(position, point));
        
        if (pArray.size() <= 1)
        {
            this->stand();
            relust.isCanNotFineTheWay = true;
            break;
        }
        
        CCArray* array = Monomer::actionsWithMoveTo(pArray);
        
        CallFunc* moveByBegin = CallFunc::create(this, callfunc_selector(Monomer::moveByBegin));
        array->insertObject(moveByBegin, 0);
        
        CallFunc* moveByEnd = CallFunc::create(this, callfunc_selector(Monomer::moveByEnd));
        array->addObject(moveByEnd);
        
        //临时转换。。时间紧迫
        Vector<FiniteTimeAction *>arrayVector;
        for (int i = 0; i <array->count(); ++i)
        {
            FiniteTimeAction* pAction=(FiniteTimeAction*)array->getObjectAtIndex(i);
            arrayVector.pushBack(pAction);
        }
        Sequence* actions =Sequence::create(arrayVector);
        
        actions->setTag(TAG_MOVET);
        this->runAction(actions);
        
        relust.time = actions->getDuration();
        relust.point = pArray.back();
        
      
    }
    while (0);
    
    return relust;
}

MoveInfo Monomer::goTo(MapPoint point)
{
    MoveInfo relust = {0.0f, MapPoint(this->getPosition()), false};
    
    do
    {
        CC_BREAK_IF(m_isCanMoved == false);
        CC_BREAK_IF(getStateType() == FStateDeath);
        CC_BREAK_IF(getStateType() == FStateAttack);
        CC_BREAK_IF(getStateType() == FStateCaster);
        CC_BREAK_IF(getStateType() == FStateHurt);
        CC_BREAK_IF(this->isMoveRunning());
        
        this->stopActionByTag(TAG_MOVET);
        this->stopActionByTag(TAG_FOLLOWATTACK);
        this->stopActionByTag(TAG_COOLINGTIMEATTACK);
        
        MapPoint begin = MapPoint(this->getPosition());
        std::deque<MapPoint> pArray = GAME_SCENE->getPath(begin, point);
        
        if (pArray.size() <= 1)
        {
            this->stand();
            relust.isCanNotFineTheWay = true;
            break;
        }
        
        CCArray* array = Monomer::actionsWithMoveTo(pArray);
        
        CallFunc* goBegin = CallFunc::create(this, callfunc_selector(Monomer::goBegin));
        array->insertObject(goBegin, 0);
        
        CallFunc* goEnd = CallFunc::create(this, callfunc_selector(Monomer::goEnd));
        array->addObject(goEnd);
        
        //临时转换。。时间紧迫
        Vector<FiniteTimeAction *>arrayVector;
        for (int i = 0; i <array->count(); ++i)
        {
            FiniteTimeAction* pAction=(FiniteTimeAction*)array->getObjectAtIndex(i);
            arrayVector.pushBack(pAction);
        }
        Sequence* actions =Sequence::create(arrayVector);
        
        actions->setTag(TAG_MOVET);
        this->runAction(actions);
        
        relust.time = actions->getDuration();
        relust.point = pArray.back();
    }
    while (0);
    
    return relust;
}

void Monomer::scheduleUpdateVertexZ()
{
    this->unschedule(schedule_selector(Monomer::updateVertexZ));
    this->schedule(schedule_selector(Monomer::updateVertexZ));
}

void Monomer::unscheduleUpdateVertexZ()
{
    this->unschedule(schedule_selector(Monomer::updateVertexZ));
}

void Monomer::updateVertexZ(float delay)
{
    float value = BgMap::getZOrder(this->getPosition());//z轴
    this->setZOrder(value);
    
    Point point = this->getPosition();
    
    do
    {
        CC_BREAK_IF(m_nMonomer == NULL);
        
        if (GAME_SCENE->getCurrBgMap()->getCurrentGridValue(point) == 2)
        {
            CC_BREAK_IF(m_nMonomer->getOpacity() == 128);
            
            m_nMonomer->setOpacity(128);
            m_nMonomer->setColor(ccGRAY);
        }
        else
        {
            CC_BREAK_IF(m_nMonomer->getOpacity() == 255);
            
            m_nMonomer->setOpacity(255);
            m_nMonomer->setColor(ccWHITE);
        }
    }
    while (0);
}

void Monomer::addAttackSkill(unsigned int skillNumber)
{
    AttackSkillInfo* skillInfo = AttackSkillSystem::getAttackSkillInfo(skillNumber);
    if (skillInfo)
    {
        typedef std::map<unsigned int, AttackSkillInfo*> ::value_type MapType;
        m_attackSkillMap->insert(MapType(skillInfo->getSkillNumber(), skillInfo));
        unsigned int casterSpecificID = skillInfo->getCasterSpecificID();
        TextureController::addSpriteFrames(TexturePathSkillCaster, casterSpecificID);
        unsigned int locusSpecificID = skillInfo->getLocusSpecificID();
        TextureController::addSpriteFrames(TexturePathSkillLocus, locusSpecificID);
        unsigned int explosionSpecificID = skillInfo->getExplosionSpecificID();
        TextureController::addSpriteFrames(TexturePathSkillExplosion, explosionSpecificID);
    }
    
}

void Monomer::subAttackSkill(unsigned int skillNumber)
{
    std::map<unsigned int, AttackSkillInfo*>::iterator itr = m_attackSkillMap->find(skillNumber);
    if (itr != m_attackSkillMap->end())
    {
        unsigned int casterSpecificID = itr->second->getCasterSpecificID();
        TextureController::subSpriteFrames(TexturePathSkillCaster, casterSpecificID);
        unsigned int locusSpecificID = itr->second->getLocusSpecificID();
        TextureController::subSpriteFrames(TexturePathSkillLocus, locusSpecificID);
        unsigned int explosionSpecificID = itr->second->getExplosionSpecificID();
        TextureController::subSpriteFrames(TexturePathSkillExplosion, explosionSpecificID);
    }
}

void Monomer::moveByBegin()
{
    
}

void Monomer::moveByEnd()
{
    this->stand();
}

void Monomer::goBegin()
{
}

void Monomer::goEnd()
{
    this->stand();
}

bool Monomer::attackEnemy()
{
    do
    {
        CC_BREAK_IF(m_attackMonomerMajor == NULL);
        
        CC_BREAK_IF(m_isCooling);
        
        m_isCooling = true;
        
        this->stopActionByTag(TAG_MOVET);
        
        FigureStateType stateType = FStateAttack;
        
        if (m_attackSkillInfo == NULL)
        {
            m_attackSkillInfo = m_attackSkillMap->at(0);
        }
        
        if (m_attackSkillInfo->getAttackType() == 2)
        {
            stateType = FStateCaster;
        }
        
        float lenghtX = m_attackMonomerMajor->getPositionX() - this->getPositionX();
        float lenghtY = (m_attackMonomerMajor->getPositionY() - this->getPositionY()) * sqrtf(2);
        float lenght = sqrtf(lenghtX * lenghtX + lenghtY * lenghtY);
        float pointX = lenghtX / lenght;
        float pointY = lenghtY / lenght;
        
        float angle_X = acosf(pointX) * 180 / M_PI;
        float angle_Y = acosf(pointY) * 180 / M_PI;
        
        float angle = angle_X;
        if (angle_Y > 90)
        {
            angle = 360 - angle_X;
        }
        
        FigureDirectionType dirType = this->getDirectionType(angle);
        
        this->setFigureState(stateType, dirType);
        
        return true;
    }
    while (0);
    
    return false;
}

FigureDirectionType Monomer::getDirectionType(float angle)
{
    FigureDirectionType dirType = FDirNone;
    
    int type = (((int)(angle + 22.5)) % 360 ) / 45.0f;
    
    switch (type)
    {
        case 0:
            dirType = FDirRight;
            break;
        case 1:
            dirType = FDirUpAndRight;
            break;
        case 2:
            dirType = FDirUp;
            break;
        case 3:
            dirType = FDirLeftAndUp;
            break;
        case 4:
            dirType = FDirLeft;
            break;
        case 5:
            dirType = FDirDownAndLeft;
            break;
        case 6:
            dirType = FDirDown;
            break;
        case 7:
            dirType = FDirRightAndDown;
            break;
        default:
            break;
    }
    
    return dirType;
}

void Monomer::setCoolingFalse()
{
    m_isCooling = false;
}

void Monomer::addAgainstMe(Monomer *var, float blood)
{
    do
    {
        CC_BREAK_IF(getStateType() == FStateDeath);
        CC_BREAK_IF(var->getStateType() == FStateDeath);
        
        FlutteringFairy* fairy = FlutteringFairy::addFairy(this, Point(0, this->getContentSize().height * 0.5), TypeSubBlood, m_blood-blood, this, callfunc_selector(Monomer::flutteringFairyFinish));
        this->pushFlutteringFairy(fairy);
        this->setBlood(blood);
        this->updateBloodProgress();
        
        m_againstMeSet->insert(var);
        
        CC_BREAK_IF(m_blood > 0);
        
        this->death();
    }
    while (0);
}

void Monomer::detectionOfEachOther()
{
    
}

void Monomer::pushFlutteringFairy(FlutteringFairy *fairy)
{
    if (m_flutteringFairyDeque->size() >0)
    {
        std::deque<FlutteringFairy*>::iterator itr;
        for (itr=m_flutteringFairyDeque->begin(); itr!=m_flutteringFairyDeque->end(); itr++)
        {
            FlutteringFairy *lastFairy = *itr;
            float y = 30;
            if (y > 0)
            {
                MoveBy* moveBy = MoveBy::create(0.15f, Point(0, y));
                lastFairy->runAction(moveBy);
            }
        }
    }
    m_flutteringFairyDeque->push_back(fairy);
}

void Monomer::flutteringFairyFinish()
{
    m_flutteringFairyDeque->pop_front();
}

void Monomer::detectionWhetherCounter()
{
    
}

void Monomer::updateBloodProgress()
{
    do
    {
        CC_BREAK_IF(bloodBg == NULL);
        
        Rect rect = Rect::ZERO;
        rect.size = Size(bloodBg->getContentSize().width*m_blood/m_bloodCap, bloodIn->getContentSize().height);
        rect.size.width = MAX(rect.size.width, 0);
        bloodIn->setTextureRect(rect);
    }
    while (0);
}

void Monomer::removeAttackMonomerMajor(Monomer *var)
{
    if (var == m_attackMonomerMajor)
    {
        this->setAttackMonomerMajor(NULL);
    }
}

void Monomer::removeAgainstMe(Monomer *var)
{
    m_againstMeSet->erase(var);
}

Action* Monomer::delayCallBack(float time, SEL_CallFunc callFunc)
{
    time = MAX(time, 0.f);
    DelayTime* delayTime = DelayTime::create(time);
    CallFunc* callFunc0 = CallFunc::create(this, callFunc);
    Sequence* actions = Sequence::create(delayTime, callFunc0, NULL);
    this->runAction(actions);
    return actions;
}

void Monomer::attacking()
{
    SkillEffectShow::playSkillEffect(*m_attackSkillInfo, this, m_attackMonomerMajor, 0);
}

void Monomer::underAttack()
{
    this->setFigureState(FStateStand, FDirNone);
}

void Monomer::attackCompleted()
{
    this->setFigureState(FStateStand, FDirNone);
    this->delayCallBack(m_attackTime, callfunc_selector(Monomer::setCoolingFalse));
    this->delayCallBack(m_attackTime, callfunc_selector(Monomer::detectionOfEachOther))->setTag(TAG_FOLLOWATTACK);
}

void Monomer::deathActionFinish()
{
    this->setZOrder(BgMap::getZOrderZero(GAME_SCENE->getCurrBgMap()));//z轴
}
