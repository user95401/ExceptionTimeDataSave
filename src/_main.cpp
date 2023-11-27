#include "mod_utils.hpp"
#include "hooks.hpp"
#include "SimpleIni.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

LONG WINAPI VectoredExceptionHandler(_EXCEPTION_POINTERS* pExceptInfo) {
    //save game?
    GameManager::sharedState()->save();
    ModUtils::log("game saved");//save log why no
    //level editor tries
    LevelEditorLayer* LevelEditorLayer_ = GameManager::sharedState()->getEditorLayer();
    if (LevelEditorLayer_) {
        LevelEditorLayer_->getEditorUI()->undoLastAction(CCNode::create());
        LevelEditorLayer_->getEditorUI()->undoLastAction(CCNode::create());
        LevelEditorLayer_->getEditorUI()->undoLastAction(CCNode::create());
        ModUtils::log("was performed 3 undo actions in current level editor layer");
        EditorPauseLayer::create(LevelEditorLayer_)->saveLevel();
        ModUtils::log("current level saved");
    }
    //error msg
    DWORD exc_code = pExceptInfo->ExceptionRecord->ExceptionCode;
    bool dontCare = false;
    //msg text
    std::stringstream buffer;
    buffer << "Received fatal exception: ";
    switch (exc_code) {
    case DBG_PRINTEXCEPTION_WIDE_C:
        buffer << "DBG_PRINTEXCEPTION_WIDE_C";
        dontCare = true;
        break;
    case DBG_PRINTEXCEPTION_C:
        buffer << "DBG_PRINTEXCEPTION_C";
        dontCare = true;
        break;
    default:
        buffer << "0x" << std::hex << exc_code;
        break;
    }
    ModUtils::log(buffer.str());
    //msg text makeup end
    if (dontCare) return EXCEPTION_CONTINUE_EXECUTION;//came from 2.2 pc gdps... idk if needs here
    return EXCEPTION_CONTINUE_SEARCH;
}

DWORD WINAPI PROCESS_ATTACH(void* hModule) {
    //nothing here yet...
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, PROCESS_ATTACH, hModule, 0, 0);
        AddVectoredExceptionHandler(1, VectoredExceptionHandler);
    }
    return TRUE;
}
