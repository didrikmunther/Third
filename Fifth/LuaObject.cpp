//
//
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "LuaObject.h"
#include "CEntity.h"


LuaObject::LuaObject(CEntity* parent, LuaScript* script)
    : _script(script)
    , _parent(parent)
    , _object((*script->getObjectCreation())(parent))
{
    
}

void LuaObject::beginCall(std::string function) {
    selectReference(function);
    lua_pushvalue(_script->getState(), -2);
}

void LuaObject::endCall(int argc, int results) {
    lua_call(_script->getState(), argc + 1, results);
}

void LuaObject::endCall() {
    endCall(0, 0);
}

void LuaObject::selectScript() {
    _object.push(_script->getState());
}

void LuaObject::selectReference(std::string reference) {
    selectScript();
    ((luabridge::LuaRef)_object[reference.c_str()]).push(_script->getState());
}

bool LuaObject::hasReference(std::string reference) {
    return (bool)_object[reference.c_str()];
}

void LuaObject::pushObject() {
    selectScript();
    lua_pushvalue(_script->getState(), -1);
}

LuaScript* LuaObject::getScript() {
    return _script;
}
