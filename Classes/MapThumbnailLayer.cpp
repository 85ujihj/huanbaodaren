

#include "MapThumbnailLayer.h"
#include "GameScene.h"

#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect

const Point centerPoint = Point(72.5, 93);
const int lenght = 60;
const int scale = 1000;

static Sprite* createIndicator(unsigned int size)
{
	int pixels[size][size];
    
	for (int i = 0; i < size; ++i)
    {
		for (int j = 0; j < size; ++j)
        {
			pixels[i][j] = 0xffffffff;
		}
	}
    
	Texture2D *texture = new Texture2D();
	texture->initWithData(pixels,20, Texture2D::PixelFormat::RGB888, 1, 1,
                          Size(size, size));
    Sprite* sprite = Sprite::createWithTexture(texture);
    texture->release();
    return sprite;
}

MapThumbnailMenu::MapThumbnailMenu()
{
    m_enemyDic = new CCDictionary();
}

MapThumbnailMenu::~MapThumbnailMenu()
{
    CC_SAFE_DELETE(m_enemyDic);
}

MapThumbnailMenu* MapThumbnailMenu::create()
{
    MapThumbnailMenu* sprite = new MapThumbnailMenu();
    if (sprite && sprite->initWithFile("ui/mapThumbnail.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void MapThumbnailMenu::onEnter()
{
    Sprite::onEnter();
 
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(MapThumbnailMenu::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(MapThumbnailMenu::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(MapThumbnailMenu::onTouchEnded, this);
    listener->onTouchCancelled=CC_CALLBACK_2(MapThumbnailMenu::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->schedule(schedule_selector(MapThumbnailMenu::update), 0.04);
}

void MapThumbnailMenu::onExit()
{
    Sprite::onExit();
}

bool MapThumbnailMenu::initWithFile(const char *pszFilename)
{
    if (!Sprite::initWithFile(pszFilename))
    {
        return false;
    }

    Sprite* bg = CCSprite::create("ui/bg_mapThumbnail.png");
    bg->setAnchorPoint(Point::ZERO);
    this->addChild(bg,-2);
    
    m_playerIndicator = createIndicator(4);
    m_playerIndicator->setPosition(centerPoint);
    this->addChild(m_playerIndicator, -1);
    
    m_pCoordinateTTF = CCLabelTTF::create("", "Helvetica-Bold", 12);
    m_pCoordinateTTF->setPosition(Point(centerPoint.x, 17));
    this->addChild(m_pCoordinateTTF);
    
    return true;
}

void MapThumbnailMenu::update(float delay)
{
    CCArray* enemyKeys = GAME_SCENE->getEnemyDictionary()->allKeys();
    
    if (enemyKeys == NULL)
        return;
    
    for (vector<int>::iterator itr = keyVec.begin(); itr!=keyVec.end(); itr++)
    {
        Enemy* enemy = (Enemy*)GAME_SCENE->getEnemyDictionary()->objectForKey(*itr);
        
        if (!enemy)
        {
            Sprite* enemyIndicator = (Sprite*)m_enemyDic->objectForKey(*itr);
            m_enemyDic->removeObjectForKey(*itr);
            enemyIndicator->removeFromParent();
            continue;
        }
        unsigned int distance = ccpDistance(Player::sharePlayer()->getPosition(), enemy->getPosition());
        if (distance > scale)
        {
            Sprite* enemyIndicator = (Sprite*)m_enemyDic->objectForKey(*itr);
            m_enemyDic->removeObjectForKey(*itr);
            enemyIndicator->removeFromParent();
        }
        
    }
    keyVec.clear();
    for (unsigned int i=0; i<enemyKeys->count(); i++)
    {
        int key = ((CCInteger*)enemyKeys->objectAtIndex(i))->getValue();
        Enemy* enemy = (Enemy*)GAME_SCENE->getEnemyDictionary()->objectForKey(key);
        unsigned int distance = ccpDistance(Player::sharePlayer()->getPosition(), enemy->getPosition());
        if (distance <= scale)
        {
            keyVec.push_back(key);
            int distanceX = enemy->getPositionX() - Player::sharePlayer()->getPositionX();
            int distanceY = enemy->getPositionY() - Player::sharePlayer()->getPositionY();
            float x = lenght * distanceX/(float)scale + centerPoint.x;
            float y = lenght * distanceY/(float)scale + centerPoint.y;
            Sprite* enemyIndicator = (Sprite*)m_enemyDic->objectForKey(key);
            if (enemyIndicator == NULL)
            {
                enemyIndicator = createIndicator(2);
                enemyIndicator->setColor(ccRED);
                this->addChild(enemyIndicator, -1);
                m_enemyDic->setObject(enemyIndicator, key);
            }
            enemyIndicator->setPosition(Point(x, y));
        }
    }
    Point point = Player::sharePlayer()->getPosition()/10;
    char str[32];
    sprintf(str, "X:%03d     Y:%03d", (int)point.x, (int)point.y);
    m_pCoordinateTTF->setString(str);
}

bool MapThumbnailMenu::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point=this->convertToNodeSpace(point);
    
    if (ccpDistance(point, centerPoint) <= lenght)
    {
        return true;
    }
    
    return false;
}

void MapThumbnailMenu::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    
}

void MapThumbnailMenu::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (GAME_SCENE->getGameInfoUIController()->getChildByTag(0xff00f))
    {
        return;
    }
    MapThumbnailLayer* map = MapThumbnailLayer::create();
    GAME_SCENE->getGameInfoUIController()->addChild(map, 0 , 0xff00f);
}

void MapThumbnailMenu::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}



bool MapThumbnailLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite* bg = Sprite::create("ui/bg_map_layer.png");
    bg->setPosition(winSize/2);
    this->addChild(bg);
    
    m_nMap = MapThumbnailScrollView::create();
    m_nMap->setPosition(Point(43, 43));
    bg->addChild(m_nMap);
    
    
    ControlButton* button = ControlButton::create(Scale9Sprite::create("ui/closed_normal.png"));
    button->setBackgroundSpriteForState(Scale9Sprite::create("ui/closed_selected.png"), Control::State::HIGH_LIGHTED);
    button->setPreferredSize(Size(57, 58));
    button->setPosition(ccpSub(ccpAdd(bg->getPosition(), bg->getContentSize()/2), button->getContentSize()/2));
    this->addChild(button);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(MapThumbnailLayer::removeSelf), Control::EventType::TOUCH_UP_INSIDE);
   // button->setTouchPriority(0);
    
    //this->setTouchEnabled(true);
    
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(MapThumbnailLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void MapThumbnailLayer::removeSelf(Ref* obj, Control::EventType)
{
    this->removeFromParentAndCleanup(true);
}


bool MapThumbnailLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{

    return true;
}



MapThumbnailScrollView::MapThumbnailScrollView()
:m_bIsMoved(false)
,m_beginPoint(Point::ZERO)
{

}

MapThumbnailScrollView::~MapThumbnailScrollView()
{

}


MapThumbnailScrollView* MapThumbnailScrollView::create()
{
    MapThumbnailScrollView* scrollView = new MapThumbnailScrollView();
    if (scrollView && scrollView->initWithViewSize(Size(600, 480)))
    {
        scrollView->initWithMap();
        scrollView->autorelease();
        return scrollView;
    }
    CC_SAFE_DELETE(scrollView);
    return NULL;
}

void MapThumbnailScrollView::initWithMap()
{
    this->setAnchorPoint(CCPointZero);
    this->setBounceable(false);
    
    Texture2D* texture = GAME_SCENE->getCurrBgMap()->getSmallMap()->getTexture();
    Sprite* sprite = CCSprite::createWithTexture(texture);
    sprite->setAnchorPoint(Point::ZERO);
    sprite->setPosition(Point::ZERO);
    this->addChild(sprite);
    this->setContentSize(sprite->getContentSize());
    
    m_pEndPoint = createIndicator(8);
    m_pEndPoint->setColor(ccYELLOW);
    //m_pContainer->addChild(m_pEndPoint);
    _container->addChild(m_pEndPoint);
    m_pEndPoint->setPosition(Point(-100, -100));
    
    Blink* blink = Blink::create(0.5f, 1);
    RepeatForever* repeatForever = RepeatForever::create(blink);
    m_pEndPoint->runAction(repeatForever);
    
    m_playerIndicator = CCSprite::create("ui/self_indicator.png");
    _container->addChild(m_playerIndicator);

    float x = Player::sharePlayer()->getPositionX() * _container->getContentSize().width / GAME_SCENE->getCurrBgMap()->getBgSize().width;
    float y = Player::sharePlayer()->getPositionY() * _container->getContentSize().height / GAME_SCENE->getCurrBgMap()->getBgSize().height;
    x -= this->getViewSize().width / 2;
    y -= this->getViewSize().height / 2;
    x = MIN(x, this->getContentSize().width - this->getViewSize().width);
    x = MAX(x, 0);
    y = MIN(y, this->getContentSize().height - this->getViewSize().height);
    y = MAX(y, 0);
    _container->setPosition(Point(-x, -y));
    
    
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(MapThumbnailScrollView::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(MapThumbnailScrollView::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(MapThumbnailScrollView::onTouchEnded, this);
    //listener->onTouchCancelled=CC_CALLBACK_2(MapThumbnailScrollView::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->scheduleUpdate();
}

void MapThumbnailScrollView::update(float delay)
{
    float x = Player::sharePlayer()->getPositionX() * _container->getContentSize().width / GAME_SCENE->getCurrBgMap()->getBgSize().width;
    float y = Player::sharePlayer()->getPositionY() * _container->getContentSize().height / GAME_SCENE->getCurrBgMap()->getBgSize().height;
    m_playerIndicator->setPosition(Point(x, y));
}

bool MapThumbnailScrollView::onTouchBegan(Touch *pTouch, Event *pEvent){
    
    m_beginPoint = pTouch->getLocation();
    return ScrollView::onTouchBegan(pTouch, pEvent);
}


void MapThumbnailScrollView::onTouchMoved(Touch *pTouch, Event *pEvent){
    ScrollView::onTouchMoved(pTouch, pEvent);
    
    if (ccpDistance(m_beginPoint, pTouch->getLocation()) < 5)
    {
        m_bIsMoved = true;
    }
}

void MapThumbnailScrollView::onTouchEnded(Touch *pTouch, Event *pEvent){
    
    ScrollView::onTouchEnded(pTouch, pEvent);
    
    do
    {
        if (m_bIsMoved)
        {
            m_bIsMoved = false;
            break;
        }
        
        Point point = pTouch->getLocation();
        point=_container->convertToNodeSpace(point);
        m_pEndPoint->setPosition(point);
        float x = point.x * GAME_SCENE->getCurrBgMap()->getBgSize().width / _container->getContentSize().width;
        float y = point.y * GAME_SCENE->getCurrBgMap()->getBgSize().height / _container->getContentSize().height;
        
        Player::sharePlayer()->goTo(MapPoint(Point(x, y)));
        
    }
    while (0);
}
