//
//  CAssetManager.cpp
//  Third
//
//  Created by Didrik Munther on 17/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CAssetManager.h"
#include "NFile.h"
#include "CSpriteSheet.h"
#include "CLuaScript.h"
#include "CTile.h"


std::map<std::string, CSprite*> CAssetManager::_Sprites;
std::map<std::string, CSpriteSheet*> CAssetManager::_SpriteSheets;
std::map<std::string, TTF_Font*> CAssetManager::_Fonts;
std::map<std::string, CLuaScript*> CAssetManager::_LuaScripts;
std::map<std::string, Tileset*> CAssetManager::_Tilesets;
int CAssetManager::_assetId = 0;

CAssetManager::CAssetManager() { }


CSprite* CAssetManager::addSprite(std::string name, std::string spriteSheetKey, Box source) {
    if(_Sprites.find(name) != _Sprites.end()) {
        if(DEBUG)
            NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because it already exists.");
        return _Sprites[name];
    } else if(_SpriteSheets.find(spriteSheetKey) == _SpriteSheets.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because the spritesheet \"", spriteSheetKey, "\" didn't exist.");
        return nullptr;
    } else {
        _Sprites[name] = new CSprite(_SpriteSheets[spriteSheetKey], source);
        return _Sprites[name];
    }
}

std::string CAssetManager::addSprite(CSprite* sprite, std::string name /* = "" */) {
    if(name == "")
        name = "reserved:" + std::to_string(_assetId++);
    if(_Sprites.find(name) != _Sprites.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because it already exists.");
        return name;
    } else {
        _Sprites[name] = sprite;
        return name;
    }
}

CSprite* CAssetManager::createSprite(CSpriteSheet* spriteSheet, Box source) {
    if(spriteSheet == nullptr)
        return nullptr;
    
    return new CSprite(spriteSheet, source);
}

CSpriteSheet* CAssetManager::addSpriteSheet(std::string name, SDL_Renderer* renderer, std::string fileName) {
    if(_SpriteSheets.find(name) != _SpriteSheets.end()) {
        NFile::log(LogType::WARNING, "Couldn't add spritesheet: \"", name, "\", because it already exists.");
        return _SpriteSheets[name];
    } else {
        CSpriteSheet* temp = new CSpriteSheet();
        if(temp->openFile(renderer, fileName) != 0) {
            NFile::log(LogType::WARNING, "Couldn't add spritesheet: \"", name, "\", could not open file \"", fileName, "\".");
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Loaded asset: \"", fileName, "\" as \"", name, "\"");
            _SpriteSheets[name] = temp;
            return _SpriteSheets[name];
        }
    }
}

TTF_Font* CAssetManager::addFont(std::string name, std::string fileName, int size) {
    if(_Fonts.find(name) != _Fonts.end()) {
        NFile::log(LogType::WARNING, "Couldn't add font: \"", name, "\", because it already exists.");
        return _Fonts[name];
    } else {
        TTF_Font *temp = TTF_OpenFont(fileName.c_str(), size);
        if(temp == nullptr) {
            NFile::log(LogType::WARNING, "Couldn't add font: \"", name, "\", could not open file \"", fileName, "\".");
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Loaded font: \"", fileName, "\" as \"", name, "\".");
            _Fonts[name] = temp;
            return temp;
        }
    }
}

CLuaScript* CAssetManager::addLuaScript(lua_State* L, std::string path) {
    CLuaScript* script = new CLuaScript(L, path);
    auto name = script->getName();
    
    if(_LuaScripts.find(name) != _LuaScripts.end()) {
        NFile::log(LogType::WARNING, "Couldn't add script: \"", name, "\", because it already exists");
        delete script;
        return _LuaScripts[name];
    } else {
        _LuaScripts[name] = script;
        NFile::log(LogType::SUCCESS, "Loaded script: \"", path, "\" as \"", name, "\".");
        return script;
    }
}

Tileset* CAssetManager::addTileset(std::string name, Tileset* tileSet) {
    if(_Tilesets.find(name) != _Tilesets.end()) {
        NFile::log(LogType::WARNING, "Couldn't add tileset: \"", name, "\", because it already exists");
        return _Tilesets[name];
    } else {
        _Tilesets[name] = tileSet;
        if(DEBUG)
            NFile::log(LogType::SUCCESS, "Added tileset: \"", name, "\".");
        return tileSet;
    }
}

CSprite* CAssetManager::getSprite(std::string key) {
    return _get(key, &_Sprites);
}

CSpriteSheet* CAssetManager::getSpriteSheet(std::string key) {
    return _get(key, &_SpriteSheets);
}

TTF_Font* CAssetManager::getFont(std::string key) {
    return _get(key, &_Fonts);
}

Tileset* CAssetManager::getTileset(std::string key) {
    return _get(key, &_Tilesets);
}

CLuaScript* CAssetManager::getLuaScript(std::string key) {
    return _get(key, &_LuaScripts);
}

void CAssetManager::onCleanup(CLEAN_FLAGS flags /* = CLEAN_FLAGS::EVERYTHING */) {
    
    NFile::log(LogType::ALERT, "Unloading assets!");
    
    _cleanup(&_Sprites, false);
    _cleanup(&_SpriteSheets);
    _cleanup(&_Tilesets);
    
    {
        std::string toWrite = "";
        auto i = _Fonts.begin();
        while(i != _Fonts.end()) {
            toWrite += "\"" + i->first + "\", ";
            TTF_CloseFont(i->second);
            _Fonts.erase(i++->first);
        }
        _Fonts.clear();
        if(toWrite != "")
            NFile::log(LogType::SUCCESS, "Unloaded assets: ", toWrite);
    }
    
    if(!(flags & CLEAN_FLAGS::NOT_LUA_SCRIPTS)) {
        _cleanup(&_LuaScripts);
    }
    
}