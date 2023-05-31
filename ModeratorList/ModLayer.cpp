#include "ModLayer.h"
#include "ModUsers.h"
#include <vector>
#include <iostream>
#include <WinUser.h>

bool ModLayer::init() {

    auto size = CCDirector::sharedDirector()->getWinSize();

    auto cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBL->setPosition({ 0,0 });
    cornerBL->setAnchorPoint({ 0,0 });
    this->addChild(cornerBL);

    auto cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBR->setPosition({ size.width,0 });
    cornerBR->setAnchorPoint({ 1,0 });
    cornerBR->setFlipX(true);
    this->addChild(cornerBR);

    auto background = CCSprite::create("GJ_gradientBG.png");
    auto backgroundSize = background->getContentSize();
    background->setScaleX(size.width / backgroundSize.width);
    background->setScaleY(size.height / backgroundSize.height);
    background->setAnchorPoint({ 0, 0 });
    background->setColor({ 0, 128, 255 });
    background->setZOrder(-2);
    this->addChild(background);

    CCSprite* backButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backButton = gd::CCMenuItemSpriteExtra::create(backButtonSprite, this, menu_selector(ModLayer::backButtonCallback));
    CCMenu* backButtonMenu = CCMenu::create();
    backButtonMenu->setPosition(24, size.height - 23);
    backButtonMenu->addChild(backButton);
    this->addChild(backButtonMenu);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    CCSprite* infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    //infoButtonSprite->setColor({255, 170, 255});
    auto infoButton = gd::CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(ModLayer::infoButtonCallback));
    CCMenu* infoButtonMenu = CCMenu::create();
    infoButtonMenu->setZOrder(3);
    infoButtonMenu->setPosition(30, 30);
    infoButtonMenu->addChild(infoButton);
    this->addChild(infoButtonMenu);

    CCMenu* menu = CCMenu::create();
    auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    prevBtn = gd::CCMenuItemSpriteExtra::create(
        prevSprite,
        this,
        menu_selector(ModLayer::onPrevPage)
    );
    prevBtn->setPosition({ -(size.width / 2) + 25, 0 });
    menu->addChild(prevBtn);

    auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextSprite->setFlipX(true);
    nextBtn = gd::CCMenuItemSpriteExtra::create(
        nextSprite,
        this,
        menu_selector(ModLayer::onNextPage)
    );
    nextBtn->setPosition({ +(size.width / 2) - 25, 0 });
    menu->addChild(nextBtn);
    this->addChild(menu);

    auto bgSprite = cocos2d::extension::CCScale9Sprite::create("square02_001.png");
    bgSprite->setPosition({ size / 2 });
    bgSprite->setAnchorPoint({0.5, 0.5});
    bgSprite->setOpacity(128);
    bgSprite->setContentSize({size.width * (4.25f/6), 175.f});
    this->addChild(bgSprite);

    // users lol
    ModUsers::clearLists();

    ModLayer::objects = std::vector<CCNode*>{};

    ModLayer::createUserRequest();

    nextBtn->setVisible(false);
    prevBtn->setVisible(false);

    // create the loading circle!! poggers!!!!

    auto loading = gd::LoadingCircle::create();
    loading->setZOrder(5);
    loading->runAction(CCRotateBy::create(20.0f, 360.f * 20.0f));
    this->addChild(loading);
    objects.push_back(loading);

    // logo, of course...
    auto logo = CCLabelBMFont::create("Moderators", "goldFont.fnt");
    logo->setScale(1.5f);
    logo->setPosition({size.width / 2, size.height * (9/10.f)});
    this->addChild(logo);

    return true; 
}

void ModLayer::keyBackClicked() {
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

ModLayer* ModLayer::create() {
	auto pRet = new ModLayer();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void ModLayer::callback(CCObject*) {
	auto scene = CCScene::create();
	auto layer = ModLayer::create(); // creates the layer using the ::create function above. if it fails, DONT CRASH!!!
	scene->addChild(layer);
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));

}

void ModLayer::backButtonCallback(CCObject*) {
    keyBackClicked();
}

void ModLayer::infoButtonCallback(CCObject*) {

    std::string caption = "<co>Moderators</c> are users that can send <cg>high quality levels</c> to <cr>RobTop</c>, the creator of Geometry Dash.\n\nOf these levels, some may be chosen to receive a <cy>star rating</c>.\n\n<cb>Levels sent by Moderators appear in the Magic tab.</c>";

    auto alert = gd::FLAlertLayer::create(nullptr, "Info", "OK", nullptr, caption);
    alert->show();
}

