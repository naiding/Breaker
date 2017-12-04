//
//  SettingScene.h
//  Breaker
//
//  Created by Naiding Z on 12/3/17.
//

#ifndef SettingScene_h
#define SettingScene_h

#include "cocos2d.h"

USING_NS_CC;

class Setting : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuOkCallback(cocos2d::Ref* pSender);
    void menuSoundToggleCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(Setting);
    
private:
    Vec2 origin;
    Size visibleSize;
    void createBackground(int tag);
    void createMenu(int tag);
};

#endif /* SettingScene_h */
