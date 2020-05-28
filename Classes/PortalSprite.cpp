

#include "PortalSprite.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

PortalSprite::PortalSprite(const char* path)
:m_path(path)
,m_sprite(NULL)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture_set/portal.plist");
    //SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("texture_set/portal.plist");
    
}

PortalSprite::~PortalSprite()
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("texture_set/portal.plist");
}

PortalSprite* PortalSprite::createWithPortalSprite(const char* path)
{
    PortalSprite* sprite = new PortalSprite(path);
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        sprite->playAnimate();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

bool PortalSprite::init()
{
    if (!Sprite::init())
        return false;

    this->setContentSize(Size(150, 60));
    
    m_sprite = Sprite::create();
    m_sprite->setAnchorPoint(Point(0.5, 0.3));
    m_sprite->setPosition(this->getContentSize()/2);
    this->addChild(m_sprite);
    
    return true;
}

void PortalSprite::playAnimate()
{
    //CCArray* array = CCArray::createWithCapacity(8);
    Vector<SpriteFrame*>array;
    int i = 1;
    while (1)
    {
        //CCString* string = CCString::createWithFormat("%s%04d.png",m_path.c_str(), i);
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s%04d.png",m_path.c_str(), i));
        CC_BREAK_IF(spriteFrame == NULL);
        array.pushBack(spriteFrame);
        //array->addObject(spriteFrame);
        i++;
    }
    Animation* movie = Animation::createWithSpriteFrames(array, 1 / (float)9);
    Animate* animate = Animate::create(movie);
    RepeatForever* repeat = RepeatForever::create(animate);
    m_sprite->runAction(repeat);
}
