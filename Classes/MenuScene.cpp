#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "GameplayScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{

	// init origin & visible size
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//The gameplay background adapted to screen resolution
	Sprite* background = Sprite::create("res/MainMenu.png");
	background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);
	rX = visibleSize.width / background->getContentSize().width;
	rY = visibleSize.height / background->getContentSize().height;
	background->setScaleX(rX);
	background->setScaleY(rY);

	//Menu 1 configuration
	menu1 = Layer::create();

	auto PlayervPlayerButton = Button::create("res/playerVSplayer.png", "res/playerVSplayer.png");
	PlayervPlayerButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height*0.58f + origin.y));
	menu1->addChild(PlayervPlayerButton);

	PlayervPlayerButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			GameplayScene* newScene  = GameplayScene::create();
			newScene->isCPUvsPlayer(false);
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene));
		}
	});

	auto CPUvPlayerButton = Button::create("res/cpuVSplayer.png", "res/cpuVSplayer.png");
	CPUvPlayerButton->setPosition(Point(visibleSize.width / 2 + origin.x, PlayervPlayerButton->getPosition().y - PlayervPlayerButton->getBoundingBox().size.height*1.2f));
	//CPUvPlayerButton->setPosition(Point(visibleSize.width / 2 + origin.x, PlayervPlayerButton->getPosition().y - visibleSize.height*0.27));
	menu1->addChild(CPUvPlayerButton);

	CPUvPlayerButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{

		case ui::Widget::TouchEventType::ENDED:
			ShowMenu(menu2, menu1);
		}
	});

	menu1->setScale(rY);
	this->addChild(menu1);


	//Menu 2 configuration
	menu2 = Layer::create();

	auto EasyButton = Button::create("res/Easy.png", "res/Easy.png");
	EasyButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height*0.58f + origin.y));
	menu2->addChild(EasyButton);

	EasyButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			GameplayScene* newScene = GameplayScene::create();
			newScene->isCPUvsPlayer(true);
			newScene->isGameEasy(true);
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene));
		}
	});

	auto HardButton = Button::create("res/Hard.png", "res/Hard.png");
	HardButton->setPosition(Point(visibleSize.width / 2 + origin.x, PlayervPlayerButton->getPosition().y - PlayervPlayerButton->getBoundingBox().size.height*1.2f));
	//HardButton->setPosition(Point(visibleSize.width / 2 + origin.x, PlayervPlayerButton->getPosition().y - visibleSize.height*0.27));
	menu2->addChild(HardButton);

	HardButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			GameplayScene* newScene = GameplayScene::create();
			newScene->isCPUvsPlayer(true);
			newScene->isGameEasy(false);
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene));
			break;
		}
	});

	auto BackButton = Button::create("res/Back.png", "res/Back.png");
	BackButton->setPosition(Point(visibleSize.width*0.19f + origin.x, visibleSize.height*0.24f + origin.y));
	menu2->addChild(BackButton);

	BackButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{

		case ui::Widget::TouchEventType::ENDED:
			ShowMenu(menu1, menu2);
		}
	});

	menu2->setScale(rY);
	this->addChild(menu2);
	menu2->setVisible(false);

    
    return true;
}



void MenuScene::ShowMenu(Layer* menutoShow, Layer* menutoHide) {

	//Pop up animation (Showing off)
	auto nullify = ScaleTo::create(0, 0);
	auto showup = ScaleTo::create(0.4F, 1*rY);
	EaseBackOut *easeAction = EaseBackOut::create(showup);
	auto BoxAnimation = Sequence::create(nullify, easeAction, nullptr);

	//Show the menutoShow and hide menutoHide
	menutoShow->setVisible(true); 
	menutoHide->setVisible(false); 
	menutoShow->runAction(BoxAnimation); 

}
