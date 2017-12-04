//
//  BreakerScene.h
//  Breaker
//
//  Created by Naiding Z on 12/3/17.
//

#ifndef BreakerScene_h
#define BreakerScene_h

#include "cocos2d.h"
USING_NS_CC;

class Breaker : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Breaker);
    
    void keyPressedCallback(EventKeyboard::KeyCode keyCode, Event *event);
    void keyReleasedCallback(EventKeyboard::KeyCode keyCode, Event *event);
    
    virtual void update(float delta);
    
private:
    
    bool isStart;
    void gameInit();
    void gameStart();
    void gameOver();
    
    Vec2 origin;
    Size visibleSize;
    void addNewSpriteAtPosition(Vec2 p, int tag);
    
    int playerTag;
    int ballTag;
    
    bool moveLeft;
    bool moveRight;
    float playerMoveSpeed;
    void createPlayer(int tag);
    
    void registerKeyboardListener();
    
    int playTimes;
};

#endif /* BreakerScene_h */
