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
    , _mapToLoad("")
{
    
}

void CInstance::onLoop() {
    if(_mapToLoad != "")
        _loadMap(_mapToLoad);
}

CInstance::~CInstance() {
    delete camera;
}

void CInstance::loadMap(std::string path) {
    _mapToLoad = path;
}

void CInstance::_loadMap(std::string path) {
    CAssetManager::onCleanup(CLEAN_FLAGS::NOT_LUA_SCRIPTS);
    NFile::loadMap(path, this);
    _mapToLoad = "";
}

void CInstance::closeInstance() {
    entityManager.onCleanup();
    lua_close(L);
}