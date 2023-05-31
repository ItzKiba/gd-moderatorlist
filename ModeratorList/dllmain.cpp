// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "LevelBrowserLayer.h"

DWORD WINAPI thread(void* hModule)
{
    if (MH_Initialize() != MH_OK)
    {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE> (hModule), 0);
    }

    // enable hooks
    // button - 0x15a040
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x15a040), LevelBrowserLayer::hook, reinterpret_cast<void**>(&LevelBrowserLayer::init));
    MH_EnableHook(MH_ALL_HOOKS);

    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:  
        CreateThread(0, 0x1000, thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

