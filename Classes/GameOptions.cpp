

#include "GameOptions.h"
#include "GameScene.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
void GameOptions::onEnterTransitionDidFinish()
{
    CCNode::onEnterTransitionDidFinish();
    
    m_pBg = Sprite::create("ui/options.png");
    m_pBg->setPosition(Vec2::ZERO);
    this->addChild(m_pBg, -1);
    
    ControlButton* button = ControlButton::create(Scale9Sprite::create("ui/closed_normal.png"));
    button->setBackgroundSpriteForState(Scale9Sprite::create("ui/closed_selected.png"), Control::State::HIGH_LIGHTED);
    button->setPreferredSize(Size(57, 58));
    button->setPosition(ccpSub(ccpAdd(m_pBg->getPosition(), m_pBg->getContentSize()), button->getContentSize()/2));
    m_pBg->addChild(button);
    
    button->addTargetWithActionForControlEvents(GAME_UILAYER,
                                                cccontrol_selector(GameInfoUIController::removeBigMenuAndButton),
                                                Control::EventType::TOUCH_UP_INSIDE );
    
    
  //  button->setTouchPriority(0);
}
