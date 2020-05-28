

#include "LMenu.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
#define PLAYSOUND 

static int count = 0;

LMenu::LMenu(void)
:m_sign(0)
,m_stateSelected(false)
,m_textTag("")
,m_bIsTouchEnabled(true)
,m_specially(true)
,m_closeTapSound(false)
,m_touchClick(false)
,m_touchState(LMenuStateNormal)
,m_color(Color3B::WHITE)
,m_bgNormal(NULL)
,m_bgHighlighted(NULL)
,m_bgDisabled(NULL)
,m_bgSelected(NULL)
,m_spriteNormal(NULL)
,m_spriteHighlighted(NULL)
,m_spriteDisabled(NULL)
,m_spriteSelected(NULL)
,m_spriteNPoint(Point::ZERO)
,m_spriteHPoint(Point::ZERO)
,m_preferredSize(Point::ZERO)
,m_target(NULL)
,m_touchBegin(NULL)
,m_touchMoved(NULL)
,m_touchMovedOutSide(NULL)
,m_touchUpInSide(NULL)
,m_touchUpSide(NULL)
,m_nbSwallowsTouches(true)
//,m_IsRehisterTouchDispatcher(false)
//,m_nTouchPriority((int)kCCMenuHandlerPriority)
{
    count++;
    printf("LMenu:%d",count);
}

LMenu::~LMenu(void)
{    
    count--;
    printf("~LMenu:%d",count);
}

void LMenu::onExit(){
    
    //this->unRegisterWithTouchDispatcher();
    Sprite::onExit();
}

void LMenu::onEnter(){
    
    Sprite::onEnter();
    //this->registerWithTouchDispatcher();
   
    
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(LMenu::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(LMenu::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(LMenu::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
     this->updateWithPoint();
    
}


LMenu* LMenu::create(const Size &size)
{

    LMenu* btn=new LMenu();
    
    if (btn && btn->init())
    {
        btn->setPreferredSize(size);
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

LMenu* LMenu::createWithRGBAProtocol(CCRGBAProtocol* backGround){
    
    LMenu* btn=new LMenu();
    
    if (btn && btn->initWithBackGround(backGround))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

LMenu* LMenu::createWithSprite(Sprite *backGround){

    return LMenu::createWithRGBAProtocol(backGround);
}

LMenu* LMenu::createWithScale9Sprite(Scale9Sprite *backGround){
    
    return LMenu::createWithRGBAProtocol(backGround);
}

LMenu* LMenu::createWithLayerColor(LayerColor *backGround){
    
    return LMenu::createWithRGBAProtocol(backGround);
}

bool LMenu::initWithBackGround(CCRGBAProtocol *backGround){
    
    if (!backGround)
    {
        return false;
    }
    
    if (!Sprite::init())
    {
        return false;
    }

    if (Node* _backGround=dynamic_cast<Node*>(backGround))
    {
        this->setPreferredSize(_backGround->getContentSize());
    }
    else
    {
        return false;
    }
    
    return this->setBackGround(LMenuStateNormal, backGround);
}

bool LMenu::initWithSpriteBackGround(Sprite *backGround){

    return this->initWithBackGround(backGround);
}

bool LMenu::initWithScale9SpriteBackGround(Scale9Sprite *backGround){
    
    return this->initWithBackGround(backGround);
}

bool LMenu::initWithLayerColorBackGround(LayerColor *backGround){
    
    return this->initWithBackGround(backGround);
}

bool LMenu::setBackGround(LMenuTouchState touchState, CCRGBAProtocol *var){

    if (!var)
    {
        return false;
    }
    
    if (Node* node=dynamic_cast<Node*>(var))
    {
        this->addChild(node,-1);
        
        if (!m_preferredSize.equals(CCSizeZero) && !m_preferredSize.equals(node->getContentSize()))
        {
            if (Scale9Sprite* _var=dynamic_cast<Scale9Sprite*>(node))
            {
                _var->setPreferredSize(m_preferredSize);
            }
            if (CCLayerRGBA* _var=dynamic_cast<CCLayerRGBA*>(node))
            {
                _var->setContentSize(m_preferredSize);
            }
        }
        
        if (touchState==LMenuStateNormal)
        {
            if (m_bgNormal)
            {
                m_bgNormal->removeFromParent();
            }
            m_bgNormal=node;
        }
        else
        {
            node->setVisible(false);
        }
        
        if (touchState==LMenuStateHighlighted)
        {
            if (m_bgHighlighted)
            {
                m_bgHighlighted->removeFromParent();
            }
            m_bgHighlighted=node;
        }
        
        if (touchState==LMenuStateDisabled)
        {
            if (m_bgDisabled)
            {
                m_bgDisabled->removeFromParent();
            }
            m_bgDisabled=node;
        }
        
        if (touchState==LMenuStateSelected)
        {
            if (m_bgSelected)
            {
                m_bgSelected->removeFromParent();
            }
            m_bgSelected=node;
        }
        
        this->updateWithPoint();
        this->updateWithPreferredSize();
        
        return true;

    }

    return false;
}

bool LMenu::setSpriteBackGround(LMenuTouchState touchState, Sprite *var){
    
    return this->setBackGround(touchState, var);
}

bool LMenu::setScale9SpriteBackGround(LMenuTouchState touchState, Scale9Sprite *var){
    
    return this->setBackGround(touchState, var);
}

bool LMenu::setLayerColorBackGround(LMenuTouchState touchState, LayerColor *var){
    
    return this->setBackGround(touchState, var);
}

void LMenu::updateWithPreferredSize(){

    if (m_bgNormal && !m_bgNormal->getContentSize().equals(m_preferredSize));
    {
        if (Scale9Sprite* node=dynamic_cast<Scale9Sprite*>(m_bgNormal))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgNormal))
        {
            node->setContentSize(m_preferredSize);
        }
    }

    if (m_bgHighlighted && !m_bgHighlighted->getContentSize().equals(m_preferredSize))
    {
        if (Scale9Sprite* node=dynamic_cast<Scale9Sprite*>(m_bgHighlighted))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgHighlighted))
        {
            node->setContentSize(m_preferredSize);
        }
    }

    if (m_bgDisabled && !m_bgDisabled->getContentSize().equals(m_preferredSize))
    {
        if (Scale9Sprite* node=dynamic_cast<Scale9Sprite*>(m_bgDisabled))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgDisabled))
        {
            node->setContentSize(m_preferredSize);
        }
    }
    
    if (m_bgSelected && !m_bgSelected->getContentSize().equals(m_preferredSize))
    {
        if (Scale9Sprite* node=dynamic_cast<Scale9Sprite*>(m_bgSelected))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgSelected))
        {
            node->setContentSize(m_preferredSize);
        }
    }

}

