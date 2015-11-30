//
//  CInstance.cpp
//  Fifth
//
//  Created by Didrik Munther on 06/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CInstance.h"
#include "NFile.h"
#include "CAssetManager.h"


CInstance::CInstance(CGame* game)
    : game(game)
    , L(luaL_newstate())
    , camera(new CCamera())
    , gravity(3.0f)
{
    
}

CInstance::~CInstance() {
    delete camera;
}

void CInstance::loadAssets(std::string path) {
    //CAssetManager::onCleanup(CLEAN_FLAGS::NOT_LUA_SCRIPTS);
    NFile::loadAssets(path, this);

}

void CInstance::doLine(std::string line) {
    luaL_dostring(L, line.c_str());
}

void CInstance::closeInstance() {
    entityManager.onCleanup();
    lua_close(L);
}