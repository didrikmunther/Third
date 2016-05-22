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
std::map<std::string, CMusic*> CAssetManager::_Music;
std::map<std::string, CSound*> CAssetManager::_Sounds;
int CAssetManager::_assetId = 0;

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

CMusic* CAssetManager::addMusic(std::string name, std::string fileName) {
    fileName = "resources/mixer/" + fileName;
    if(_Music.find(name) != _Music.end()) {
        NFile::log(LogType::WARNING, "Couldn't add music: \"", name, "\", because it already exists");
        return _Music[name];
    } else {
        CMusic* music = new CMusic();
        music->loadMusic(fileName);
        if(music->music == nullptr) {
            NFile::log(LogType::WARNING, "Couldn't add music: \"", name, "\", could not open file \"", fileName, "\".");
            delete music;
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Added music: \"", name, "\".");
            _Music[name] = music;
            return music;
        }
    }
}

CSound* CAssetManager::addSound(std::string name, std::string fileName) {
    fileName = "resources/mixer/" + fileName;
    if(_Music.find(name) != _Music.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sound: \"", name, "\", because it already exists");
        return _Sounds[name];
    } else {
        CSound* sound = new CSound();
        sound->loadSound(fileName);
        if(sound->sound == nullptr) {
            NFile::log(LogType::WARNING, "Couldn't add sound: \"", name, "\", could not open file \"", fileName, "\".");
            delete sound;
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Added sound: \"", name, "\".");
            _Sounds[name] = sound;
            return sound;
        }
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

int CAssetManager::playMusic(std::string key, int loops /* = -1 */) {
    auto music = _get(key, &_Music);
    if(music == nullptr) return -1;
    
    music->playMusic(loops);
    
    return 0;
}

int CAssetManager::playSound(std::string key, int loops /* = 0 */) {
    auto sound = _get(key, &_Sounds);
    if(sound == nullptr) return -1;
    
    int i = 0;
    for(; i <= CHANNELS; i++) {
        if(!Mix_Playing(i))
            break;
    }
    if(i == CHANNELS) {
        i = 0;
        Mix_HaltChannel(0); // todo: stop the sound that was started farthest back
    }
//    std::cout << i << "\n";
    
    sound->playSound(i, loops);
    
    return 0;
}

void CAssetManager::onCleanup(CLEAN_FLAGS flags /* = CLEAN_FLAGS::EVERYTHING */) {
    
    NFile::log(LogType::ALERT, "Unloading assets!");
    
    _cleanup(&_Sprites, false);
    _cleanup(&_SpriteSheets);
    _cleanup(&_Tilesets);
    _cleanup(&_Music);
    _cleanup(&_Sounds);
    
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