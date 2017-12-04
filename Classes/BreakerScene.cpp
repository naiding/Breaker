//
//  BreakerScene.cpp
//  Breaker-mobile
//
//  Created by Naiding Z on 12/3/17.
//

#include "BreakerScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

#define PLAYER_WIDTH 120
#define PLAYER_HEIGHT 30
#define BALL_RADIUS 30

Scene* Breaker::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Breaker::create();
    scene->addChild(layer);
    return scene;
}

bool Breaker::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->playTimes = 0;
    this->playerTag = 123;
    this->ballTag = 456;
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2* edges = new Vec2[4];
    edges[0] = Vec2(-this->visibleSize.width/2, -this->visibleSize.height/2);
    edges[1] = Vec2(-this->visibleSize.width/2, this->visibleSize.height/2);
    edges[2] = Vec2(this->visibleSize.width/2, this->visibleSize.height/2);
    edges[3] = Vec2(this->visibleSize.width/2, -this->visibleSize.height/2);
    
    auto edgesBody = PhysicsBody::createEdgeChain(edges, 4, PhysicsMaterial(0.1f, 1.0f, 0.0f), 5.0f);
    auto edgesNode = Node::create();
    edgesNode->setPosition(Vec2(this->visibleSize.width/2, this->visibleSize.height/2));
    edgesNode->setPhysicsBody(edgesBody);
    this->addChild(edgesNode);

    createPlayer(this->playerTag);
    registerKeyboardListener();
    
    gameInit();
    
    scheduleUpdate();
    
    return true;
}

void Breaker::gameInit()
{
    this->isStart = false;
    this->moveLeft = false;
    this->moveRight = false;
    this->playerMoveSpeed = 800;
    addNewSpriteAtPosition(Vec2(this->visibleSize.width/2, PLAYER_HEIGHT+BALL_RADIUS/2), this->ballTag);
}

void Breaker::gameStart()
{
    if(this->playTimes > 0) gameInit();
    this->playTimes += 1;
    this->isStart = true;
    auto player = this->getChildByTag(this->playerTag);
    player->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->origin.y+PLAYER_HEIGHT/2));
    auto ball = this->getChildByTag(this->ballTag);
    ball->getPhysicsBody()->setVelocity(Vec2(500, 400));
}

void Breaker::gameOver()
{
    this->isStart = false;
    this->removeChildByTag(this->ballTag);
    log("game over");
}

void Breaker::addNewSpriteAtPosition(Vec2 p, int tag)
{
    auto sp = Sprite::create("ball.png");
    sp->setTag(tag);
    sp->setScale(0.1, 0.1);
    auto body = PhysicsBody::createCircle(sp->getContentSize().width/2);
    body->setVelocity(Vec2(0, 0));
    body->setMass(0);
    body->getShape(0)->setDensity(0.1);   //密度
    body->getShape(0)->setFriction(0);    //摩擦系数
    body->getShape(0)->setRestitution(1); //弹力系数
    body->setGravityEnable(false);
    sp->setPhysicsBody(body);
    sp->setPosition(p);
    this->addChild(sp);
}

void Breaker::createPlayer(int tag)
{
    auto player = Sprite::create("player.png", Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
    player->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->origin.y+PLAYER_HEIGHT/2));
    player->setTag(tag);
    
    auto body = PhysicsBody::createBox(player->getContentSize());
    body->setGravityEnable(false);
    body->getShape(0)->setDensity(0.1);   //密度
    body->getShape(0)->setFriction(0);    //摩擦系数
    body->getShape(0)->setRestitution(1); //弹力系数
    body->setDynamic(false);
    player->setPhysicsBody(body);
    
    this->addChild(player, 1);
}

void Breaker::registerKeyboardListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Breaker::keyPressedCallback, this);
    listener->onKeyReleased = CC_CALLBACK_2(Breaker::keyReleasedCallback, this);
    
    EventDispatcher *eventDispatcher = Director::getInstance() -> getEventDispatcher();
    eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
}

void Breaker::keyPressedCallback(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (this->isStart)
    {
        if (int(keyCode) == 26 )
        {
            this->moveLeft = true;
            this->moveRight = false;
        }
        else if (int(keyCode) == 27)
        {
            this->moveLeft = false;
            this->moveRight = true;
        }
        else
        {
            this->moveLeft = false;
            this->moveRight = false;
        }
    }
//    __String *x = __String::createWithFormat("%d", int(keyCode));
//    log(x->getCString());
    if(int(keyCode) == 59 && !isStart) gameStart();
}

void Breaker::keyReleasedCallback(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (this->isStart)
    {
        if (int(keyCode) == 26 ) this->moveLeft = false;
        if (int(keyCode) == 27) this->moveRight = false;
    }
}

void Breaker::update(float delta)
{
    if (this->isStart)
    {
        if(this->moveLeft && !this->moveRight)
        {
            auto player = this->getChildByTag(this->playerTag);
            if(player->getPositionX() - player->getContentSize().width/2 > 0)
                player->setPositionX(player->getPositionX() - delta*this->playerMoveSpeed);
        }
        else if(this->moveRight && !this->moveLeft)
        {
            auto player = this->getChildByTag(this->playerTag);
            if(player->getPositionX() + player->getContentSize().width/2 < this->visibleSize.width)
                player->setPositionX(player->getPositionX() + delta*this->playerMoveSpeed);
        }
        
        auto ball = this->getChildByTag(this->ballTag);
        if (ball->getPositionY() < PLAYER_HEIGHT)
        {
            gameOver();
        }
    }
}

