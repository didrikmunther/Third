//
//  CLuaScript.cpp
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CLuaScript.h"
#include "CEntity.h"
#include "NFile.h"


CLuaScript::CLuaScript(lua_State* L, std::string path)
    : _L(L)
    , _path(path)
    , _objectCreationFunction(init(L, path))
{
    
}

luabridge::LuaRef CLuaScript::init(lua_State* L, std::string path) {
    luaL_dofile(L, path.c_str());
    return luabridge::getGlobal(L, "create");
}

void CLuaScript::doFile() {
    luaL_dofile(_L, _path.c_str());
}

luabridge::LuaRef* CLuaScript::getObjectCreation() {
    if(_objectCreationFunction.isNil())  
        NFile::log(LogType::ERROR, "No creation function for script \"", _path, "\"");
    
    return &_objectCreationFunction;
}

lua_State* CLuaScript::getState() {
    return _L;
}

std::string CLuaScript::getPath() {
    return _path;
}

std::string CLuaScript::getName() {
    //std::string dir = _path.substr(_path.find("/") + 1, _path.size() - _path.find("/"));
    std::string dir = _path.substr(_path.find("scripts/") + 1, _path.size() - _path.find("scripts/"));
    dir = dir.substr(dir.find("/") + 1, dir.size() - dir.find("/"));
    return dir.substr(0, dir.find("."));;
}