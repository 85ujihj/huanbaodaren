//
//  HRocker.cpp
//  guyue
//
//  Created by dsh on 15/1/13.
//
//

#include "HRocker.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
const double PI=3.1415;
HRocker::HRocker(void)
{
    //rocketRun=false;
}

HRocker::~HRocker(void)
{
}
void HRocker::setJoystickPositionChangeHandler(JoystickHandler handler)
{
    _handler = handler;
}

//创建摇杆(摇杆的操作题图片资源名，摇杆背景图片资源名，起始坐标)
HRocker* HRocker::createHRocker(const char *rockerImageName,const char *rockerBGImageName,Point position)
{
    HRocker *layer = HRocker::create();
    if (layer)
    {
        layer->rockerInit(rockerImageName,rockerBGImageName,position);
        //layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

//自定义初始化函数
void HRocker::rockerInit(const char* rockerImageName,const char* rockerBGImageName,Point position)
{
    Sprite *spRockerBG = Sprite::create(rockerBGImageName);
    spRockerBG->setPosition(position);
    spRockerBG->setVisible(false);
    addChild(spRockerBG,0,tag_rockerBG);
    
    Sprite *spRocker = Sprite::create(rockerImageName);
    spRocker->setPosition(position);
    spRocker->setVisible(false);
    addChild(spRocker,1,tag_rocker);
    
    rockerBGPosition = position;
    rockerBGR = spRockerBG->getContentSize().width*0.4;//
    rocketDirection=-1;//表示摇杆方向不变
}

//启动摇杆(显示摇杆、监听摇杆触屏事件)
void HRocker::startRocker(bool _isStopOther)
{
    Sprite *rocker = (Sprite*)this->getChildByTag(tag_rocker);
    rocker->setVisible(true);
    
    Sprite *rockerBG = (Sprite *)this->getChildByTag(tag_rockerBG);
    //rockerBG->setVisible(true);
    rockerBG->setVisible(false);
    
 
    EventListenerTouchOneByOne * _touchEventListener=EventListenerTouchOneByOne::create();
    _touchEventListener->onTouchBegan=CC_CALLBACK_2(HRocker::onTouchBegan, this);
    _touchEventListener->onTouchMoved=CC_CALLBACK_2(HRocker::onTouchMoved, this);
    _touchEventListener->onTouchEnded=CC_CALLBACK_2(HRocker::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);
    
}

//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
//void HRocker::stopRocker()
//{
//    CCSprite *rocker = (CCSprite *)this->getChildByTag(tag_rocker);
//    rocker->setVisible(false);
//    
//    CCSprite * rockerBG = (CCSprite *)this->getChildByTag(tag_rockerBG);
//    rockerBG->setVisible(false);
//    //Director::getInstance()->getEventDispatcher()->removeEventListener(this);
//    // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//}


//获取当前摇杆与用户触屏点的角度
float HRocker::getRad(Point pos1,Point pos2)
{
    float px1 = pos1.x;
    float py1 = pos1.y;
    float px2 = pos2.x;
    float py2 = pos2.y;
    
    //得到两点x的距离
    float x = px2 - px1;
    //得到两点y的距离
    float y = py1 - py2;
    //算出斜边长度
    float xie = sqrt(pow(x,2) + pow(y,2));
    //得到这个角度的余弦值(通过三角函数中的店里：角度余弦值=斜边/斜边)
    float cosAngle = x / xie;
    //通过反余弦定理获取到期角度的弧度
    float rad = acos(cosAngle);
    //注意：当触屏的位置Y坐标<摇杆的Y坐标，我们要去反值-0~-180
    if (py2 < py1)
    {
        rad = -rad;
    }
    return rad;
}

Point getAngelePosition(float r,float angle){
    return Point(r*cos(angle),r*sin(angle));
}

//抬起事件
bool HRocker::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    Sprite *rocker = (Sprite *)this->getChildByTag(tag_rocker);
    if (rocker->boundingBox().containsPoint(point))
    {
        isCanMove = true;
       // CCLOG("begin true");
    }
    else
    {
       // CCLOG("begin false");
        isCanMove = false;
    }
    return isCanMove;
}
//移动事件
void HRocker::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!isCanMove)
    {
        return;
    }
    Point point = pTouch->getLocation();
    Sprite *rocker = (Sprite *)this->getChildByTag(tag_rocker);
    //得到摇杆与触屏点所形成的角度
    float angle = getRad(rockerBGPosition,point);
    //判断两个圆的圆心距是否大于摇杆背景的半径
    if (sqrt(pow((rockerBGPosition.x - point.x),2) + pow((rockerBGPosition.y - point.y),2)) >= rockerBGR)
    {
        
        //保证内部小圆运动的长度限制
        rocker->setPosition(getAngelePosition(rockerBGR,angle)+Point(rockerBGPosition.x,rockerBGPosition.y));
        //  CCLOG("touch");
    }
    else
    {
        //当没有超过，让摇杆跟随用户触屏点移动即可
        rocker->setPosition(point);
        //CCLOG("touch");
    }
    
    //判断方向
    this->updateDirectionForTouchLocation(point);
    
    
    
}
// 计算触摸点到方向键中心距离值，转换成角度，得到正确的方向值，然后传递值到委托
void HRocker::updateDirectionForTouchLocation(Point location)
{
    /*
    float radians = (location-rockerBGPosition).getAngle();
    float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);
    
    if (degrees <= 22.5 && degrees >= -22.5)
    {
        //right
        m_sDirection = Point(1.0, 0.0);
        rocketDirection=2;
    }
    else if (degrees > 22.5 && degrees < 67.5)
    {
        //bottomright
        m_sDirection = Point(1.0, -1.0);
        rocketDirection=3;
    }
    else if (degrees >= 67.5 && degrees <= 112.5)
    {
        //bottom
        m_sDirection = Point(0.0, -1.0);
        rocketDirection=4;
    }
    else if (degrees > 112.5 && degrees < 157.5)
    {
        //bottomleft
        m_sDirection = Point(-1.0, -1.0);
        rocketDirection=5;
    }
    else if (degrees >= 157.5 || degrees <= -157.5)
    {
        //left
        m_sDirection = Point(-1.0, 0.0);
        rocketDirection=6;
    }
    else if (degrees < -22.5 && degrees > -67.5)
    {
        //topright
        m_sDirection = Point(1.0, 1.0);
        rocketDirection=1;
    }
    else if (degrees <= -67.5 && degrees >= -112.5)
    {
        //top
        m_sDirection = Point(0.0, 1.0);
        rocketDirection=8;
    }
    else if (degrees < -112.5 && degrees > -157.5)
    {
        //topleft
        m_sDirection = Point(-1.0, 1.0);
        rocketDirection=7;
    }
     */
    m_sDirection = ccpSub(location,this->getPosition());
    Point t_direction = m_sDirection.getNormalized();
    
    if (_handler)
        _handler(m_sDirection);
    

}