void LMenu::updateWithPoint(){
    
    Point point=Point(m_preferredSize.width/2, m_preferredSize.height/2);
    
    if (m_bgNormal)
    {
        if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgNormal))
        {
            node->setPosition(point);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgNormal))
        {
            node->setPosition(Point::ZERO);
        }
    }
    
    if (m_spriteNormal)
    {
        if (m_spriteNPoint.equals(Point::ZERO))
        {
            m_spriteNormal->setPosition(point);
        }
        else
        {
            m_spriteNormal->setPosition(m_spriteNPoint);
        }
    }
    
    if (m_bgHighlighted)
    {
        if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgHighlighted))
        {
            node->setPosition(point);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgHighlighted))
        {
            node->setPosition(Point::ZERO);
        }
    }
    
    if (m_spriteHighlighted)
    {
        if (m_spriteHPoint.equals(Point::ZERO))
        {
            m_spriteHighlighted->setPosition(point);
        }
        else
        {
            m_spriteHighlighted->setPosition(m_spriteHPoint);
        }
    }
    
    if (m_bgDisabled)
    {
        if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgDisabled))
        {
            node->setPosition(point);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgDisabled))
        {
            node->setPosition(Point::ZERO);
        }
    }
    
    if (m_spriteDisabled)
    {
        if (m_spriteDPoint.equals(Point::ZERO))
        {
            m_spriteDisabled->setPosition(point);
        }
        else
        {
            m_spriteDisabled->setPosition(m_spriteDPoint);
        }
    }
    
    if (m_bgSelected)
    {
        if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgSelected))
        {
            node->setPosition(point);
        }
        if (CCLayerRGBA* node=dynamic_cast<CCLayerRGBA*>(m_bgSelected))
        {
            node->setPosition(Point::ZERO);
        }
    }
    
    if (m_spriteSelected)
    {
        if (m_spriteSPoint.equals(Point::ZERO))
        {
            m_spriteSelected->setPosition(point);
        }
        else
        {
            m_spriteSelected->setPosition(m_spriteSPoint);
        }
    }
}

Rect LMenu::getRect(){

    return Rect(0, 0, m_preferredSize.width, m_preferredSize.height);
}

