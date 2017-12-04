//
//  GameScene.h
//  Breaker
//
//  Created by Naiding Z on 12/3/17.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"

USING_NS_CC;

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Game);
    
    void keyPressedCallback(EventKeyboard::KeyCode keyCode, Event *event);
    void keyReleasedCallback(EventKeyboard::KeyCode keyCode, Event *event);
    
    virtual void update(float delta);
    
private:
    Vec2 origin;
    Size visibleSize;
    bool moveLeft;
    bool moveRight;
    float playerMoveSpeed;
    void createBackground(int tag);
    void createPlayer(int tag);
    void createBall(int tag);
    void registerKeyboardListener();
};

#endif /* GameScene_h */
