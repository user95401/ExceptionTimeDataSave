#include <Geode/Geode.hpp>
using namespace geode::prelude;

void SaveMethods() {
    //save game?
    AppDelegate::sharedApplication()->trySaveGame();
    //who is it also ok
    GameManager::sharedState()->save();
	//save log why no
    log::info("Game saved");
    //level editor tries
    try {
        LevelEditorLayer* LevelEditorLayer_ = GameManager::sharedState()->getEditorLayer();
        LevelEditorLayer_->m_editorUI->undoLastAction(CCNode::create());
        LevelEditorLayer_->m_editorUI->undoLastAction(CCNode::create());
        LevelEditorLayer_->m_editorUI->undoLastAction(CCNode::create());
        log::info("Was performed 3 undo actions in current level editor layer");
        EditorPauseLayer::create(LevelEditorLayer_)->saveLevel();
        log::info("Current level saved");
    }
    catch(const std::exception& e) {
        log::warn(std::string("level saving failed... ") + e.what());
    }
}

BOOL WINAPI ConsoleHandler(DWORD CEvent) {
    //save game at some events
    switch(CEvent) {
    case CTRL_CLOSE_EVENT:
        SaveMethods();
        break;
    case CTRL_SHUTDOWN_EVENT:
        SaveMethods();
        break;
    }
    return TRUE;
}

LONG WINAPI VectoredExceptionHandler(_EXCEPTION_POINTERS* pExceptInfo) {
    //save game
    SaveMethods();
    //error msg
    DWORD exc_code = pExceptInfo->ExceptionRecord->ExceptionCode;
    bool dontCare = false;
    //msg text
    std::stringstream buffer;
    buffer << "Was received fatal exception: ";
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
    log::warn(buffer.str());
    //msg text makeup end
    if (dontCare) return EXCEPTION_CONTINUE_EXECUTION;//came from 2.2 pc gdps... idk if needs here
    return EXCEPTION_CONTINUE_SEARCH;
}

$on_mod(Loaded) {
    //VectoredExceptionHandler
	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
    //ConsoleHandler for idk
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE) {//c cast???!!((
        log::error("Can't install console handler!\n");
    }
}