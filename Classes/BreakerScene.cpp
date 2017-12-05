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
#define BRICK_WIDTH 80
#define BRICK_HEIGHT 20

Scene* Breaker::createScene()
{
    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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
    this->playerTag = 10;
    this->ballTag = 11;
    
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
    gameInit();
    scheduleUpdate();
    
    return true;
}

void Breaker::onEnter()
{
    Layer::onEnter();
    
    registerKeyboardListener();
    
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = CC_CALLBACK_1(Breaker::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    
    log("On Enter");
}

void Breaker::onExit()
{
    Layer::onExit();
    log("On Exit");
    
    EventDispatcher *eventDispatcher = Director::getInstance() -> getEventDispatcher();
    eventDispatcher -> removeAllEventListeners();
}

bool Breaker::onContactBegin(cocos2d::PhysicsContact &contact)
{
    log("on Contact Begin");
    auto spriteA = (Sprite *)contact.getShapeA()->getBody()->getNode();
    auto spriteB = (Sprite *)contact.getShapeB()->getBody()->getNode();
    
    for(auto brick : this->bricksVec)
    {
        if (brick->getTag() == spriteA->getTag())
        {
            this->removeChildByTag(spriteA->getTag());
            this->bricksVec.eraseObject(brick);
        }
        if (brick->getTag() == spriteB->getTag())
        {
            this->removeChildByTag(spriteB->getTag());
            this->bricksVec.eraseObject(brick);
        }
    }
    
//    __String *x = __String::createWithFormat("%d", spriteA->getTag());
//    log(x->getCString());

    return true;
}

void Breaker::gameInit()
{
    this->isStart = false;
    this->isPause = false;
    this->moveLeft = false;
    this->moveRight = false;
    this->playerMoveSpeed = 800;
    createBall(this->ballTag);
    
    for(int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            Vec2 point = Vec2(this->origin.x + 40 + (BRICK_WIDTH+5)*j, this->origin.y + 400 + (BRICK_HEIGHT+3)*i);
            Sprite* brick = createBrickAtPosition(point, 20+i*10+j);
            this->bricksVec.insert(0, brick);
        }
    }
}

void Breaker::gameStart()
{
    if(this->playTimes > 0) gameInit();
    this->playTimes += 1;
    this->isStart = true;
    this->isPause = false;
    auto player = this->getChildByTag(this->playerTag);
    player->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->origin.y+PLAYER_HEIGHT/2));
    auto ball = this->getChildByTag(this->ballTag);
    ball->getPhysicsBody()->setVelocity(Vec2(cocos2d::RandomHelper::random_int(100, 200), cocos2d::RandomHelper::random_int(100, 200)));
}

void Breaker::gameOver()
{
    this->isStart = false;
    this->isPause = false;
    this->removeChildByTag(this->ballTag);
    for(auto brick : this->bricksVec)
    {
        this->removeChildByTag(brick->getTag());
    }
    this->bricksVec.clear();
    
    log("Game Over");
}

void Breaker::createBall(int tag)
{
    auto sp = Sprite::create("ball.png");
    sp->setTag(tag);
    sp->setScale(0.1, 0.1);
    auto body = PhysicsBody::createCircle(sp->getContentSize().width/2);
    body->setVelocity(Vec2(0, 0));
    body->setMass(0);
    body->getShape(0)->setDensity(0.1);
    body->getShape(0)->setFriction(0);
    body->getShape(0)->setRestitution(1);
    body->setGravityEnable(false);
    body->setContactTestBitmask(0xFFFFFFFF);
    sp->setPhysicsBody(body);
    sp->setPosition(Vec2(this->visibleSize.width/2, PLAYER_HEIGHT+BALL_RADIUS/2));
    this->addChild(sp);
}

Sprite* Breaker::createBrickAtPosition(Vec2 p, int tag)
{
    auto brick = Sprite::create("brick.png", Rect(0, 0, BRICK_WIDTH, BRICK_HEIGHT));
    brick->setPosition(p);
    brick->setTag(tag);
    
    auto body = PhysicsBody::createBox(brick->getContentSize());
    body->setGravityEnable(false);
    body->getShape(0)->setDensity(0.1);
    body->getShape(0)->setFriction(0);
    body->getShape(0)->setRestitution(1);
    body->setDynamic(false);
    body->setContactTestBitmask(0xFFFFFFFF);
    brick->setPhysicsBody(body);
    
    this->addChild(brick, 1);
    
    return brick;
}

void Breaker::createPlayer(int tag)
{
    auto player = Sprite::create("player.png");
    player->setScale(0.35, 0.35);
    player->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->origin.y+PLAYER_HEIGHT/2));
    player->setTag(tag);
    
    auto body = PhysicsBody::createBox(player->getContentSize());
    body->setGravityEnable(false);
    body->getShape(0)->setDensity(0.1);
    body->getShape(0)->setFriction(0);
    body->getShape(0)->setRestitution(1);
    body->setDynamic(false);
    body->setContactTestBitmask(0xFFFFFFFF);
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
    if (this->isStart && !this->isPause)
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
    if(int(keyCode) == 59 && !this->isStart)
    {
        log("Start !!");
        gameStart();
    }
    else if(int(keyCode) == 59 && this->isStart && !this->isPause)
    {
        log("Pause !!");
        this->isPause = true;
        auto ball = this->getChildByTag(this->ballTag);
        this->ballMoveSpeed = ball->getPhysicsBody()->getVelocity();
        ball->getPhysicsBody()->setVelocity(Vec2(0, 0));

    }
    else if(int(keyCode) == 59 && this->isStart && this->isPause)
    {
        log("Resume !!");
        this->isPause = false;
        auto ball = this->getChildByTag(this->ballTag);
        ball->getPhysicsBody()->setVelocity(this->ballMoveSpeed);
    }
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
    if (this->isStart && !this->isPause)
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

