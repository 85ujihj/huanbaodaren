

#include "ProgressAutomatic.h"

ProgressAutomatic* ProgressAutomatic::create(Sprite* sp)
{
    ProgressAutomatic *pProgressTimer = new ProgressAutomatic();
    if (pProgressTimer->initWithSprite(sp))
    {
        Sprite* sprite = Sprite::createWithTexture(sp->getTexture());
        sprite->setColor(Color3B(100, 100, 100));
        sprite->setPosition(pProgressTimer->getContentSize()/2);
        pProgressTimer->addChild(sprite, -1);
        pProgressTimer->autorelease();
    }
    else
    {
        delete pProgressTimer;
        pProgressTimer = NULL;
    }
    
    return pProgressTimer;
}

void ProgressAutomatic::RunCoolingAction(float delay)
{
    if (this->getPercentage() < 100.0f)
        return;

    this->setPercentage(0);
    m_fInterval = 100 / (delay * 60);
    this->unschedule(schedule_selector(ProgressAutomatic::updateCoolingAction));
    this->schedule(schedule_selector(ProgressAutomatic::updateCoolingAction));
}

void ProgressAutomatic::updateCoolingAction(float t)
{
    float percentage = _percentage + m_fInterval;
    this->setPercentage(percentage);
    if (percentage >= 100.0f)
        this->unschedule(schedule_selector(ProgressAutomatic::updateCoolingAction));
}

void ProgressAutomatic::RunCoolingNotAction(float delay)
{
    if (this->getPercentage() < 100.0f)
        return;
    
    this->setPercentage(0);

    DelayTime* delayTime = DelayTime::create(delay);
    CallFunc* callFunc =CallFunc::create(CC_CALLBACK_0(ProgressAutomatic::setCoolingFalse, this));
    //CCCallFunc::create(this, callfunc_selector(ProgressAutomatic::setCoolingFalse));
    Sequence* actions = CCSequence::create(delayTime, callFunc, NULL);
    this->runAction(actions);
}

void ProgressAutomatic::setCoolingFalse()
{
    this->setPercentage(100);
}