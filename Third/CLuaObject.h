//
//  CLuaObject.h
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __LuaTest4__CLuaObject__
#define __LuaTest4__CLuaObject__

#include <stdio.h>
#include <map>
#include <vector>

#include "CLuaScript.h"


class CEntity;
class CComponent;

class CLuaObject {
    
public:
    CLuaObject(CEntity* parent, CComponent* component, CLuaScript* script);
    
    void beginCall(std::string function);
    void endCall(int argc, int results);
    void endCall();
    
    std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>> getTable(std::string name);
    
    template<typename T>
    void callSetFunction(std::string function, T t) {
        beginCall(function);
        pushObject(t);
        endCall(1, 0);
    }
    
    void selectScript();
    void selectReference(std::string reference);
    bool hasReference(std::string reference);
    
    void pushThisObject();
    
    template<class T>
    void pushObject(T t) {
        luabridge::Stack<T>::push(getScript()->getState(), t);
    }
    
    CLuaScript* getScript();
    
    luabridge::LuaRef operator[] (std::string key) {
        return _object[key.c_str()];
    }
    
    
private:
    CLuaScript* _script;
    CEntity* _parent;
    luabridge::LuaRef _object;
    
    std::map<std::string, bool> hasReferences;
    
};

#endif /* defined(__LuaTest4__CLuaObject__) */
