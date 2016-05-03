//
//  NFile.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include "NFile.h"
#include "Define.h"
#include "CAssetManager.h"
#include "CEntityManager.h"
#include "CTile.h"
#include "CAnimation.h"


rapidjson::Document NFile::loadJsonFile(std::string fileName) {
    
    rapidjson::Document errorDoc;
    errorDoc.Parse<0>("{\"JSONParsingError\":\"Unknown reason for error (does the json file have a member called JSONParsingError?)\"}");
    
    std::ifstream file(fileName);
    
    if(!file.is_open()) {             // Todo add error msg logging
        errorDoc["JSONParsingError"] = "Couldn't open file";
        return errorDoc;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    rapidjson::Document d;
    
    if(d.Parse<0>(content.c_str()).HasParseError()) {
        errorDoc["JSONParsingError"] = "Couldn't parse file";
        return errorDoc;
    }
    
    return d;
    
}

std::string NFile::readFromFile(std::string fileName) {
    std::ifstream file;
    std::string toReturn = "";
    std::string line;
    
    file.open(fileName.c_str());
    
    if(file.is_open()) {
        while(std::getline(file, line)) {
            toReturn += line;
        }
    } else {
        NFile::log(LogType::WARNING, "Could not open file \"", fileName, "\".");
    }
    
    return toReturn;
}

void NFile::loadAssets(std::string fileName, CInstance* instance) {
    fileName = "resources/map/" + fileName;
    log(LogType::ALERT, "Loading map: \"", fileName.c_str(), "\"");
    
    rapidjson::Document d = loadJsonFile(fileName);
    
    if(d.HasMember("JSONParsingError")) {
        NFile::log(LogType::ERROR, "Could not load map \"", fileName, "\". (Reason: \"", d["JSONParsingError"].GetString(), ")");
        return;
    }
    
    loadFonts(&d);
    loadSpriteSheets(&d, instance);
    loadSprites(&d);
    loadScripts(&d, instance);
    
    log(LogType::SUCCESS, "Loaded map: \"", fileName.c_str(), "\" as \"", d["name"].GetString(), "\"");
}

int NFile::loadLevel(std::string fileName, CInstance* instance) {
    fileName = "resources/level/" + fileName;
    log(LogType::ALERT, "Loading level: \"", fileName.c_str(), "\"");
    
    rapidjson::Document d = loadJsonFile(fileName);
    
    std::string error = "";
    if(d.HasMember("JSONParsingError"))
        error = d["JSONParsingError"].GetString();
    else if(!d["this"].HasMember("player"))
        error = "No player";
    else if(!d["this"].HasMember("controller"))
        error = "No controller";
    
    if(error != "") {
        NFile::log(LogType::ERROR, "Could not load level \"", fileName, "\". (Reason: \"", error, "\")");
        return -1;
    }
    
    instance->entityManager.onDeserialize(&d["this"], instance);
    instance->player = instance->entityManager.getEntity(d["this"]["player"].GetString());
    instance->controller = instance->entityManager.getEntity(d["this"]["controller"].GetString());
    instance->camera->setTarget(instance->player);
    
    NFile::log(LogType::SUCCESS, "Loaded level: \"", fileName.c_str(), "\"");
    
    return 0;
}

void NFile::loadFonts(rapidjson::Document* d) {
    if(!d->HasMember("fonts"))
        return;
    
    const rapidjson::Value& fonts = (*d)["fonts"];                                     // Fonts
    for(rapidjson::SizeType i = 0; i < fonts.Size(); i++) {
        const rapidjson::Value& font = fonts[i];
        if(!(font.HasMember("name") && font.HasMember("path") && font.HasMember("size")))
            continue;
        
        CAssetManager::addFont(font["name"].GetString(), font["path"].GetString(), font["size"].GetInt());
    }
}

void NFile::loadSpriteSheets(rapidjson::Document* d, CInstance* instance) {
    if(!d->HasMember("spriteSheets"))
        return;
    
    const rapidjson::Value& spriteSheets = (*d)["spriteSheets"];                       // Sprite sheets
    for(rapidjson::SizeType i = 0; i < spriteSheets.Size(); i++) {
        const rapidjson::Value& spriteSheet = spriteSheets[i];
        if(!(spriteSheet.HasMember("name") && spriteSheet.HasMember("path")))
            continue;
        
        CAssetManager::addSpriteSheet(spriteSheet["name"].GetString(), instance->window.getRenderer(), spriteSheet["path"].GetString());
    }
}

void NFile::loadSprites(rapidjson::Document* d) {
    if(!d->HasMember("sprites"))
        return;
    
    const rapidjson::Value& sprites = (*d)["sprites"];                                 // Sprites
    for(rapidjson::SizeType i = 0; i < sprites.Size(); i++) {
        const rapidjson::Value& sprite = sprites[i];
        if(!(sprite.HasMember("name") && sprite.HasMember("spriteSheetKey")))
            continue;
        
        if(sprite.HasMember("offsets")) {
            const rapidjson::Value& offsets = sprite["offsets"];
            CAssetManager::addSprite(sprite["name"].GetString(),
                                     sprite["spriteSheetKey"].GetString(),
                                     Box{offsets[0].GetInt(), offsets[1].GetInt(), offsets[2].GetInt(), offsets[3].GetInt()});
        } else if(sprite.HasMember("tileset")) {
            std::string name = sprite["name"].GetString();
            std::string spriteSheetKey = sprite["spriteSheetKey"].GetString();
            int startX = sprite["tileset"][0].GetInt();
            int startY = sprite["tileset"][1].GetInt();
            int tileSize = sprite["tileset"][2].GetInt();
            
            for(int y = 0; y < 4; y++) {
                for(int x = 0; x < 4; x++) {
                    CAssetManager::addSprite(name + std::to_string(y * 4 + x),
                                             spriteSheetKey,
                                             Box{startX + x * tileSize, startY + y * tileSize, tileSize, tileSize});
                }
            }
            
            Tileset* set = new Tileset;
            for(int sprite = 0; sprite < 16; sprite++) {
                set->spriteKeys[sprite] = name + std::to_string(sprite);
            }
            CAssetManager::addTileset(name, set);
        } else if(sprite.HasMember("animation")) {
            const rapidjson::Value& animationValue = sprite["animation"];
            std::string name = sprite["name"].GetString();
            std::string spriteSheet = sprite["spriteSheetKey"].GetString();
            if(!(animationValue.HasMember("frames") && animationValue.HasMember("fps") && animationValue.HasMember("offsets")))
                continue;
            
            int frames = animationValue["frames"].GetInt();
            int fps = animationValue["fps"].GetInt();
            
            const rapidjson::Value& offset = animationValue["offsets"];
            int sX = offset[0].GetInt();
            int sY = offset[1].GetInt();
            int sW = offset[2].GetInt();
            int sH = offset[3].GetInt();
            
            std::vector<std::string> sprites;
            for(int i = 0; i < frames; i++) {
                std::string spriteName = name + std::to_string(i);
                sprites.push_back(spriteName);
                CAssetManager::addSprite(spriteName, spriteSheet, Box(sX + i * sW, sY, sW, sH));
            }
            
            CAnimation* animation = new CAnimation(sprites, fps);
            CAssetManager::addSprite(animation, name);
        }
    }
}

void NFile::loadScripts(rapidjson::Document* d, CInstance* instance) {
    if(!d->HasMember("scripts"))
        return;
    
    const rapidjson::Value& scripts = (*d)["scripts"];
    for(rapidjson::SizeType i = 0; i < scripts.Size(); i++) {
        const rapidjson::Value& script = scripts[i];
        if(!script.IsString())
            return;
        
        std::string path = script.GetString();
        auto luaScript = CAssetManager::addLuaScript(instance->L, "resources/scripts/" + path + ".lua");
        if(path.substr(path.length() - 10) == "Controller" && instance->controller != nullptr) {
            instance->controller->addComponent(instance, luaScript);
        }
    }
}

void NFile::log(int type, std::string msg) {
    std::string prepend = "";
    
    LogType logType = (LogType)type;
    switch(logType) {
        case SUCCESS:
            prepend = "[LSUCCESS] ";
            break;
        case ALERT:
            prepend = "[LALERT] ";
            break;
        case ERROR:
            prepend = "[LERROR] ";
            break;
        case WARNING:
            prepend = "[LWARNING] ";
            break;
    }
    
    msg = prepend + msg + "\n";
    
    _print(msg);
    writeToFile(LOG_FILE, msg);
}

void NFile::clearFile(std::string fileName) {
    std::ofstream file(fileName);
    file.close();
}
