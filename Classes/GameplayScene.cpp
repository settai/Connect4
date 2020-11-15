#include "GameplayScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include <sstream>
#include <algorithm>

USING_NS_CC;

Scene* GameplayScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameplayScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool GameplayScene::init()
{
	// init origin & visible size
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//The gameplay background adapted to screen resolution
	Sprite* background = Sprite::create("res/GameplayBG.png");
	background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);
	rX = visibleSize.width / background->getContentSize().width;
	rY = visibleSize.height / background->getContentSize().height;
	background->setScaleX(rX);
	background->setScaleY(rY);

	// init board, player and the CPU
	board = new Board(this, rX,rY);
	player = new Player();
	CPU = new PlayerAI();


	//Replay Button
	replay = Button::create("res/Replay.png", "res/Replay.png");
	replay->setPosition(Point(visibleSize.width*0.89F + origin.x  , visibleSize.height / 2 + origin.y));
	replay->setScale(rX);
	replay->addTouchEventListener(CC_CALLBACK_2(GameplayScene::replayButtonTouch, this));
	this->addChild(replay);


	//Turn
	string turn = "Turn";
	auto turnLabel = Label::createWithTTF(turn, "fonts/Avenger.ttf", 50);
	turnLabel->setPosition(Point(visibleSize.width*0.09F + origin.x, visibleSize.height*0.83f + origin.y));
	turnLabel->setTextColor(Color4B(207, 141, 1, 255));
	turnLabel->enableShadow(Color4B(79, 31, 25, 255), Size(3, -2), 3);
	this->addChild(turnLabel);

	//Turn Box
	Sprite* turnBox = Sprite::create("res/Box.png");
	turnBox->setPosition(turnLabel->getPosition().x + visibleSize.height*0.003f, turnLabel->getPosition().y - visibleSize.height*0.08f);
	turnBox->setScale(rY*0.8,rY);
	this->addChild(turnBox);


	//Turn Number
	string turnNumberString; ostringstream temp; temp << board->turn; turnNumberString = temp.str();
	turnNumber = Label::createWithTTF(turnNumberString, "fonts/coolvetica.ttf", 40);
	turnNumber->setPosition(turnBox->getPosition().x, turnBox->getPosition().y + visibleSize.height*0.003f);
	turnNumber->setTextColor(Color4B::WHITE);
	this->addChild(turnNumber);

	// Node
	string node = "Node";
	auto nodeLabel = Label::createWithTTF(node, "fonts/Avenger.ttf", 50);
	nodeLabel->setPosition(Point(turnLabel->getPosition().x, turnLabel->getPosition().y - visibleSize.height*0.23f));
	nodeLabel->setTextColor(Color4B(207, 141, 1, 255));
	nodeLabel->enableShadow(Color4B(79, 31, 25, 255), Size(3, -2), 3);
	this->addChild(nodeLabel);

	//Node Box
	Sprite* nodeBox = Sprite::create("res/Box.png");
	nodeBox->setPosition(nodeLabel->getPosition().x + visibleSize.height*0.003f, nodeLabel->getPosition().y - visibleSize.height*0.08f);
    nodeBox->setScale(rY*0.8,rY);
	this->addChild(nodeBox);

	//Node Number
	string nodeNumberString; ostringstream temp2; temp2 << CPU->nbreNoeuds; nodeNumberString = temp2.str();
	nodeNumber = Label::createWithTTF(nodeNumberString, "fonts/coolvetica.ttf", 40);
	nodeNumber->setPosition(nodeBox->getPosition().x, nodeBox->getPosition().y + visibleSize.height*0.003f);
	nodeNumber->setTextColor(Color4B::WHITE);
	this->addChild(nodeNumber);

	//Return 
	string back = "Back";
	auto backLabel = Label::createWithTTF(back, "fonts/Avenger.ttf", 50);
	backLabel->setPosition(Point(nodeLabel->getPosition().x, nodeLabel->getPosition().y - visibleSize.height*0.37f)); //23
	backLabel->setTextColor(Color4B(207, 141, 1, 255));
	backLabel->enableShadow(Color4B(79, 31, 25, 255), Size(3, -2), 3);
	this->addChild(backLabel);

	//Token (Red or Blue)
	token = Sprite::create("res/Blue.png");
	token->setPosition(Point(nodeBox->getPosition().x + visibleSize.height*0.003f, nodeBox->getPosition().y - visibleSize.height*0.14f));
	token->setScale(rY, rY);
	this->addChild(token);

	//Return Button
	returnButton = Button::create("res/Return.png", "res/Return.png");
	returnButton->setPosition(Point(backLabel->getPosition().x + visibleSize.height*0.003f, backLabel->getPosition().y - visibleSize.height*0.105f));
	returnButton->setScale(rY, rY);
	returnButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::returnButtonTouch, this));
	this->addChild(returnButton);


	// Touch Events Handling
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameplayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameplayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameplayScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameplayScene::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameplayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

	Point touchPoint = touch->getLocation();
	for (int C = 0; C < 7; C++)
		if (board->Columns[C].containsPoint(touchPoint))
			{				
				int R = board->emptyPlace(C);
				if (R != -1)
				{	
					
					//Player1 always get to play his turn first no matter if it's against human or CPU
					if (board->turn%2==1) // turn is equal to 1 initially 
					{
						if (CPUvsPlayer) listener->setEnabled(false); 	//Disable listener
					
						player->R = R; player->C = C;	//Get player1 Move

						//Init the blue piece
						Sprite* Blue = Sprite::create("res/Blue.png");
						Blue->setPosition(board->startPoints[C]);
						Blue->setScale(rX, rY);
						this->addChild(Blue);



						//Update board with player1's move
						board->T[R][C] = 1;
						board->turn++;
						updateNumbers();

						//Perform the bounce animation
						ActionInterval* MoveTo = MoveTo::create(1.3f, board->Cercles[R][C]);
						EaseBounceOut* Bounce = EaseBounceOut::create(MoveTo);
						Blue->runAction(Bounce);
				
						scheduleOnce(schedule_selector(GameplayScene::player1WinCheck), 1.3f); //Win detection for player1

					}


					else if (!CPUvsPlayer && board->turn % 2 == 0)
					{
						CPU->R = R; CPU->C = C;

						//Init the red piece
						Sprite* Red = Sprite::create("res/Red.png");
						Red->setPosition(board->startPoints[C]);
						Red->setScale(rX, rY);
						this->addChild(Red);

						//Update board with player2's move
						board->T[R][C] = 2;
						board->turn++;
						updateNumbers();

						//Perform the bounce animation
						ActionInterval* MoveTo = MoveTo::create(1.3f, board->Cercles[R][C]);
						EaseBounceOut* Bounce = EaseBounceOut::create(MoveTo);
						Red->runAction(Bounce);

						scheduleOnce(schedule_selector(GameplayScene::player2WinCheck), 1.3f); //Win detection for player2
					}


					if (CPUvsPlayer)
						scheduleOnce(schedule_selector(GameplayScene::computerPlay), 1.3f);

				}

			} 

	return;

}
bool GameplayScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	return true;
}
void GameplayScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {     }
void GameplayScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {     }

