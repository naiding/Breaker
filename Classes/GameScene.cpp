//
//  GameScene.cpp
//  Breaker
//
//  Created by Naiding Z on 12/3/17.
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* Game::createScene()
{
    return Game::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Game::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    scheduleUpdate();

    this->moveLeft = false;
    this->moveRight = false;
    this->playerMoveSpeed = 400;
    
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();
    
    createBackground(1);
    createPlayer(2);
    createBall(3);
    
    registerKeyboardListener();

    return true;
}

void Game::createBall(int tag)
{    
    auto ball = Sprite::create("ball.png");
    ball->setTag(tag);
    if (ball == nullptr) problemLoading("'ball.png'");
    else
    {
//        __String *x = __String::createWithFormat("%f", ball->getContentSize().width);
//        log(x->getCString());
        ball->setPosition(Vec2(this->origin.x + this->visibleSize.width/2,
                               this->origin.y + 35));
        ball->setScale(0.1, 0.1);
        this->addChild(ball, 1);
    }
}

void Game::createPlayer(int tag)
{
    auto player = Sprite::create("player.png", Rect(0, 0, 120, 40));
    player->setTag(tag);
    if (player == nullptr) problemLoading("'player.png'");
    else
    {
        player->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->origin.y));
        this->addChild(player, 1);
    }
}

void Game::createBackground(int tag)
{
    auto background = Sprite::create("Background.png", Rect(0, 0, this->visibleSize.width, this->visibleSize.height));
    background->setTag(tag);
    if (background == nullptr) problemLoading("'Background.png'");
    else
    {
        background->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->visibleSize.height/2 + this->origin.y));
        this->addChild(background, 0);
    }
}

void Game::registerKeyboardListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Game::keyPressedCallback, this);
    listener->onKeyReleased = CC_CALLBACK_2(Game::keyReleasedCallback, this);
    
    EventDispatcher *eventDispatcher = Director::getInstance() -> getEventDispatcher();
    eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
}

void Game::keyPressedCallback(EventKeyboard::KeyCode keyCode, Event *event)
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

void Game::keyReleasedCallback(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (int(keyCode) == 26 ) this->moveLeft = false;
    if (int(keyCode) == 27) this->moveRight = false;
}

void Game::update(float delta)
{
    if(this->moveLeft && !this->moveRight)
    {
        auto player = this->getChildByTag(2);
        if(player->getPositionX() - player->getContentSize().width/2 > 0)
            player->setPositionX(player->getPositionX() - delta*this->playerMoveSpeed);
    }
    else if(this->moveRight && !this->moveLeft)
    {
        auto player = this->getChildByTag(2);
        if(player->getPositionX() + player->getContentSize().width/2 < this->visibleSize.width)
            player->setPositionX(player->getPositionX() + delta*this->playerMoveSpeed);
    }
}