bool LMenu::onTouchBegan(Touch *pTouch, Event *pEvent) {
    
    Point uipoint=pTouch->getLocationInView();
    Point point=Director::getInstance()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (!this->isVisible()) return false;
    
    if (!m_bIsTouchEnabled) return false;
    
    if (!getRect().containsPoint(point)) return false;
    
    if (!isNormal()) return false;
    
    return this->setTouchBegin(point);
}

void LMenu::onTouchMoved(Touch *pTouch, Event *pEvent){
    
    Point uipoint=pTouch->getLocationInView();
    Point point=Director::getInstance()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (!this->getTouchClick()) return;
    
    if (getRect().containsPoint(point))
    {
        this->setTouchState(LMenuStateHighlighted);
        this->setTouchMoved(point);
    }
    else
    {
        this->setTouchState(LMenuStateNormal);
        this->setTouchMovedOutSide(point);
    }
}

void LMenu::onTouchEnded(Touch *pTouch, Event *pEvent){
    
    Point uipoint=pTouch->getLocationInView();
    Point point=Director::getInstance()->convertToGL(uipoint);
    point = this->convertToNodeSpace(point);

    if (!this->getTouchClick()) return;
    
    this->setTouchClick(false);
    
    this->setTouchUpSide(point);
    
    if (getRect().containsPoint(point))
    {
        if (m_stateSelected)
        {
            this->setTouchState(LMenuStateSelected);
        }
        this->setTouchUpInSide(point);
    }
}

void LMenu::onTouchCancelled(Touch *pTouch, Event *pEvent){
    
    this->onTouchEnded(pTouch, pEvent);
}

void LMenu::setTouchEnabled(bool enabled){
    
    m_bIsTouchEnabled=enabled;
    
    if (!m_specially) return;
    
    if (m_bIsTouchEnabled)
    {
        this->setTouchState(LMenuStateNormal);
    }
    else
    {
        this->setTouchState(LMenuStateDisabled);
    }
}

bool LMenu::getTouchEnabled()
{
    
    return m_bIsTouchEnabled;
}

//void LMenu::registerWithTouchDispatcher(){
//    
//    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//    touchDispatcher->addTargetedDelegate(this, m_nTouchPriority, m_nbSwallowsTouches); 
//    m_IsRehisterTouchDispatcher=true;
//}
//
//void LMenu::unRegisterWithTouchDispatcher(){
//    
//    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//    touchDispatcher->removeDelegate(this);
//    m_IsRehisterTouchDispatcher=false;
//}

//void LMenu::setTouchDispatcher(bool bSwallowsTouches, int nPriority) {
//    
//    m_nbSwallowsTouches=bSwallowsTouches;
//    m_nTouchPriority=nPriority;
//    
//    if (m_IsRehisterTouchDispatcher)
//    {
//        CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//        touchDispatcher->removeDelegate(this);
//        touchDispatcher->addTargetedDelegate(this, nPriority, bSwallowsTouches);
//    }
//}

void LMenu::addTarget(void *target, SEL_LMenu selector, LMenuType type){
    
    switch (type)
    {
        case TouchBegin:
            m_touchBegin=selector;
            break;
        case TouchMoved:
            m_touchMoved=selector;
            break;
        case TouchMovedOutSide:
            m_touchMovedOutSide=selector;
            break;
        case TouchUpInSide:
            m_touchUpInSide=selector;
            break;
        case TouchUpSide:
            m_touchUpSide=selector;
            break;
        default:
            return;
    }
    
    m_target=target;
}

bool LMenu::isNormal(){
    
    return (m_touchState==LMenuStateNormal);
}

bool LMenu::isHighlighted(){
    
    return (m_touchState==LMenuStateHighlighted);
}

bool LMenu::isDisabled(){
    
    return (m_touchState==LMenuStateDisabled);
}

bool LMenu::isSelected(){
    
    return (m_touchState==LMenuStateSelected);
}

void LMenu::setTouchState(LMenuTouchState var){
    
    if (m_touchState==var) return;
    
    this->setNormalEffect();
    
    m_touchState=var;
    
    switch (m_touchState)
    {
        case LMenuStateHighlighted:
            this->setHighlightedEffect();
            break;
        case LMenuStateDisabled:
            this->setDisabledEffect();
            break;
        case LMenuStateSelected:
            this->setSelectedEffect();
            break;
        default:
            break;
    }
    
    
}

LMenuTouchState LMenu::getTouchState()
{
    
    return m_touchState;
}

void LMenu::setTouchClick(bool var){
    
    if (m_touchClick==var) return;
    
    m_touchClick=var;
    
    if (m_touchClick)
    {
        this->setTouchState(LMenuStateHighlighted);
    }
    else
    {
        this->setTouchState(LMenuStateNormal);
    }
    
    if (var && !m_closeTapSound) PLAYSOUND;
}

