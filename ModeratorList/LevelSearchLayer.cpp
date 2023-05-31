#include "LevelSearchLayer.h"

bool __fastcall LevelSearchLayer::hook(CCLayer* self)
{
	bool result = LevelSearchLayer::init(self);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();

	auto modButtonSprite = CCSprite::create("ML_modListButton.png");
	modButtonSprite->setScale(0.8f);
	auto modButton = gd::CCMenuItemSpriteExtra::create(modButtonSprite, self, nullptr);

	CCMenu* menu = CCMenu::create();
	menu->setPosition(size.width - 25, size.height - 125);
	
	menu->addChild(modButton);
	self->addChild(menu);
	return result;
}