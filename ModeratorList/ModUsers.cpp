#include "ModUsers.h"

//void ModUsers::addUser(const char* name, int badgeLevel) {
//	switch (badgeLevel) {
//	case 1:
//		ModUsers::ModList.push_back(name);
//		break;
//	case 2:
//		ModUsers::ElderModList.push_back(name);
//		break;
//	}
//}

std::vector<std::string> ModUsers::getModList() {
	return ModUsers::ModList;
}

std::vector<int> ModUsers::getBadgeList() {
	return ModUsers::BadgeList;
}

std::vector<int> ModUsers::getAccountList() {
	return ModUsers::AccountList;
}


void ModUsers::clearLists() {
	ModUsers::ModList.clear();
	ModUsers::BadgeList.clear();
	ModUsers::AccountList.clear();
}

void ModUsers::setRequestString(std::string str) {
	ModUsers::requestString = str;
}

std::string ModUsers::getRequestString() {
	return ModUsers::requestString;
}

void ModUsers::setupLists(std::string str) {

	std::vector<std::string> tokens;

	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, '\n')) {
		tokens.push_back(token);
	}

	for (unsigned int i = 0; i < tokens.size(); i += 3) {
		ModUsers::ModList.push_back(tokens.at(i));
		ModUsers::BadgeList.push_back(std::stoi(tokens.at(i + 1)));
		ModUsers::AccountList.push_back(std::stoi(tokens.at(i + 2)));
	}
	

}