void GameplayScene::computerPlay(float dt)
{
	// init difficulty against CPU
	if (easy) CPU->finalDepth = 2;
	else CPU->finalDepth = 6;

	//Get & update board with AI's Move
	int state[6][7];
	board->copyBoard(state);

	int C = CPU->alpha_beta(state, 0, true, 0, 0, -100000000, +100000000);
	//int C = CPU->minimax(state,0,true,0,0);
	int R = board->emptyPlace(C);
	board->T[R][C] = 2; 
	board->turn++;

	//Get CPU's Move
	CPU->R = R;
	CPU->C = C;

	//Init the blue piece
	Sprite* Red = Sprite::create("res/Red.png");
	Vec2 start = board->startPoints[CPU->C];
	Red->setPosition(start);
	Red->setScale(rX, rY);
	this->addChild(Red);

	//Perform the bounce animation
	Vec2 destination = board->Cercles[CPU->R][CPU->C];
	ActionInterval* MoveTo = MoveTo::create(1.2f, destination);
	EaseBounceOut* Bounce = EaseBounceOut::create(MoveTo);
	Red->runAction(Bounce);


	//Win detection for player2
	scheduleOnce(schedule_selector(GameplayScene::player2WinCheck), 0.6f);


}

void GameplayScene::player1WinCheck(float dt)
{
	token->setTexture("res/Red.png");

	if (board->isWin(player->R, player->C, 1))
	{
		Layer* winCelebration = Layer::create();
		Sprite* frames[4];
		Sprite* balls[4];
		board->fill4inRowList(player->R, player->C, 1);
		for (int i = 0; i < 4; i++)
		{
			balls[i] = Sprite::create("res/Blue.png");
			balls[i]->setPosition(board->Cercles[board->fourinRow[i][0]][board->fourinRow[i][1]]);
            balls[i]->setScale(rY, rY);
			winCelebration->addChild(balls[i], 300);
			frames[i] = Sprite::create("res/frame.png");
			frames[i]->setPosition(board->Cercles[board->fourinRow[i][0]][board->fourinRow[i][1]]);
            frames[i]->setScale(rY, rY);
			winCelebration->addChild(frames[i], 200);

		}

		//Win Animation
		auto scale = ScaleBy::create(0.5f, 1.02f);
		auto scaleBack = ScaleTo::create(0.5f, 1.f);
		auto delay = DelayTime::create(0.1f);
		auto action = Sequence::create(scale, delay, scaleBack, delay->clone(), nullptr);
		winCelebration->runAction(RepeatForever::create(action));
		this->addChild(winCelebration, 400);

		//Disable screen touch when a win is detected
		listener->setEnabled(false);

	}


	updateNumbers();
	CPU->nbreNoeuds = 0;

}

