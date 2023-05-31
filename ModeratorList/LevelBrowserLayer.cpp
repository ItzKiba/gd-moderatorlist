#include "LevelBrowserLayer.h"
#include "ModLayer.h"

bool __fastcall LevelBrowserLayer::hook(gd::LevelBrowserLayer* self, void* edx, gd::GJSearchObject* obj) {
	bool result = LevelBrowserLayer::init(self, obj);
	
	if (obj->getType() != gd::kGJSearchTypeMagic) {
		return result;
	}
		
	auto size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* btnSpr = CCSprite::createWithSpriteFrameName("GJ_starBtnMod_001.png");
	auto btn = gd::CCMenuItemSpriteExtra::create(btnSpr, self, menu_selector(ModLayer::callback));

	CCMenu* menu = CCMenu::create();
	menu->setPosition(size.width - 26.75f, 100.0f);

	menu->addChild(btn);
	self->addChild(menu);

	return result;
}