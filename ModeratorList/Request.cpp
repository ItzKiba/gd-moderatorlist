#include "Request.h"
#include "ModUsers.h"

void Request::createUserRequest() {
	auto request = new cocos2d::extension::CCHttpRequest();
	request->setUrl("https://github.com/ItzKiba/gd-moderatorlist/blob/main/list.txt");
	request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);

	request->setResponseCallback(request, callfuncND_selector(Request::onUserRequestComplete));

	cocos2d::extension::CCHttpClient::getInstance()->send(request);
	request->release();
}

void Request::onUserRequestComplete(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response) {

	if (!response) {
		ModUsers::setRequestString("No response.");
		return;
	}

	if (!response->isSucceed()) {
		ModUsers::setRequestString("Unsuccessful");
		return;
	}

	//success
	std::vector<char>* buffer = response->getResponseData();

	std::string result(buffer->begin(), buffer->end());
	ModUsers::setRequestString(result);
}