void ModLayer::openProfileCallback(CCObject* obj) {
    int id = obj->getTag();
    auto page = gd::ProfilePage::create(id, false);
    page->show();
}

void ModLayer::loadPage(unsigned int page) {

    for (CCNode* node : ModLayer::objects) {
        node->removeFromParent();
    }
    ModLayer::objects = std::vector<CCNode*>{};

    auto size = CCDirector::sharedDirector()->getWinSize();

    auto modList = ModUsers::getModList();
    auto badgeList = ModUsers::getBadgeList();
    auto accList = ModUsers::getAccountList();

    unsigned int firstIndex = page * 10;
    unsigned int lastIndex = min(((page+1) * 10), badgeList.size());

    unsigned int maxPages = (badgeList.size()-1) / 10;
    
    if (page == 0) {
        prevBtn->setVisible(false);
    }
    else {
        prevBtn->setVisible(true);
    }
    if (page < maxPages) {
        nextBtn->setVisible(true);
    }
    else {
        nextBtn->setVisible(false);
    }

    int numDots = maxPages + 1;

    float leftPosScale = 0.50f - numDots * 0.015f;
    float rightPosScale = 0.50f + numDots * 0.015f;

    float dotStepScale = rightPosScale - leftPosScale;
    if (numDots != 1) {
        dotStepScale /= numDots - 1;
    }
    

    // DOTS lol
    for (unsigned int i = 0; i < numDots; i++) {
        auto dotSprite = CCSprite::createWithSpriteFrameName("uiDot_001.png");
        dotSprite->setScale(0.75f);

        dotSprite->setOpacity(64);
        if (i == page) {
            dotSprite->setOpacity(255);
        }

        dotSprite->setPosition({(leftPosScale * size.width) + (dotStepScale * i * size.width), 10.f});
        ModLayer::objects.push_back(dotSprite);
        this->addChild(dotSprite);
    }

    // badges and shiz
    float step = 30.f;
    float currBadgeX = size.width * (1.0f / 5);
    float currBadgeY = (size.height / 2) + (2 * step);

    for (unsigned int i = firstIndex; i < lastIndex; i++) {
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("modBadge_01_001.png");
        auto label = CCLabelBMFont::create(modList.at(i).c_str(), "bigFont.fnt");
        //label->setAnchorPoint({0, 0.5});
        auto labelBtn = CCMenuItemLabel::create(label, this, menu_selector(ModLayer::openProfileCallback));
        labelBtn->setAnchorPoint({ 0, 0.5 });
        labelBtn->setTag(accList.at(i));
        auto labelMenu = CCMenu::create();
        labelMenu->addChild(labelBtn);
        labelMenu->setScale(0.5f);
        labelMenu->setAnchorPoint({0, 0});
        labelBtn->setColor({255, 255, 185});
        if (badgeList.at(i) == 2) {
            sprite = CCSprite::createWithSpriteFrameName("modBadge_02_001.png");
            label->setColor({ 185, 255, 185 });
        }
        if (modList.at(i) == "RobTop") {
            label->setColor({0, 255, 255});
            label->setColor({ 0, 255, 255 });
        }
        sprite->setPositionX(currBadgeX);
        labelMenu->setPositionX(currBadgeX + 20.f);
        sprite->setPositionY(currBadgeY);
        labelMenu->setPositionY(currBadgeY);

        this->addChild(sprite);
        this->addChild(labelMenu);
        
        ModLayer::objects.push_back(sprite);
        ModLayer::objects.push_back(label);
        ModLayer::objects.push_back(labelBtn);
        ModLayer::objects.push_back(labelMenu);

        currBadgeY -= step;
        if (i == firstIndex + 4) {
            currBadgeX = size.width * (3.5f / 6);
            currBadgeY = (size.height / 2) + (2 * step);
        }
    }
}

void ModLayer::onPrevPage(CCObject* object) {
    loadPage(--page);
}

void ModLayer::onNextPage(CCObject* object) {
    loadPage(++page);
}

CCScene* ModLayer::scene() {
    auto layer = ModLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

int ModLayer::getPage() const {
    return page;
}

void ModLayer::createUserRequest() {
    auto request = new cocos2d::extension::CCHttpRequest();
    request->setUrl("https://raw.githubusercontent.com/ItzKiba/gd-moderatorlist/main/list.txt");
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);

    request->setResponseCallback(this, httpresponse_selector(ModLayer::onUserRequestComplete));

    cocos2d::extension::CCHttpClient::getInstance()->send(request);
    request->release();
}

