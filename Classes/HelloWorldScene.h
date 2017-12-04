#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuSettingCallback(cocos2d::Ref* pSender);
    void menuQuitCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void update(float delta);
    
private:
    Vec2 origin;
    Size visibleSize;
    void createBackground(int tag);
    void createWelcomeLabel(int tag);
    void createMenu(int tag);
};

#endif // __HELLOWORLD_SCENE_H__
