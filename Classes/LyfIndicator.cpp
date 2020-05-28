

#include "LyfIndicator.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
#define dvalue(x) x*2
#define BG_SIZE this->getContentSize()
#define CENTER_POINT Point(this->getContentSize()/2)
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
#define HOMEPOINT Point(this->getParentNode()->getContentSize().width\
+this->getContentSize().width*this->getScaleX()/2,\
parentNode->getContentSize().height/2)

LyfIndicator::LyfIndicator(LayerColor* layerColor)
:m_bg(NULL)
,m_indicator(NULL)
,m_indWidth(0)
,m_indHeight(0)
,m_scale(1)
,m_offPoint(Point::ZERO)
{
    this->setParentNode(layerColor);
}

LyfIndicator::~LyfIndicator(){
}

LyfIndicator* LyfIndicator::createWithAddChild(LayerColor* layerColor, float scale){
    
    LyfIndicator* indicator=new LyfIndicator(layerColor);
    if (indicator && indicator->init())
    {
        indicator->initWithBg();
        indicator->initWithIndicator(scale);
        indicator->autorelease();
        return indicator;
    }
    
    CC_SAFE_DELETE(indicator);
    return NULL;
}

bool LyfIndicator::init(){
    
    if (!Sprite::init()) return false;
    
    this->setPosition(HOMEPOINT);
    
    this->getParentNode()->addChild(this);
    
    return true;
}

void LyfIndicator::refresh(){
    
    this->setPosition(ccpAdd(HOMEPOINT, m_offPoint));
    
    if (m_bg) m_bg->removeFromParent();

    this->initWithBg();
    
    m_bg->setPosition(CENTER_POINT);
}

void LyfIndicator::setPointOff(Point point){

    if (m_offPoint.x==point.x && m_offPoint.y==point.y)
        return;
    
    m_offPoint=point;
    this->setPosition(ccpAdd(HOMEPOINT, m_offPoint));
}

void LyfIndicator::initWithBg(){
    
    
    if (BACKGROUNDIMAGE)
    {
        m_bg=Scale9Sprite::create(BACKGROUNDIMAGE);
    }
    else
    {
        m_bg=Scale9Sprite::create();
    }
    
    Size bgSize = m_bg->getContentSize();
    bgSize.width = 16;
    Size size = this->getParentNode()->getContentSize();
    m_bg->setPreferredSize(Size(bgSize.width, size.height-OFF_HEIGHT/2));
    this->addChild(m_bg,-1);
    
    this->setContentSize(Size(bgSize.width, size.height-OFF_HEIGHT));
    
}

void LyfIndicator::initWithIndicator(float scale){
    
    m_indicator=Scale9Sprite::create(INDICATORIMAGE);
    
    m_indWidth=m_indicator->getContentSize().width;
    
    Rect capinsets=Rect(4,
                            4,
                            2,
                            2);
    
    m_indicator->setCapInsets(capinsets);
    
    this->setScale(scale);
    
    this->addChild(m_indicator);
}

void LyfIndicator::setScale(float scale){
    
    m_scale=scale;
    
    if (m_scale>=1) m_scale=1;

    m_indHeight=BG_SIZE.height*scale;
    
    if (m_indHeight<m_indWidth) return;

    m_indicator->setPreferredSize(Size(m_indWidth, m_indHeight));
}

void LyfIndicator::setIndicator(float windowSize, float contentSize, float lenght){

    float rop=lenght/contentSize;
    float upRop=1-2.5*(lenght-contentSize)/windowSize;
    float downRop=1-2.5*(-lenght)/windowSize;
    
    if (rop<0)
    {
        rop=0;
        upRop=1;
    }
    else if (rop>1)
    {
        rop=1;
        downRop=1;
    }
    else
    {
        downRop=1;
        upRop=1;
        
        if (m_scale==1) rop=0;
    }
    
    m_indHeight=BG_SIZE.height*m_scale*upRop*downRop;
    
    if (m_indHeight<m_indWidth)
    {
        m_indHeight=m_indWidth;
    }

    int y = (int)m_indicator->getPositionY()*10;
    
    m_indicator->setPreferredSize(Size(m_indWidth, m_indHeight));
    m_indicator->setPositionY(BG_SIZE.height-m_indHeight/2-(BG_SIZE.height-m_indHeight)*rop);
    
    do
    {
        if (y == (int)m_indicator->getPositionY()*10)
        {
            CC_BREAK_IF(m_indicator->getActionByTag(0xfff));
            
            CC_BREAK_IF(m_indicator->getOpacity() < 255);
            
            DelayTime* delayTime = DelayTime::create(0.2f);
            FadeOut* fadeOut = FadeOut::create(0.3f);
            EaseSineOut* easeSineOut = EaseSineOut::create(fadeOut);
            Sequence* actions = Sequence::create(delayTime, easeSineOut, NULL);
            actions->setTag(0xfff);
            m_indicator->runAction(actions);
        }
        else
        {
            CC_BREAK_IF(m_indicator->getOpacity() == 255);
            
            m_indicator->stopAllActions();
            m_indicator->setOpacity(255);
        }
    }
    while (0);
    
    


}

void LyfIndicator::onEnter(){
    
    Sprite::onEnter();
    
    m_bg->setPosition(CENTER_POINT);
    
    m_indicator->setPosition(Point(CENTER_POINT.x, BG_SIZE.height-m_indicator->getContentSize().height/2));
}

void LyfIndicator::setOpacity(GLubyte opacity){
    
    CCSprite::setOpacity(opacity);
    m_indicator->setOpacity(opacity);
}