void ModLayer::onUserRequestComplete(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response) {

    if (!response) {
        ModUsers::setRequestString("No response.");
        ModLayer::loadFailedToConnect();
        return;
    }

    if (!response->isSucceed()) {
        ModUsers::setRequestString("Unsuccessful");
        ModLayer::loadUnsuccessful();
        return;
    }
    // SUCCESS if it reaches here
    std::vector<char>* buffer = response->getResponseData();

    std::string result(buffer->begin(), buffer->end());
    ModUsers::setRequestString(result);
    ModUsers::setupLists(result);
    ModLayer::loadPage(0);
    ModLayer::showModStats();


    /*auto label3 = CCLabelBMFont::create(ModUsers::getRequestString().c_str(), "bigFont.fnt");
    label3->setPosition({ 200, 200 });
    this->addChild(label3);*/
}

void ModLayer::loadFailedToConnect() {
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto label1 = CCLabelBMFont::create("Failed to connect!", "bigFont.fnt");
    label1->setPosition({ size.width / 2, size.height / 2 + 15 });
    label1->setColor({255, 150, 150});
    label1->setScale(0.8f);

    auto label2 = CCLabelBMFont::create("Your internet may not be working.", "bigFont.fnt");
    label2->setPosition({size.width / 2, (size.height / 2) - 15});
    label2->setScale(0.5f);
    label2->setAnchorPoint({ 0.5, 0.5 });
    label2->setOpacity(128);

    this->addChild(label1);
    this->addChild(label2);

    

}

void ModLayer::loadUnsuccessful() {
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto label1 = CCLabelBMFont::create("Connection unsuccessful!", "bigFont.fnt");
    label1->setPosition({size.width / 2, size.height / 2 + 15});
    label1->setColor({ 255, 150, 150 });
    label1->setScale(0.8f);

    auto label2 = CCLabelBMFont::create("A connection was established, but\nno data could be retrieved.", "bigFont.fnt");
    label2->setPosition({ size.width / 2, (size.height / 2) - 15 });
    label2->setAnchorPoint({0.5, 0.5});
    label2->setScale(0.5f);
    label2->setOpacity(128);

    this->addChild(label1);
    this->addChild(label2);
    
}

void ModLayer::showModStats() {
    auto size = CCDirector::sharedDirector()->getWinSize();

    auto bgSprite2 = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
    bgSprite2->setPosition({ size.width / 2, 40.f });
    bgSprite2->setAnchorPoint({ 0.5, 0.5 });
    bgSprite2->setOpacity(128);
    bgSprite2->setZOrder(-2);
    bgSprite2->setContentSize({ size.width * (4.25f / 6), 40.f });
    this->addChild(bgSprite2);

    auto label = CCLabelBMFont::create("Totals", "bigFont.fnt");
    label->setPosition({size.width * 0.2f, 40.f});
    label->setAnchorPoint({0, 0.5});
    label->setScale(0.5f);
    this->addChild(label);

    int numMods = 0;
    int numElderMods = 0;
    for (unsigned int i = 0; i < ModUsers::getBadgeList().size(); i++) {
        if (ModUsers::getBadgeList().at(i) == 1) {
            numMods++;
        }
        else {
            numElderMods++;
        }
    }

    auto modSprite = CCSprite::createWithSpriteFrameName("modBadge_01_001.png");
    modSprite->setPosition({ size.width * 0.475f, 40.f });
    modSprite->setScale(0.8f);
    this->addChild(modSprite);

    auto modLabel = CCLabelBMFont::create(std::to_string(numMods).c_str(), "bigFont.fnt");
    modLabel->setPosition({size.width * 0.475f + 15.f, 40.f});
    modLabel->setColor({ 255, 255, 185 });
    modLabel->setScale(0.5f);
    modLabel->setAnchorPoint({ 0, 0.5 });
    this->addChild(modLabel);

    auto modSpriteElder = CCSprite::createWithSpriteFrameName("modBadge_02_001.png");
    modSpriteElder->setPosition({ size.width * 0.35f, 40.f });
    modSpriteElder->setScale(0.8f);
    this->addChild(modSpriteElder);

    auto modLabelElder = CCLabelBMFont::create(std::to_string(numElderMods).c_str(), "bigFont.fnt");
    modLabelElder->setPosition({ size.width * 0.35f + 15.f, 40.f });
    modLabelElder->setColor({ 185, 255, 185 });
    modLabelElder->setScale(0.5f);
    modLabelElder->setAnchorPoint({0, 0.5});
    this->addChild(modLabelElder);



}