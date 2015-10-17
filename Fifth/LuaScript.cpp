//
//  LuaScript.cpp
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "LuaScript.h"
#include "CEntity.h"


LuaScript::LuaScript(lua_State* L, std::string path)
    : _L(L)
    , _path(path)
    , _objectCreationFunction(init(L, path))
{
    
}

luabridge::LuaRef LuaScript::init(lua_State* L, std::string path) {
    luaL_dofile(L, path.c_str());
    return luabridge::getGlobal(L, "create");
}

void LuaScript::doFile() {
    luaL_dofile(_L, _path.c_str());
}

luabridge::LuaRef* LuaScript::getObjectCreation() {
    if(_objectCreationFunction.isNil())  
        std::cout << "No creation function for script \"" << _path << "\".\n";
    
    return &_objectCreationFunction;
}

lua_State* LuaScript::getState() {
    return _L;
}

std::string LuaScript::getPath() {
    return _path;
}

std::string LuaScript::getName() {
    std::string dir = _path.substr(_path.find_last_of("/") + 1, _path.size() - _path.find_last_of("/"));
    return dir.substr(0, dir.find("."));
}