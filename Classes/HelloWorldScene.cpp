#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "SettingScene.h"
#include "GameScene.h"
#include "BreakerScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    scheduleUpdate();

    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();

    // 2. add Background
    createBackground(1);
    
    // 3. add Welcome label
    createWelcomeLabel(2);
    
    // 4. add Menu
    createMenu(3);

    return true;
}

void HelloWorld::update(float delta)
{
//    auto label = this->getChildByTag(123);
//    label->setPosition(label->getPosition() + Vec2(1, -1));
}

void HelloWorld::createBackground(int tag)
{
    auto background = Sprite::create("Background.png", Rect(0, 0, this->visibleSize.width, this->visibleSize.height));
    background->setTag(tag);
    if (background == nullptr)
    {
        problemLoading("'Background.png'");
    }
    else
    {
        background->setPosition(Vec2(this->visibleSize.width/2 + this->origin.x, this->visibleSize.height/2 + this->origin.y));
        this->addChild(background, 0);
    }
}

void HelloWorld::createWelcomeLabel(int tag)
{
    auto label = Label::createWithTTF("Welcome to Breaker !!", "fonts/Marker Felt.ttf", 40);
//    auto label = Label::createWithSystemFont("Welcome to Breaker !!", "Arial", 40);
    label->setTag(tag);
    label->setColor(Color3B(26,83,255));
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(Vec2(this->origin.x + this->visibleSize.width/2, this->origin.y + 3*this->visibleSize.height/4));
        this->addChild(label, 1);
    }
}

void HelloWorld::createMenu(int tag)
{
    MenuItemFont::setFontName("Hercunalum");
    MenuItemFont::setFontSize(30);
    
    MenuItemFont *start = MenuItemFont::create("Start", CC_CALLBACK_1(HelloWorld::menuStartCallback, this));
    MenuItemFont *setting = MenuItemFont::create("Setting", CC_CALLBACK_1(HelloWorld::menuSettingCallback, this));
    MenuItemFont *quit = MenuItemFont::create("Quit", CC_CALLBACK_1(HelloWorld::menuQuitCallback, this));
    
    Menu *menu = Menu::create(start, setting, quit, NULL);
    menu->alignItemsVertically();
    this->addChild(menu);
    menu->setTag(tag);
}

void HelloWorld::menuStartCallback(Ref* pSender)
{
//    auto game = Game::createScene();
//    auto gameScene = TransitionTurnOffTiles::create(1.0f, game);
//    Director::getInstance() -> pushScene(gameScene);
    
    auto breaker = Breaker::createScene();
    Director::getInstance() -> pushScene(breaker);
}

void HelloWorld::menuSettingCallback(Ref* pSender)
{
    auto setting = Setting::createScene();
    auto settingScene = TransitionPageTurn::create(1.0f, setting, false);
    Director::getInstance() -> pushScene(settingScene);
}

void HelloWorld::menuQuitCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    exit(0);
}
