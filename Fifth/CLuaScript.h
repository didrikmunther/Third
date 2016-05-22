//
//  CLuaScript.h
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __LuaTest4__CLuaScript__
#define __LuaTest4__CLuaScript__

#include <stdio.h>
#include <string>
#include <iostream>

extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}

#include "LuaBridge/LuaBridge.h"


class Entity;

class CLuaScript {
    
public:
    CLuaScript(lua_State* L, std::string path);
    
    luabridge::LuaRef init(lua_State* L, std::string path);
    void funcError();
    
    void doFile();
    
    luabridge::LuaRef* getObjectCreation();
    
    lua_State* getState();
    std::string getPath();
    std::string getName();
    
    void onCleanup() {  }
    
    bool isInvalid() { return _isInvalid; }
    
private:
    lua_State* _L;
    std::string _path;
    std::string _name;
    luabridge::LuaRef _objectCreationFunction;
    
    bool _isInvalid;
    
};

#endif /* defined(__LuaTest4__CLuaScript__) */