void GameplayScene::player2WinCheck(float dt)
{
	token->setTexture("res/Blue.png");

	//Win Check for human player
	if (board->isWin(CPU->R, CPU->C, 2))
	{
		Layer* winCelebration = Layer::create();
		Sprite* frames[4];
		Sprite* balls[4];
		board->fill4inRowList(CPU->R, CPU->C, 2);
		for (int i = 0; i < 4; i++)
		{
			balls[i] = Sprite::create("res/Red.png");
			balls[i]->setPosition(board->Cercles[board->fourinRow[i][0]][board->fourinRow[i][1]]);
            balls[i]->setScale(rY, rY);
			winCelebration->addChild(balls[i], 300);
			frames[i] = Sprite::create("res/frame.png");
			frames[i]->setPosition(board->Cercles[board->fourinRow[i][0]][board->fourinRow[i][1]]);
            frames[i]->setScale(rY, rY);
			winCelebration->addChild(frames[i],200);
		}

		//Win Animation
		auto scale = ScaleBy::create(0.5f, 1.02f);
		auto scaleBack = ScaleTo::create(0.5f, 1.f);
		auto delay = DelayTime::create(0.1f);
		auto action = Sequence::create(scale, delay, scaleBack, delay->clone(), nullptr);
		winCelebration->runAction(RepeatForever::create(action));
		this->addChild(winCelebration, 400);

		//Disable screen touch when a win is detected
		listener->setEnabled(false);

	}

	else
	{
		// Enable the listener for the next turn
		listener->setEnabled(true);
	}


	updateNumbers();
	CPU->nbreNoeuds = 0;

}

void GameplayScene::replayButtonTouch(Ref *sender, Widget::TouchEventType type)
{
	//Classic Button Animation
	auto getBigger = ScaleTo::create(0.2F, rX*0.8);
	auto getSmaller = ScaleTo::create(0.3F, rX);
	auto ButtonAnimation = Sequence::create(getBigger, getSmaller, nullptr);

	switch (type)
		{
		case Widget::TouchEventType::BEGAN:;
			replay->runAction(ButtonAnimation);
			break;

		case Widget::TouchEventType::ENDED:
			GameplayScene * newScene = GameplayScene::create();
			newScene->easy = easy;
			newScene->CPUvsPlayer = CPUvsPlayer;
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene, Color3B(0, 0, 0)));;
			break;

		}
}

void GameplayScene::updateNumbers()
{
	string turnNumberString; ostringstream temp; temp << board->turn; turnNumberString = temp.str();
	turnNumber->setString(turnNumberString);

	string nodeNumberString; ostringstream temp2; temp2 << CPU->nbreNoeuds; nodeNumberString = temp2.str();
	nodeNumber->setString(nodeNumberString);

}

void GameplayScene::returnButtonTouch(Ref *sender, Widget::TouchEventType type)
{

	//Classic Button Animation
	auto getBigger = ScaleTo::create(0.2F, rX*0.8);
	auto getSmaller = ScaleTo::create(0.3F, rX);
	auto ButtonAnimation = Sequence::create(getBigger, getSmaller, nullptr);

	switch (type)
	{
	case Widget::TouchEventType::BEGAN:;
		returnButton->runAction(ButtonAnimation);
		break;

	case Widget::TouchEventType::ENDED:
		Scene * scene = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, scene));
		break;

	}

}

void GameplayScene::isCPUvsPlayer(bool gameType)
{
	CPUvsPlayer = gameType;
}

void GameplayScene::isGameEasy(bool isEasy)
{
	easy = isEasy;
}