

#include "SkillEffectShow.h"
#include "TextureController.h"
#include "GameScene.h"
#include "GameSocket.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
//static int SkillEffectShowCount = 0;

SkillEffectShow::SkillEffectShow(AttackSkillInfo& skillInfo, float hitDelay)
:m_skillInfo(skillInfo)
,m_attacker(NULL)
,m_victim(NULL)
,m_showSprite(NULL)
,m_explosionSprite(NULL)
,m_tailSprite(NULL)
,m_hitDelay(hitDelay)
{
    //CCLog("SkillEffectShow:%d",++SkillEffectShowCount);
    m_attacker =Node::create();
    m_victim =Node::create();
  
}

SkillEffectShow::~SkillEffectShow()
{
    //CCLog("SkillEffectShow:%d",--SkillEffectShowCount);
}

void SkillEffectShow::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();
}

void SkillEffectShow::onExitTransitionDidStart()
{
    Node::onExitTransitionDidStart();
    if (m_attacker->getParent())
    {
        m_attacker->removeFromParent();
    }
    if (m_victim->getParent())
    {
        m_victim->removeFromParent();
    }
}

SkillEffectShow* SkillEffectShow::playSkillEffect(AttackSkillInfo& skillInfo, Monomer* attacker, Monomer* victim, float hitDelay)
{
    SkillEffectShow* skillEffectShow = new SkillEffectShow(skillInfo, hitDelay);
    if (skillEffectShow && skillEffectShow->init(attacker, victim))
    {
        GAME_SCENE->getCurrBgMap()->addChild(skillEffectShow);
        skillEffectShow->release();
        return skillEffectShow;
    }
    CC_SAFE_DELETE(skillEffectShow);
    return NULL;
}

bool SkillEffectShow::init(Monomer* attacker, Monomer* victim)
{
    if (!Node::init())
        return false;
    
    if (attacker == NULL || victim == NULL)
        return false;
    
    attacker->addChild(m_attacker);
    victim->addChild(m_victim);
    
    this->initWithShowSprite();
    
    if (m_skillInfo.getCasterSpecificID() != 0)
    {
        std::string path = TextureController::getTexturePath(TexturePathSkillCaster, m_skillInfo.getCasterSpecificID());
        this->playCasterSpecific(path.c_str());
    }

    this->scheduleOnce(schedule_selector(SkillEffectShow::emission), m_hitDelay);
    
    return true;
}

void SkillEffectShow::initWithShowSprite()
{
    m_showSprite = Sprite::create();
    m_showSprite->setPosition(((Monomer*)m_attacker->getParent())->getHandPoint());
    GAME_SCENE->getCurrBgMap()->addChild(m_showSprite);
}

void SkillEffectShow::playCasterSpecific(const char *path)
{
    Figure* figure = ((Monomer*)m_attacker->getParent())->getFigure();
    
    if (figure->getWeaponSprite() == NULL)
        return;
    
    Sprite* sprite = Sprite::create();
    sprite->setPosition(figure->getWeaponSprite()->getContentSize()/2);
    figure->getWeaponSprite()->addChild(sprite);
    
    //CCArray* array = CCArray::createWithCapacity(4);
    Vector<SpriteFrame*>array;
    int flag = 0;
    
    do
    {
        //CCString* string = CCString::createWithFormat("%s_%d0_%02d.png", path, figure->getDirectionType(), flag);
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s_%d0_%02d.png", path, figure->getDirectionType(), flag));
        CC_BREAK_IF(spriteFrame == NULL);
        array.pushBack(spriteFrame);
        flag++;
    }
    while (1);
    
    if (array.size() > 0)
    {
        Animation* movie = Animation::createWithSpriteFrames(array, 1 / (float)10);
        Animate* animate = Animate::create(movie);
       CallFunc* callFunc = CallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
        //CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(CCSprite::removeFromParent, sprite));
        
        Sequence* actions = Sequence::create(animate, callFunc, NULL);
        sprite->runAction(actions);
    }
    else
    {
        sprite->removeFromParent();
    }
}

void SkillEffectShow::emission(float delay)
{
    this->schedule(schedule_selector(SkillEffectShow::track));
    
    if (m_skillInfo.getLocusSpecificID() != 0)
    {
        std::string path = TextureController::getTexturePath(TexturePathSkillLocus, m_skillInfo.getLocusSpecificID());
        
        this->playLocusSpecific(path.c_str());
        
        if (m_skillInfo.getIsTailing())
        {
            this->playTailing();
        }
    }
}

