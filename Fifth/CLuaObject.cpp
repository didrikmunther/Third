//
//
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CLuaObject.h"
#include "CEntity.h"


CLuaObject::CLuaObject(CEntity* parent, CComponent* component, CLuaScript* script)
    : _script(script)
    , _parent(parent)
    , _object((*script->getObjectCreation())(parent, component))
{
    
}

void CLuaObject::beginCall(std::string function) {
    selectReference(function);
    lua_pushvalue(_script->getState(), -2);
}

void CLuaObject::endCall(int argc, int results) {
    lua_call(_script->getState(), argc + 1, results);
}

void CLuaObject::endCall() {
    endCall(0, 0);
}

void CLuaObject::selectScript() {
    _object.push(_script->getState());
}

void CLuaObject::selectReference(std::string reference) {
    selectScript();
    ((luabridge::LuaRef)_object[reference.c_str()]).push(_script->getState());
}

bool CLuaObject::hasReference(std::string reference) {
    return !_object[reference.c_str()].isNil();
}

void CLuaObject::pushObject() {
    selectScript();
    lua_pushvalue(_script->getState(), -1);
}

CLuaScript* CLuaObject::getScript() {
    return _script;
}