//离开事件
void HRocker::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!isCanMove)
    {
        return;
    }
    Sprite *rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
    Sprite *rocker = (Sprite*)this->getChildByTag(tag_rocker);
    rocker->stopAllActions();
    rocker->runAction(CCMoveTo::create(0.08f,rockerBG->getPosition()));
    isCanMove = false;
    m_sDirection = Vec2::ZERO;
    //rocketDirection=0;
    //CCLOG("%d",rocketDirection);
    //CCLOG("end");
}
//void HRocker::update(float dt)
//{
//    //判断是否按下摇杆及其类型
//    //    switch(rocker->rocketDirection)
//    //    {
//    //        case 1:
//    //            hero->SetAnimation("run_animation.plist","run_animation.png","run_",8,rocker->rocketRun);//"run_"为run_animation.png集合图片中每张图片的公共名称部分
//    //            hero->setPosition(ccp(hero->getPosition().x+1,hero->getPosition().y)); //向右走
//    //            break;
//    //        case  2:
//    //            hero->SetAnimation("run_animation.plist","run_animation.png","run_",8,rocker->rocketRun);//"run_"为run_animation.png集合图片中每张图片的公共名称部分
//    //            hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y+1));   //向上走
//    //            break;
//    //        case 3:
//    //            hero->SetAnimation("run_animation.plist","run_animation.png","run_",8,rocker->rocketRun);//"run_"为run_animation.png集合图片中每张图片的公共名称部分
//    //            hero->setPosition(ccp(hero->getPosition().x-1,hero->getPosition().y));   //向左走
//    //            break;
//    //        case 4:
//    //            hero->SetAnimation("run_animation.plist","run_animation.png","run_",8,rocker->rocketRun);//"run_"为run_animation.png集合图片中每张图片的公共名称部分
//    //            hero->setPosition(ccp(hero->getPosition().x,hero->getPosition().y-1));   //向下走
//    //            break;
//    //        default:
//    //            hero->StopAnimation();//停止所有动画和运动
//    //            break;
//    //            
//    //    }
//}