void SkillEffectShow::track(float delay)
{
    float speed = m_skillInfo.getFlightSpeed() / 60.0f;
    
    if (m_skillInfo.getFlightSpeed() == 0)
    {
        speed = 0xffffffff;
    }
  
    float lenght = ccpDistance(m_showSprite->getPosition(), this->getDestination());
    float lenghtX = this->getDestination().x - m_showSprite->getPositionX();
    float lenghtY = this->getDestination().y - m_showSprite->getPositionY();
    float x = m_showSprite->getPositionX() + speed * lenghtX / lenght;
    float y = m_showSprite->getPositionY() + speed * lenghtY / lenght;
    Point point = Point(x, y);
    
    if (lenght <= 64)
    {
        m_showSprite->setVisible(false);
    }
    if (ccpDistance(point, this->getDestination()) > speed)
    {
        m_showSprite->setPosition(point);
        CCPoint high = ((Monomer*)m_attacker->getParent())->getHandPointRelativeFootOffset();
        m_showSprite->setZOrder(BgMap::getZOrder(ccpSub(point, high)) + 1);
    }
    else
    {
        m_showSprite->setPosition(this->getDestination());
        this->unschedule(schedule_selector(SkillEffectShow::track));
        this->hit();
    }
}

void SkillEffectShow::hit()
{
    if (m_skillInfo.getExplosionSpecificID() != 0)
    {
        std::string path = TextureController::getTexturePath(TexturePathSkillExplosion, m_skillInfo.getExplosionSpecificID());
        this->playExplosionSpecific(path.c_str());
    }
    else
    {
        this->sendMessage();
        this->releaseThis();
    }
}

void SkillEffectShow::updateTailing(float delay)
{
    float lenght = ccpDistance(this->getDeparture(), m_showSprite->getPosition());
    float lenghtX = m_showSprite->getPositionX() - this->getDeparture().x;
    float lenghtY = m_showSprite->getPositionY() - this->getDeparture().y;
    
    float angle_X = asinf(lenghtY / lenght) * 180 / M_PI;
	float angle_Y = asinf(lenghtX / lenght) * 180 / M_PI;

    float rotation = 0;
    
    if (angle_X >= 0)
    {
        rotation = angle_Y - 90;
    }
    else
    {
        rotation = 90 - angle_Y ;
    }
    
    Size preferredSize = Size::ZERO;
    
    if (lenght <30)
    {
        preferredSize.width = 0;
    }
    else
    {
        preferredSize.width = lenght - 30;
    }
    
    preferredSize.height = 5;
    
    m_tailSprite->setPreferredSize(preferredSize);

    m_tailSprite->setRotation(rotation);
}

void SkillEffectShow::playTailing()
{
    //拖尾特效播放
    
    m_tailSprite = Scale9Sprite::create("ui/red.png");
    m_tailSprite->setPreferredSize(Size::ZERO);
    m_tailSprite->setAnchorPoint(Point(1, 0.5));
    m_tailSprite->setPosition(Point::ZERO);
    m_showSprite->addChild(m_tailSprite, -1);
    
    this->schedule(schedule_selector(SkillEffectShow::updateTailing));
}

void SkillEffectShow::playLocusSpecific(const char *path)
{
    int rotation = this->getRotationWithLocusSpecific();
    
    int flag = 0;
    
    //CCArray* array = CCArray::createWithCapacity(4);
    Vector<SpriteFrame*>array;
    do
    {
        //CCString* string = CCString::createWithFormat("%s_%d_%02d.png", path, rotation, flag);
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s_%d_%02d.png", path, rotation, flag));
        CC_BREAK_IF(spriteFrame == NULL);
        array.pushBack(spriteFrame);
        flag++;
    }
    while (1);
    
    if (array.size() > 0)
    {
        Animation* movie = Animation::createWithSpriteFrames(array, 1 / (float)24);
        Animate* animate = Animate::create(movie);
        RepeatForever* repeat = RepeatForever::create(animate);
        m_showSprite->runAction(repeat);
    }
}

void SkillEffectShow::playExplosionSpecific(const char *path)
{
    m_explosionSprite = CCSprite::create();
    
    if (m_skillInfo.getIsThirdParty() == true)
    {
        Point point = m_showSprite->getPosition();
        m_explosionSprite->setPosition(point);
        GAME_SCENE->getCurrBgMap()->addChild(m_explosionSprite);
        Point high = ((Monomer*)m_attacker->getParent())->getHurtPointRelativeFootOffset();
        m_explosionSprite->setZOrder(BgMap::getZOrder(ccpSub(point, high)) + 1);

    }
    else
    {
        Monomer* victim = (Monomer*)m_victim->getParent();
        if (victim == NULL)
        {
            this->releaseThis();
            return;
        }
        
        float x = victim->getContentSize().width * victim->getFigure()->getAnchorPointCenter().x;
        float y = victim->getContentSize().height * victim->getFigure()->getAnchorPointCenter().y;
        m_explosionSprite->setPosition(Point(x, y));
        m_victim->addChild(m_explosionSprite);
    }
    
    int flag = 0;
    
    //CCArray* array = CCArray::createWithCapacity(4);
    Vector<SpriteFrame*>array;
    do
    {
       // CCString* string = CCString::createWithFormat("%s_%02d.png", path, flag);
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s_%02d.png", path, flag));
        CC_BREAK_IF(spriteFrame == NULL);
        array.pushBack(spriteFrame);
        flag++;
    }
    while (1);
    
    if (array.size() > 0)
    {
        Animation* movie = Animation::createWithSpriteFrames(array, 1 / (float)24);
        Animate* animate = Animate::create(movie);
        CallFunc* finish = CallFunc::create(this, callfunc_selector(SkillEffectShow::releaseThis));
        Sequence* actions1 = Sequence::create(animate, finish, NULL);
        
        DelayTime* delay = DelayTime::create(animate->getDuration()/2);
        CallFunc* callFunc = CallFunc::create(this, callfunc_selector(SkillEffectShow::sendMessage));
        Sequence* actions2 = Sequence::create(delay, callFunc, NULL);
        Spawn* spawn = Spawn::create(actions1, actions2, NULL);
        
        m_explosionSprite->runAction(spawn);
    }
    else
    {
        this->sendMessage();
        this->releaseThis();
    }
}

