#include "SettingScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* Setting::createScene()
{
    return Setting::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Setting::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();
    
    createBackground(1);
    createMenu(2);
    
    return true;
}

void Setting::createBackground(int tag)
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

void Setting::createMenu(int tag)
{
    
    MenuItemFont::setFontName("Times New Roman");
    MenuItemFont::setFontSize(30);
    
    auto soundOnMenuItem = MenuItemImage::create("on.png", "on.png");
    auto soundOffMenuItem = MenuItemImage::create("off.png", "off.png");
    auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuSoundToggleCallback, this), soundOnMenuItem, soundOffMenuItem, NULL);
    soundToggleMenuItem -> setPosition(Director::getInstance() -> convertToGL(Vec2(818, 220)));
    
//    auto OnMenuItem = MenuItemImage::create("on.png", "on.png");
//    auto soundOffMenuItem = MenuItemImage::create("off.png", "off.png");
//    auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuSoundToggleCallback, this), soundOnMenuItem, soundOffMenuItem, NULL);
//    soundToggleMenuItem -> setPosition(Director::getInstance() -> convertToGL(Vec2(818, 220)));
    
    
    
    MenuItemFont *okMenuItem = MenuItemFont::create("Ok", CC_CALLBACK_1(Setting::menuOkCallback, this));
//    MenuItemFont *setting = MenuItemFont::create("Setting", CC_CALLBACK_1(HelloWorld::menuSettingCallback, this));
//    MenuItemFont *quit = MenuItemFont::create("Quit", CC_CALLBACK_1(HelloWorld::menuQuitCallback, this));
    
    Menu *menu = Menu::create(soundToggleMenuItem, okMenuItem, NULL);
    menu->alignItemsVertically();
    this->addChild(menu);
    menu->setTag(tag);
}

void Setting::menuOkCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->popScene();
}

void Setting::menuSoundToggleCallback(cocos2d::Ref *pSender)
{
    
}
