#include "Evangelism.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

//variable creation
using namespace std;
int score;
int x, y;
int humanX, humanY;
int width = 900;
int height = 900;
Label* label;
std::vector<Sprite*> members;
enum eDir { STOP, UP, DOWN, LEFT, RIGHT };
eDir direction;
Sprite* priestSprite, *humanSprite;

Scene* Evangelism::createScene()
{
	auto scene = Scene::create();
	auto layer = Evangelism::create();
	scene->addChild(layer);
	return scene;
}

bool Evangelism::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// initialize game elements
	direction = STOP;
	score = 0;

	//sprites creation
	auto backgroundSprite = Sprite::create("background.png");
	priestSprite = Sprite::create("leftpriest.png");
	humanSprite = Sprite::create("human.png");
	
	selSched = schedule_selector(Evangelism::update);
	members.push_back(priestSprite);
	members.front()->setPosition(45, 45);

	//background music
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("bgsong.wma");
	audio->playBackgroundMusic("bgsong.wma");

	// random human spawn
	humanX = 45 * (1 + random() % 18);
	humanY = 45 * (1 + random() % 18);
	humanSprite->setPosition(humanX, humanY);

	//add sprites to this layer
	this->addChild(backgroundSprite, 0);
	this->addChild(members.front(), 1);
	this->addChild(humanSprite, 2);

	//score
	label = Label::createWithSystemFont(std::to_string(score), "Arial", 48);
	label->setAnchorPoint(cocos2d::Vec2(-16, -15));
	this->addChild(label, 3);

	//keyboard controls
	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		Vec2 loc = event->getCurrentTarget()->getPosition();

		switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			case EventKeyboard::KeyCode::KEY_W:
				if (direction != DOWN)
					direction = UP;break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			case EventKeyboard::KeyCode::KEY_S:
				if (direction != UP)
					direction = DOWN;break;
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
				if (direction != RIGHT)
					direction = LEFT;break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
				if (direction != LEFT)
					direction = RIGHT;break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, priestSprite);
	this->schedule(selSched, .35);
	return true;
}

void Evangelism::update(float delta) {
	x = members.front()->getPositionX();
	y = members.front()->getPositionY();

	//loop for followers
	for (int i = members.size() - 1; i > 0; i--) {
		members[i]->setPosition(members[i - 1]->getPositionX(), members[i - 1]->getPositionY());
		members[i]->setTexture(members[i - 1]->getTexture());	
	}
	switch (direction) {
	case LEFT:
		members.front()->setPosition(x - 45, y);
		members.front()->setTexture(CCTextureCache::sharedTextureCache()->addImage("leftpriest.png"));
		break;
	case RIGHT:
		members.front()->setPosition(x + 45, y);
		members.front()->setTexture(CCTextureCache::sharedTextureCache()->addImage("rightpriest.png"));
		break;
	case UP:
		members.front()->setPosition(x, y + 45);
		members.front()->setTexture(CCTextureCache::sharedTextureCache()->addImage("uppriest.png"));
		break;
	case DOWN:
		members.front()->setPosition(x, y - 45);
		members.front()->setTexture(CCTextureCache::sharedTextureCache()->addImage("downpriest.png"));
		break;
	}

	//loop for collision with followers
	for (int i = 1; i < members.size(); i++) {
		if (members[i]->getPosition() == members.front()->getPosition()) {
			Director::getInstance()->end();
		}
	}
	if ((members.front()->getPositionX() >= humanSprite->getPositionX() - 22 && 
		 members.front()->getPositionX() <= humanSprite->getPositionX() + 22)&& 
		(members.front()->getPositionY() >= humanSprite->getPositionY() - 22 &&
		members.front()->getPositionY() <= humanSprite->getPositionY() + 22)) {

		humanX = (1 + random() % 18) * 45;
		humanY = (1 + random() % 18) * 45;
		humanSprite->setPosition(humanX, humanY);
		score++;
		label->setString(std::to_string(score));
		int tempX = members.back()->getPositionX();
		int tempY = members.back()->getPositionY();
		Sprite* tempSprite = Sprite::create();
		switch (direction) {
		case LEFT:
			tempSprite = Sprite::create("leftpriest.png");
			tempSprite->setPosition(tempX + 45, tempY);

			break;
		case RIGHT:
			tempSprite = Sprite::create("rightpriest.png");
			tempSprite->setPosition(tempX - 45, tempY);

			break;
		case UP:
			tempSprite = Sprite::create("uppriest.png");
			tempSprite->setPosition(tempX, tempY + 45);

			break;
		case DOWN:
			tempSprite = Sprite::create("downpriest.png");
			tempSprite->setPosition(tempX, tempY - 45);

			break;
		}

		this->addChild(tempSprite);
		members.push_back(tempSprite);
		
	}

	//Outside of screen
	if (members.front()->getPositionX() >= width || members.front()->getPositionX() <= 0 || members.front()->getPositionY() >= height || members.front()->getPositionY() <= 0) {
		Director::getInstance()->end();
	}
}