bool LMenu::getTouchClick()
{
    
    return m_touchClick;
}

bool LMenu::setTouchBegin(Point point){
    
    if (m_touchBegin)
    {
        bool touchClick=((Ref *)m_target->*m_touchBegin)(this,point);
        this->setTouchClick(touchClick);
        return touchClick;
    }
    
    this->setTouchClick(true);
    
    return true;
}

void LMenu::setTouchUpInSide(Point point){
    
    if (m_touchUpInSide)
    {
        ((Ref *)m_target->*m_touchUpInSide)(this,point);
    }
}

void LMenu::setTouchUpSide(Point point){
    
    if (m_touchUpSide)
    {
        ((Ref *)m_target->*m_touchUpSide)(this,point);
    }
}

void LMenu::setTouchMoved(Point point){
    
    if (m_touchMoved)
    {
        ((Ref *)m_target->*m_touchMoved)(this,point);
    }
}

void LMenu::setTouchMovedOutSide(Point point){
    
    if (m_touchMovedOutSide)
    {
        ((Ref *)m_target->*m_touchMovedOutSide)(this,point);
    }
}

void LMenu::setNormalEffect(){

    if (!m_specially) return;
    
    if (m_touchState==LMenuStateNormal) return;
    
    if (m_bgNormal)
        m_bgNormal->setVisible(true);
    
    if (m_spriteNormal)
        m_spriteNormal->setVisible(true);

    if (m_touchState==LMenuStateHighlighted)
    {
        
        if (m_bgHighlighted) m_bgHighlighted->setVisible(false);
        
        if (m_spriteHighlighted) m_spriteHighlighted->setVisible(false);
        
    }
    else if (m_touchState==LMenuStateDisabled)
    {
        
        if (m_bgDisabled) m_bgDisabled->setVisible(false);
        
        if (m_spriteDisabled) m_spriteDisabled->setVisible(false);
        
    }
    else if (m_touchState==LMenuStateSelected)
    {
        
        if (m_bgSelected) m_bgSelected->setVisible(false);
        
        if (m_spriteSelected) m_spriteSelected->setVisible(false);
        
    }
    
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setNormalColor();

}

void LMenu::setHighlightedEffect(){

    if (!m_specially) return;
    
    if (m_bgHighlighted)
    {
        if (m_bgNormal) m_bgNormal->setVisible(false);
        
        m_bgHighlighted->setVisible(true);
    }
    
    if (m_spriteHighlighted)
    {
        if (m_spriteNormal) m_spriteNormal->setVisible(false);
        
        m_spriteHighlighted->setVisible(true);
    }
    
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setHighlightedColor();
}

void LMenu::setDisabledEffect(){

    if (!m_specially) return;
    
    if (m_bgDisabled)
    {
        if (m_bgNormal) m_bgNormal->setVisible(false);
        
        m_bgDisabled->setVisible(true);
    }
        
    if (m_spriteDisabled)
    {
        if (m_spriteNormal) m_spriteNormal->setVisible(false);
        
        m_spriteDisabled->setVisible(true);
    }
        
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setDisabledColor();
}

void LMenu::setSelectedEffect(){

    if (!m_specially) return;
    
    if (m_bgSelected)
    {
        if (m_bgNormal) m_bgNormal->setVisible(false);
        
        m_bgSelected->setVisible(true);
    }
        
    if (m_spriteSelected)
    {
        if (m_spriteNormal) m_spriteNormal->setVisible(false);
        
        m_spriteSelected->setVisible(true);
    }
        
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setSelectedColor();
}

void LMenu::setNormalColor(){
    
    if (this->getChildren().size()>0)
    {
        for (int i=0; i<this->getChildren().size(); i++)
        {
            Node* node=(Node*)this->getChildren().at(i);
            
            if (   node==(m_bgHighlighted)
                || node==(m_bgDisabled)
                || node==(m_bgSelected))
            {
                break;
            }
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                ccColor3B color=_children->getColor();
                float red=color.r*2.0f;
                float green=color.g*2.0f;
                float blue=color.b*2.0f;
                _children->setColor(ccc3(red,green,blue));
                
            }
        }
    }
}

