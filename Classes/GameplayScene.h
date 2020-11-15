#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "Board.h"
#include "Player.h"




USING_NS_CC;
using namespace std;
using namespace ui;

class GameplayScene : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameplayScene);


private:
	// Touch event listeners
	EventListenerTouchOneByOne * listener;
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

	void isCPUvsPlayer(bool gameType);
	void isGameEasy(bool isEasy);

	void computerPlay(float dt);
	void player1WinCheck(float dt);
	void player2WinCheck(float dt);

	Sprite* token;
	Label* turnNumber;
	Label* nodeNumber;
	void updateNumbers();

	Button* replay;
	void replayButtonTouch(Ref *sender, Widget::TouchEventType type);
	
	Button* returnButton;
	void returnButtonTouch(Ref *sender, Widget::TouchEventType type);

	Board* board;
	Player* player;
	PlayerAI* CPU;

	bool CPUvsPlayer;
	bool easy;




	// Screen Size and it's origen after scaling
	Size visibleSize;
	Point origin;
	double rX, rY;

	friend class MenuScene;

};