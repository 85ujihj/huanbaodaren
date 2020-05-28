//
//  MAlert.cpp
//  guyue
//
//  Created by dsh on 15/1/18.
//
//

#include "PopupLayer.h"

USING_NS_CC;
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

// 构造函数中变量设初值

PopupLayer::PopupLayer()
{
    
    m_contentPadding = 0;
    m_contentPaddingTop = 0;
    m_pMenu = NULL;
    m_callbackListener = NULL;
    m_callback = NULL;
    m_sfBackGround = NULL;
    m_s9BackGround = NULL;
    m_ltContentText = NULL;
    m_ltTitle = NULL;
}


//释放

PopupLayer::~PopupLayer()
{
    
    CC_SAFE_RELEASE(m_pMenu);
    CC_SAFE_RELEASE(m_sfBackGround);
    CC_SAFE_RELEASE(m_s9BackGround);
    CC_SAFE_RELEASE(m_ltContentText);
    CC_SAFE_RELEASE(m_ltTitle);
    
}


//初始化

bool PopupLayer::init()
{
    
    if ( !Layer::init() ){
        return false;
    }
    
    this->setContentSize(Size::ZERO);
    //初始化需要的Menu
    
    Menu* menu = CCMenu::create();
    menu->setPosition(Vec2::ZERO);
    setMenuButton(menu);  //set()方法
    //setTouchEnabled(true);  //开启触摸响应
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
    
}


//重写触摸注册函数，重新给定触摸级别

//void PopupLayer::registerWithTouchDispatcher(){
//    
//    // 这里的触摸优先级设置为-128，与CCMenu同级，保证了屏蔽下方的触摸
//    
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
//    
//}


//触摸函数ccTouchBegan，返回true
bool PopupLayer::onTouchBegan( Touch *pTouch, Event *pEvent ){
    return true;
}


//创建一个弹出层，给背景精灵变量赋值
PopupLayer* PopupLayer::create( const char* backgroundImage ){
    
    PopupLayer* popup = PopupLayer::create();
    popup->setSpriteBackGround(Sprite::create(backgroundImage));
    popup->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
    return popup;
    
}


//给标题变量赋值

void PopupLayer::setTitle( const char* title, int fontsize ){
   auto ltfTitle = LabelTTF::create(title, "Arial", fontsize);
    ltfTitle->setColor(Color3B::WHITE);
    setLabelTitle(ltfTitle);
    
}


//给文本变量赋值
void PopupLayer::setContentText( const char* text, int fontsize, int padding, int paddingTop ){
   auto content = LabelTTF::create(text, "Arial", fontsize);
    content->setColor(Color3B::WHITE);
    setLabelContentText(content);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
    
}


//给下层层变量和回调函数变量赋值
void PopupLayer::setCallBackFunc( Ref* target, SEL_CallFuncN callfun ){
    m_callbackListener = target;
    m_callback = callfun;
    
}


//给menu菜单变量添加Item
bool PopupLayer::addButton( const char* normalImage, const char* selectedImage, const char* title, int tag ){
    Size winSize = Director::getInstance()->getWinSize();
    Point center = Point(winSize.width/2, winSize.height/2);
    
    // 创建图片菜单按钮
    MenuItemImage* menuImage = MenuItemImage::create(normalImage,
                                                     selectedImage,
                                                     this,
                                                     menu_selector(PopupLayer::buttonCallBack));
    
    menuImage->setTag(tag);
    menuImage->setPosition(center);
    
    
    // 添加文字说明并设置位置
    Size menuSize = menuImage->getContentSize();
    LabelTTF* ttf = LabelTTF::create(title, "Arial", 15);
    ttf->setColor(ccc3(0, 0, 0));
    ttf->setPosition(Point(menuSize.width/2, menuSize.height/2));
    menuImage->addChild(ttf);
    getMenuButton()->addChild(menuImage);
    return true;
    
}
void PopupLayer::show()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->setPosition(Point(visibleSize.width/2-180, visibleSize.height/2));
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Point(1,1));
    //this->setPosition(Director::getInstance()->getWinSize()/2);
    Director::getInstance()->getRunningScene()->addChild(this, 0xffff);
}


//销毁弹出框，传递参数node给下层

void PopupLayer::buttonCallBack( Ref* pSender ){
    Node* node = (Node*)pSender;
    //CCLog("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener)
    {
        //执行HelloWorld层的回调函数，传递node参数
        (m_callbackListener->*m_callback)(node);
    }
    this->removeFromParentAndCleanup(true);
    
}


//全部参数都设定好后，在运行时动态加载
void PopupLayer::onEnter(){
    Layer::onEnter();
    Size winSize = Director::getInstance()->getWinSize();
    Point center = Point(winSize.width/2, winSize.height/2);
    
    Size contentSize;
    // 设定好参数，在运行时加载
    
    if (getContentSize().equals(Size::ZERO))
    {
        getSpriteBackGround()->setPosition(center);
        this->addChild(getSpriteBackGround(),0,0);
        contentSize = getSpriteBackGround()->getTexture()->getContentSize();
        
    }
    
    else{
        Scale9Sprite* background = getSprite9BackGround();
        background->setContentSize(getContentSize());
        background->setPosition(center);
        this->addChild(background, 0);
        contentSize = getContentSize();
        
    }
    
    
    //添加按钮，并根据Item的个数设置其位置
    this->addChild(getMenuButton());
    float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount()+1);
    Vector<Node*>array = getMenuButton()->getChildren();
    
    //Ref* pObj = NULL;
    
 
    for (int i=0; i<array.size(); i++) {
        
        Node* node = (Node*)array.at(i);
        node->setPosition(Point(winSize.width/2 - contentSize.width/2 + btnWidth*(i+1),
                              winSize.height/2 - contentSize.height/3));
    }
    
    
    // 显示对话框标题
    
    if (getLabelTitle()){
        getLabelTitle()->setPosition(ccpAdd(center, Point(0, contentSize.height/2 - 25.0f)));
        this->addChild(getLabelTitle());
    }
    
    
    //显示文本内容
    if (getLabelContentText()){
        LabelTTF* ltf = getLabelContentText();
        ltf->setPosition(center);
        ltf->setDimensions(Size(contentSize.width - m_contentPadding*2, contentSize.height - m_contentPaddingTop));
        ltf->setHorizontalAlignment(TextHAlignment::LEFT);
        this->addChild(ltf);
    }
    
    
    //弹出效果
    Sequence *popupActions = Sequence::create(
                                                  ScaleTo::create(0.0, 0.0),
                                                  ScaleTo::create(0.06, 1.05),
                                                  ScaleTo::create(0.08, 0.95),
                                                  ScaleTo::create(0.08, 1.0), NULL);
    
    this->runAction(popupActions);
    
}


//退出
void PopupLayer::onExit(){
    Layer::onExit();
    
}