void LMenu::setHighlightedColor(){
    
    if (this->getChildren().size()>0)
    {
        for (int i=0; i<this->getChildren().size(); i++)
        {
            Node* node=(Node*)this->getChildren().at(i);
            
            if (   node==(m_bgHighlighted)
                || node==(m_bgDisabled)
                || node==(m_bgSelected))
            {
                break;
            }
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                ccColor3B color=_children->getColor();
                float red=color.r/2.0f;
                float green=color.g/2.0f;
                float blue=color.b/2.0f;
                _children->setColor(ccc3(red,green,blue));
                
            }
        }
    }
}

void LMenu::setDisabledColor(){

    this->setHighlightedColor();
}

void LMenu::setSelectedColor(){

    this->setHighlightedColor();
}

bool LMenu::isTextTagEqual(const char *text){
    
    return (m_textTag.compare(text)==0);
}

void LMenu::setPreferredSize(Size var){
    
    m_preferredSize=var;
    
    this->setContentSize(m_preferredSize);
    
    this->updateWithPoint();
    
    this->updateWithPreferredSize();
}

Size LMenu::getPreferredSize()
{
    
    return m_preferredSize;
}

void LMenu::setSprite(LMenuTouchState touchState, CCNodeRGBA* var){
    
    if (!var) return;
    
    var->setAnchorPoint(Point(0.5f, 0.5f));
    
    Point point=Point(m_preferredSize.width/2, m_preferredSize.height/2);
    
    this->setSprite(touchState, var, point);
}

void LMenu::setSprite(LMenuTouchState touchState, CCNodeRGBA* var, Point point){
    
    if (!var) return;

    var->setPosition(point);
    this->addChild(var);
    
    Point cPoint=Point(m_preferredSize.width/2, m_preferredSize.height/2);
    
    if (touchState==LMenuStateNormal)
    {
        if (m_spriteNormal)
        {
            m_spriteNormal->removeFromParent();
            m_spriteNormal=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteNPoint=point;
        }
        else
        {
            m_spriteNPoint=Point::ZERO;
        }
        
        m_spriteNormal=var;
        
        return;
    }
    else if (touchState==LMenuStateHighlighted)
    {
        if (m_spriteHighlighted)
        {
            m_spriteHighlighted->removeFromParent();
            m_spriteHighlighted=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteHPoint=point;
        }
        else
        {
            m_spriteHPoint=Point::ZERO;
        }
        
        m_spriteHighlighted=var;
    }
    else if (touchState==LMenuStateDisabled)
    {
        if (m_spriteDisabled)
        {
            m_spriteDisabled->removeFromParent();
            m_spriteDisabled=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteDPoint=point;
        }
        else
        {
            m_spriteDPoint=Point::ZERO;
        }
        
        m_spriteDisabled=var;
    }
    else if (touchState==LMenuStateSelected)
    {
        if (m_spriteSelected)
        {
            m_spriteSelected->removeFromParent();
            m_spriteSelected=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteSPoint=point;
        }
        else
        {
            m_spriteSPoint=Point::ZERO;
        }
        
        m_spriteSelected=var;
    }
    
    var->setVisible(false);
}

CCNodeRGBA* LMenu::getSprite(LMenuTouchState touchState){
    
    switch (touchState)
    {
        case LMenuStateNormal:
            return m_spriteNormal;
            break;
        case LMenuStateHighlighted:
            return m_spriteHighlighted;
            break;
        case LMenuStateDisabled:
            return m_spriteDisabled;
            break;
        case LMenuStateSelected:
            return m_spriteSelected;
            break;
        default:
            return NULL;
            break;
    }
}


Node* LMenu::getBackGround(LMenuTouchState touchState){
    
    switch (touchState)
    {
        case LMenuStateNormal:
            return m_bgNormal;
            break;
        case LMenuStateHighlighted:
            return m_bgHighlighted;
            break;
        case LMenuStateDisabled:
            return m_bgDisabled;
            break;
        case LMenuStateSelected:
            return m_bgSelected;
            break;
        default:
            return NULL;
            break;
    }
}

void LMenu::setOpacity(GLubyte opacity){
    
    CCSprite::setOpacity(opacity);
    
    if (this->getChildren().size()>0)
    {
        for (int i=0; i<this->getChildren().size(); i++)
        {
            Node* node=(Node*)this->getChildren().at(i);
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                _children->setOpacity(opacity);
            }
        }
    }
}

void LMenu::setColor(const ccColor3B &color3){
    
    m_color=color3;
    
    if (this->getChildren().size()>0)
    {
        for (int i=0; i<this->getChildren().size(); i++)
        {
            Node* node=(Node*)this->getChildren().at(i);
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                _children->setColor(m_color);
            }
        }
    }
}

ccColor3B& LMenu::getColor(){
    
    return m_color;
}
