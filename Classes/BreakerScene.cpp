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

#define PLAYER_SCALE 0.5
#define PLAYER_WIDTH 500*PLAYER_SCALE
#define PLAYER_HEIGHT 100*PLAYER_SCALE

#define BALL_SCALE 0.25
#define BALL_RADIUS 300*BALL_SCALE

#define BRICK_SCALE 1.2
#define BRICK_WIDTH 160*BRICK_SCALE
#define BRICK_HEIGHT 40*BRICK_SCALE

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
    this->statusLabelTag = 12;
    
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2* edges = new Vec2[4];
    edges[0] = Vec2(-this->visibleSize.width/2, -this->visibleSize.height/2);
    edges[1] = Vec2(-this->visibleSize.width/2, this->visibleSize.height/2);
    edges[2] = Vec2(this->visibleSize.width/2, this->visibleSize.height/2);
    edges[3] = Vec2(this->visibleSize.width/2, -this->visibleSize.height/2);
    
    auto edgesBody = PhysicsBody::createEdgeChain(edges, 4, PhysicsMaterial(0.0f, 1.0f, 0.0f), 5.0f);
    auto edgesNode = Node::create();
    edgesNode->setPosition(Vec2(this->visibleSize.width/2, this->visibleSize.height/2));
    edgesNode->setPhysicsBody(edgesBody);
    this->addChild(edgesNode);

    createPlayer(this->playerTag);
    createStatusLabel(this->statusLabelTag);
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
        for (int j = 1; j <= 8; j++)
        {
            Vec2 point = Vec2(this->origin.x + 60 + (BRICK_WIDTH/BRICK_SCALE-40)*j, this->origin.y + 450 + (BRICK_HEIGHT/BRICK_SCALE-6)*i);
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
    ball->getPhysicsBody()->setVelocity(Vec2(cocos2d::RandomHelper::random_int(200, 300), cocos2d::RandomHelper::random_int(200, 300)));
    auto statusLabel = this->getChildByTag(this->statusLabelTag);
    
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
    sp->setScale(BALL_SCALE, BALL_SCALE);
    auto body = PhysicsBody::createCircle(sp->getContentSize().width/2);
    body->setVelocity(Vec2(0, 0));
    body->setMass(0);
    body->getShape(0)->setDensity(0.1);
    body->getShape(0)->setFriction(0);
    body->getShape(0)->setRestitution(1);
    body->setGravityEnable(false);
    body->setContactTestBitmask(0xFFFFFFFF);
    sp->setPhysicsBody(body);
    sp->setPosition(Vec2(this->visibleSize.width/2, (PLAYER_HEIGHT+BALL_RADIUS)/2));
    this->addChild(sp);
}

void Breaker::createPlayer(int tag)
{
    auto player = Sprite::create("player.png");
    player->setScale(PLAYER_SCALE, PLAYER_SCALE);
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

Sprite* Breaker::createBrickAtPosition(Vec2 p, int tag)
{
    auto brick = Sprite::create("brick.png");
    brick->setPosition(p);
    brick->setScale(BRICK_SCALE, BRICK_SCALE);
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

void Breaker::createStatusLabel(int tag)
{
    auto status = Label::createWithSystemFont("Tap Space to Start", "Arial", 13);
    status->setAnchorPoint(Vec2(0, 0));
    status->setPosition(Vec2(0, 0));
    status->setTag(tag);
    this->addChild(status);
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
            if(player->getPositionX() - (PLAYER_WIDTH-100)/2 > 0)
                player->setPositionX(player->getPositionX() - delta*this->playerMoveSpeed);
        }
        else if(this->moveRight && !this->moveLeft)
        {
            auto player = this->getChildByTag(this->playerTag);
            if(player->getPositionX() + (PLAYER_WIDTH-100)/2 < this->visibleSize.width)
                player->setPositionX(player->getPositionX() + delta*this->playerMoveSpeed);
        }
        
        auto ball = this->getChildByTag(this->ballTag);
        if (ball->getPositionY() < PLAYER_HEIGHT)
        {
            gameOver();
        }
    }
}
