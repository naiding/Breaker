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
    virtual bool onContactBegin(PhysicsContact& contact);

    virtual void update(float delta);
    virtual void onEnter();
    virtual void onExit();
    
private:
    
    bool isStart;
    bool isPause;
    void gameInit();
    void gameStart();
    void gameOver();
    
    Vec2 origin;
    Size visibleSize;
    
    
    int playerTag;
    int ballTag;
    int statusLabelTag;
    
    bool moveLeft;
    bool moveRight;
    float playerMoveSpeed;
    Vec2 ballMoveSpeed;
    void createPlayer(int tag);
    void createBall(int tag);
    void createStatusLabel(int tag);
    Sprite* createBrickAtPosition(Vec2 p, int tag);
    
    void registerKeyboardListener();
    
    int playTimes;
    
    Vector<Sprite*> bricksVec;
};

#endif /* BreakerScene_h */
