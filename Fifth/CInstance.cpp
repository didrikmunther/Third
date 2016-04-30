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
    , doLoadLevel(false)
    , levelToLoad("")
    , player(nullptr), controller(nullptr)
    , loadedMap("")
{
    
}

CInstance::~CInstance() {
    delete camera;
}

void CInstance::onLoop() {
    if(doLoadLevel) {
        _loadLevel(levelToLoad);
        levelToLoad = "";
    }
}

void CInstance::loadAssets(std::string path) {
    //CAssetManager::onCleanup(CLEAN_FLAGS::NOT_LUA_SCRIPTS);
    NFile::loadAssets(path, this);
}

void CInstance::doLine(std::string line) {
    luaL_dostring(L, line.c_str());
}

void CInstance::loadLevel(std::string fileName) {
    levelToLoad = fileName;
    doLoadLevel = true;
}

void CInstance::_loadLevel(std::string fileName) {
    entityManager.onCleanup();
    player = nullptr;
    controller = nullptr;
    camera->setTarget(nullptr);
    doLoadLevel = false;
    if(NFile::loadLevel(fileName, this) == -1) {
        NFile::log(LogType::ALERT, "Reverting loaded map to: \"", loadedMap, "\".");
        NFile::loadLevel(loadedMap, this);
    } else {
        loadedMap = fileName;
    }
}

std::string CInstance::onSerialize() {
    rapidjson::Document d;
    d.Parse("{}");
    
    rapidjson::Value values(rapidjson::kObjectType);
    entityManager.onSerialize(&values, &d.GetAllocator(), this);
    
    values.AddMember("player", rapidjson::Value(entityManager.getNameOfEntity(player).c_str(), d.GetAllocator()), d.GetAllocator());
    values.AddMember("controller", rapidjson::Value(entityManager.getNameOfEntity(controller).c_str(), d.GetAllocator()), d.GetAllocator());
    
    d.AddMember("this", values, d.GetAllocator());
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    d.Accept(writer);
    
    return sb.GetString();
}

void CInstance::closeInstance() {
    entityManager.onCleanup();
    lua_close(L);
}