int SkillEffectShow::getRotationWithLocusSpecific()
{
    
    float lenghtX = this->getDestination().x - m_showSprite->getPositionX();
    float lenghtY = this->getDestination().y - m_showSprite->getPositionY();
    lenghtY = lenghtY * sqrtf(2);
    float lenght = sqrtf(lenghtX * lenghtX + lenghtY * lenghtY);
    float angle_X = acosf(lenghtX / lenght) * 180 / M_PI;
	float angle_Y = acosf(lenghtY / lenght) * 180 / M_PI;

    float angle = angle_X;
    if (angle_Y > 90)
    {
        angle = 360 - angle_X;
    }
    
    int relust = 0;
    
    if (fabsf(67.5 - angle) <= 11.25)
    {
        //up2 right1
        relust = 5;
    }
    else if (fabsf(45 - angle) <= 11.25)
    {
        //up2 right2
        relust = 10;
    }
    else if (fabsf(22.5 - angle) <= 11.25)
    {
        //up1 right2
        relust = 15;
    }
    else if (fabsf(0 - angle) <= 11.25)
    {
        //right2
        relust = 20;
    }
    else if (fabsf(337.5 - angle) <= 11.25)
    {
        //right2 down1
        relust = 25;
    }
    else if (fabsf(315 - angle) <= 11.25)
    {
        //right2 down2
        relust = 30;
    }
    else if (fabsf(292.5 - angle) <= 11.25)
    {
        //right1 down2
        relust = 35;
    }
    else if (fabsf(270 - angle) <= 11.25)
    {
        //down2
        relust = 40;
    }
    else if (fabsf(247.5 - angle) <= 11.25)
    {
        //down2 left
        relust = 45;
    }
    else if (fabsf(225 - angle) <= 11.25)
    {
        //down2 left2
        relust = 50;
    }
    else if (fabsf(202.5 - angle) <= 11.25)
    {
        //down1 left2
        relust = 55;
    }
    else if (fabsf(180 - angle) <= 11.25)
    {
        //left2
        relust = 60;
    }
    else if (fabsf(157.5 - angle) <= 11.25)
    {
        //left2 up1
        relust = 65;
    }
    else if (fabsf(135 - angle) <= 11.25)
    {
        //left2 up2
        relust = 70;
    }
    else if (fabsf(112.5 - angle) <= 11.25)
    {
        //left up2
        relust = 75;
    }
    else if (fabsf(90 - angle) <= 11.25)
    {
        //up2
        relust = 80;
    }
    
    return relust;
}

const Vec2 SkillEffectShow::getDestination()
{
    Vec2 point = Vec2::ZERO;
    if ((Monomer*)m_victim->getParent() == NULL)
    {
        this->releaseThis();
        return Vec2::ZERO;
    }
    return ((Monomer*)m_victim->getParent())->getHurtPoint();
}

const Vec2 SkillEffectShow::getDeparture()
{
    if ((Monomer*)m_attacker->getParent() == NULL)
    {
        this->releaseThis();
        return Vec2::ZERO;
    }
    return ((Monomer*)m_attacker->getParent())->getHandPoint();
}

void SkillEffectShow::sendMessage()
{
    if (m_skillInfo.getIsThirdParty())
    {
        if (m_skillInfo.getExplosionFanAngle() == 360)
        {
            vector<Monomer*> vec = GAME_SCENE->getMonmerVecIsLenght(m_showSprite->getPosition(), m_skillInfo.getExplosionRadius());
            if (vec.size() == 0)
            {
                vec.push_back(((Monomer*)m_victim->getParent()));
            }
            GameSocket::attackGroup(((Monomer*)m_attacker->getParent()), vec, m_skillInfo.getSkillNumber());
        }
        else
        {
            
        }
    }
    else
    {
        GameSocket::attack(((Monomer*)m_attacker->getParent()), ((Monomer*)m_victim->getParent()), m_skillInfo.getSkillNumber());
    }
}

void SkillEffectShow::releaseThis()
{
    if (m_showSprite && m_showSprite->getParent())
    {
        m_showSprite->removeFromParent();
        m_showSprite = NULL;
    }
    if (m_explosionSprite && m_explosionSprite->getParent())
    {
        m_explosionSprite->removeFromParent();
        m_explosionSprite = NULL;
    }
    if (this->getParent())
    {
        this->removeFromParent();
    }
}
