#pragma once
#include "pch.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include <vector>

class Request {

public:
	void createUserRequest();
	void onUserRequestComplete(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response);

	Request() {
		
	}
};
