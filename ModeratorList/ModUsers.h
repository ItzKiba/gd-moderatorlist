#pragma once
#include "pch.h"
#include <vector>
#include <regex>

class ModUsers {

private:
	inline static std::string requestString;
	inline static std::vector<std::string> ModList;
	inline static std::vector<int> BadgeList;
	inline static std::vector<int> AccountList;
public:
	static std::vector<std::string> getModList();
	static std::vector<int> getBadgeList();
	static std::vector<int> getAccountList();
	static void clearLists();

	static void setupLists(std::string str);
	static void setRequestString(std::string str);
	static std::string getRequestString();
};