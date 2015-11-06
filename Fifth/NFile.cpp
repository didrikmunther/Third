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

void NFile::loadMap(std::string fileName, CInstance* instance) {
    
    log(LogType::ALERT, "Loading map: \"", fileName.c_str(), "\"");
    
    rapidjson::Document d = loadJsonFile(fileName);
    
    if(d.HasMember("JSONParsingError")) {
        NFile::log(LogType::ERROR, "Could not load map \"", fileName, "\". (Reason: \"", d["JSONParsingError"].GetString(), ")");
    } else {
        
        //instance->entityManager.onCleanup();
        //CAssetManager::onCleanup();
        //instance->player = nullptr;
        //instance->controller = nullptr;

        const rapidjson::Value& fonts = d["fonts"];                                     // Fonts
        for(rapidjson::SizeType i = 0; i < fonts.Size(); i++) {
            const rapidjson::Value& font = fonts[i];
            if(!(font.HasMember("name") && font.HasMember("path") && font.HasMember("size")))
                continue;
            
            CAssetManager::addFont(font["name"].GetString(), font["path"].GetString(), font["size"].GetInt());
        }

        const rapidjson::Value& spriteSheets = d["spriteSheets"];                       // Sprite sheets
        for(rapidjson::SizeType i = 0; i < spriteSheets.Size(); i++) {
            const rapidjson::Value& spriteSheet = spriteSheets[i];
            if(!(spriteSheet.HasMember("name") && spriteSheet.HasMember("path")))
                continue;
            
            CAssetManager::addSpriteSheet(spriteSheet["name"].GetString(), instance->window.getRenderer(), spriteSheet["path"].GetString());
        }

        const rapidjson::Value& sprites = d["sprites"];                                 // Sprites
        for(rapidjson::SizeType i = 0; i < sprites.Size(); i++) {
            const rapidjson::Value& sprite = sprites[i];
            if(!(sprite.HasMember("name") && sprite.HasMember("spriteSheetKey") && sprite.HasMember("offsets")))
                continue;
            
            const rapidjson::Value& offsets = sprite["offsets"];
            CAssetManager::addSprite(sprite["name"].GetString(),
                                     sprite["spriteSheetKey"].GetString(),
                                     Box{offsets[0].GetInt(), offsets[1].GetInt(), offsets[2].GetInt(), offsets[3].GetInt()});
        }

        const rapidjson::Value& containers = d["containers"];                           // Sprite containers
        for(rapidjson::SizeType i = 0; i < containers.Size(); i++) {
            const rapidjson::Value& container = containers[i];
            if(!(container.HasMember("name") && container.HasMember("spriteKey")))
                continue;
            
            if(container.HasMember("area")) {
                const rapidjson::Value& area = container["area"];
                CAssetManager::addSpriteContainer(container["name"].GetString(),
                                                  container["spriteKey"].GetString(),
                                                  Area{area[0].GetInt(), area[1].GetInt()});
            } else {
                CAssetManager::addSpriteContainer(container["name"].GetString(),
                                                  container["spriteKey"].GetString());
            }
        }

//        const rapidjson::Value& entities = d["entities"];                               // Entities
//        for(rapidjson::SizeType i = 0; i < entities.Size(); i++) {
//            const rapidjson::Value& jsonEntity = entities[i];
//            if(!(jsonEntity.HasMember("rect") && (jsonEntity.HasMember("spriteContainerKey") || jsonEntity.HasMember("colors"))))
//                continue;
//            
//            CEntity* entity;
//            
//            if(jsonEntity.HasMember("spriteContainerKey")) {
//                const rapidjson::Value& rect = jsonEntity["rect"];
//                std::string spriteContainerKey = jsonEntity["spriteContainerKey"].GetString();
//                entity = _createEntity(instance, jsonEntity, EntityParameterHolder{Box{rect[0].GetInt(), rect[1].GetInt(), rect[2].GetInt(), rect[3].GetInt()}, spriteContainerKey});
//            } else {
//                const rapidjson::Value& rect = jsonEntity["rect"];
//                const rapidjson::Value& colors = jsonEntity["colors"];
//                entity = _createEntity(instance, jsonEntity, EntityParameterHolder{Box{rect[0].GetInt(), rect[1].GetInt(), rect[2].GetInt(), rect[3].GetInt()}, SDL_Color{(Uint8)colors[0].GetInt(), (Uint8)colors[1].GetInt(), (Uint8)colors[2].GetInt(), (Uint8)colors[3].GetInt()}});
//            }
//            
//            std::string entityName;
//            
//            if(!jsonEntity.HasMember("name") || jsonEntity["name"] == "") {
//                entityName = instance->entityManager.addEntity(entity);
//            } else {
//                entityName = instance->entityManager.addEntity(entity, jsonEntity["name"].GetString());
//            }
//            
//            if(jsonEntity.HasMember("collisionLayers")) {
//                entity->collisionLayer = jsonEntity["collisionLayers"].GetInt();
//            }
//            if(jsonEntity.HasMember("spriteFollowsCollisionBox")) {
//                entity->spriteFollowsCollisionBox = jsonEntity["spriteFollowsCollisionBox"].GetBool();
//            }
//            if(jsonEntity.HasMember("spriteStateTypes")) {
//                const rapidjson::Value& spriteStates = jsonEntity["spriteStateTypes"];
//                
//                for(int j = 0; j < SpriteStateTypes::TOTAL_SPRITESTATETYPES; j++) {
//                    if(spriteStates.HasMember(std::to_string(j).c_str())) {
//                        entity->spriteStateTypes[j] = spriteStates[std::to_string(j).c_str()].GetString();
//                    }
//                }
//            }
//        }
        
        log(LogType::SUCCESS, "Loaded map: \"", fileName.c_str(), "\" as \"", d["name"].GetString(), "\"");
    }
    
//    if(instance->player == nullptr) {       // In case there is no player in the .map
//        instance->player = new CPlayer(Box{0, 0, 10, 10}, SDL_Color{255, 0, 255, 0});
//        instance->entityManager.addEntity(instance->player, "m:player2");
//        instance->camera.setTarget(instance->player);
//    }
}

void NFile::clearFile(std::string fileName) {
    std::ofstream file(fileName);
    file.close();
}
