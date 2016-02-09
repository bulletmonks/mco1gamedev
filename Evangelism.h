#pragma once
#include "cocos2d.h"

using namespace std;
class Evangelism : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Evangelism);
	void update(float) override;
private:
	cocos2d::SEL_SCHEDULE selSched;
};
