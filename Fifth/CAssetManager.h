//
//  CAssetManager.h
//  Third
//
//  Created by Didrik Munther on 17/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CAssetManager__
#define __Third__CAssetManager__

#include <stdio.h>
#include <map>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>

#include "CSprite.h"


class CSpriteContainer;
class CLuaScript;
class lua_State;

enum CLEAN_FLAGS {
    EVERYTHING      = 0,
    NOT_LUA_SCRIPTS = 1 << 0
};

class CAssetManager {
    
public:
    CAssetManager();
    
    static CSprite* addSprite(std::string name, std::string spriteSheetKey, Box source);
    static std::string addSprite(CSprite* sprite, std::string name = "");
    static CSpriteContainer* addSpriteContainer(std::string name, std::string spriteKey, Area area = {-1, -1});
    static std::string addSpriteContainer(CSpriteContainer* spriteContainer, std::string name = "");
    static CSpriteSheet* addSpriteSheet(std::string name, SDL_Renderer* renderer, std::string fileName);
    static TTF_Font* addFont(std::string name, std::string fileName, int size);
    static CLuaScript* addLuaScript(lua_State* L, std::string path);
    
    static CSprite* getSprite(std::string key);
    static CSpriteContainer* getSpriteContainer(std::string key);
    static CSpriteSheet* getSpriteSheet(std::string key);
    static TTF_Font* getFont(std::string key);
    static CLuaScript* getLuaScript(std::string key);
    
    static void removeSpriteContainer(std::string key);
    
    static void onCleanup(CLEAN_FLAGS flags = CLEAN_FLAGS::EVERYTHING);
    
private:
    static std::map<std::string, CSprite*> _Sprites;
    static std::map<std::string, CSpriteContainer*> _SpriteContainers;
    static std::map<std::string, CSpriteSheet*> _SpriteSheets;
    static std::map<std::string, TTF_Font*> _Fonts;
    static std::map<std::string, CLuaScript*> _LuaScripts;
    static int _assetId;
    
};

#endif /* defined(__Third__CAssetManager__) */
