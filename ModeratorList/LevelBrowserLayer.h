#pragma once
#include "pch.h"

namespace LevelBrowserLayer {
	inline bool(__thiscall* init)(gd::LevelBrowserLayer* self, gd::GJSearchObject* obj);
	bool __fastcall hook(gd::LevelBrowserLayer* self, void* edx, gd::GJSearchObject* obj);
}