//
//  LuaObject.h
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __LuaTest4__LuaObject__
#define __LuaTest4__LuaObject__

#include <stdio.h>
#include <iostream>

#include "LuaScript.h"


class CEntity;
class EComponent;

class LuaObject {
    
public:
    LuaObject(CEntity* parent, EComponent* component, LuaScript* script);
    
    void beginCall(std::string function);
    void endCall(int argc, int results);
    void endCall();
    
    void selectScript();
    void selectReference(std::string reference);
    bool hasReference(std::string reference);
    
    void pushObject();
    
    LuaScript* getScript();
    
    luabridge::LuaRef operator[] (std::string key) {
        return _object[key.c_str()];
    }
    
    
private:
    LuaScript* _script;
    CEntity* _parent;
    luabridge::LuaRef _object;
    
};

#endif /* defined(__LuaTest4__LuaObject__) */
