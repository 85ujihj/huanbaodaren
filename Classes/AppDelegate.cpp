#include "AppDelegate.h"
#include "LoginScene.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
//    float width = Director::getInstance()->getWinSize().width;
//    float height = Director::getInstance()->getWinSize().height;
//    
//    float proportion = width / height;
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC ||         \
//CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 ||       \
//CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//    
//    if (proportion > 1.5f)
//    {
//        height = MIN(height, 800);
//        width = height * proportion;
//    }
//    else
//    {
//        width = MIN(width, 1200);
//        height = width / proportion;
//    }
//    glview->setDesignResolutionSize(width, height, kResolutionShowAll);
//    
//#else
//    
//    if (width < 960 || height < 640)
//    {
//        if (proportion > 1.5f)
//        {
//            height = MAX(height, 640);
//            width = height * proportion;
//        }
//        else
//        {
//            width = MAX(width, 960);
//            height = width / proportion;
//        }
//        glview->setDesignResolutionSize(width, height, ResolutionPolicy::SHOW_ALL);
//    }
//    else if (width > 960 || height > 640)
//    {
//        if (proportion > 1.5f)
//        {
//            height = MIN(height, 640);
//            width = height * proportion;
//        }
//        else
//        {
//            width = MIN(width, 960);
//            height = width / proportion;
//        }
//        glview->setDesignResolutionSize(width, height, ResolutionPolicy::SHOW_ALL);
//    }
//    
//#endif
    glview->setDesignResolutionSize(960, 640, ResolutionPolicy::EXACT_FIT);

    
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);

    // create a scene. it's an autorelease object
    auto scene = LoginScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
