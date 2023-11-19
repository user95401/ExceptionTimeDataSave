#include "mod_utils.hpp"
#include "hooks.hpp"
#include "SimpleIni.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;


DWORD WINAPI PROCESS_ATTACH(void* hModule) {
    //nothing here yet...
    return 0;
}

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* info) {
    //save game?
    GameManager::sharedState()->save();
    //who is it also ok
    AppDelegate::sharedApplication()->trySaveGame();
    return EXCEPTION_CONTINUE_SEARCH;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, PROCESS_ATTACH, hModule, 0, 0);
        SetUnhandledExceptionFilter(ExceptionHandler);
    }
    return TRUE;
}
