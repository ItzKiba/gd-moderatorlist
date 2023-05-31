#pragma once
#include "pch.h"
#include <vector>
#include "network/HttpClient.h"
#include "network/HttpRequest.h"

// create a new layer that extends CCLayer
class ModLayer : public CCLayer {

	unsigned int page = 0;
	gd::GJListLayer* listLayer = nullptr;
	gd::CCMenuItemSpriteExtra* prevBtn = nullptr;
	gd::CCMenuItemSpriteExtra* nextBtn = nullptr;
	cocos2d::CCLabelBMFont* counter = nullptr;
	std::vector<CCNode*> objects = {};

protected:
	virtual bool init(); // init function for the ModLayer (all layers have an init function)
	virtual void keyBackClicked(); // for pressing the ESC key to go back
	void backButtonCallback(CCObject*);
	void infoButtonCallback(CCObject*);

	void onPrevPage(CCObject*);
	void onNextPage(CCObject*);

	void openProfileCallback(CCObject*);
	void showModStats();


public:
	void loadPage(unsigned int page);
	void loadFailedToConnect();
	void loadUnsuccessful();
	static ModLayer* create(); // similarly, layers have a create function as well.
	void callback(CCObject*); // functionality for clicking the mod button to enter the layer.
	int getPage() const;
	static cocos2d::CCScene* scene();

	void createUserRequest();
	void onUserRequestComplete(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response);
};