

#ifndef __LTabelView__LyfIndicator__
#define __LTabelView__LyfIndicator__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

#define BACKGROUNDIMAGE NULL
#define INDICATORIMAGE "ui/indicator.png"
#define OFF_HEIGHT dvalue(2)

USING_NS_CC;
USING_NS_CC_EXT;
class LyfIndicator:public Sprite{
    
public:
    
    LyfIndicator(LayerColor* layerColor);
    
    virtual ~LyfIndicator();
    
    static LyfIndicator* createWithAddChild(LayerColor* layerColor, float scale);
    
    void refresh();
    
    void setIndicator(float windowSize, float contentSize, float lenght);
    
    void setScale(float scale);
    
    void setOpacity(GLubyte opacity);
    
    CC_SYNTHESIZE(LayerColor*, parentNode, ParentNode);
    
    void setPointOff(cocos2d::Vec2 point);
    
private:
    
    cocos2d::Vec2 m_offPoint;
    
    float m_scale;
    
    Scale9Sprite* m_bg;
    
    Scale9Sprite* m_indicator;
    
    float m_indWidth;
    
    float m_indHeight;
    
    bool init();
    
    void initWithIndicator(float scale);
    
    void initWithBg();
    
    void onEnter();
};




#endif /* defined(__LTabelView__LyfIndicator__) */
