//
//  NFile.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NFile.h"
#include "Define.h"
#include <iostream>
#include <fstream>
#include "CAssetManager.h"

rapidjson::Document NFile::loadJsonFile(std::string fileName) {
    
    rapidjson::Document errorDoc;
    errorDoc.Parse<0>("{\"JSONParsingError\":\"\"}");
    
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

CEntity* NFile::createEntity(CInstance* instance, const rapidjson::Value& jsonEntity, EntityParameterHolder entityParameterHolder) {
    
    int type;
    if(!jsonEntity.HasMember("type"))
        type = 0;
    else
        type = jsonEntity["type"].GetInt();
    
    switch(type) {
        case EntityTypes::Player:
            CPlayer* player;
            if(entityParameterHolder.spriteContainerKey == "") {
                player = new CPlayer(entityParameterHolder.box, entityParameterHolder.color);
            } else {
                player = new CPlayer(entityParameterHolder.box, entityParameterHolder.spriteContainerKey);
            }
            instance->player = player;
            instance->camera.setTarget(player);
            return player;
            break;
            
        case EntityTypes::Enemy:
            CEnemy* enemy;
            if(entityParameterHolder.spriteContainerKey == "") {
                enemy = new CEnemy(entityParameterHolder.box, entityParameterHolder.color);
            } else {
                enemy = new CEnemy(entityParameterHolder.box, entityParameterHolder.spriteContainerKey);
            }
            
            if(jsonEntity.HasMember("target")) {
                CEntity* target = instance->entityManager.getEntity(jsonEntity["target"].GetString());
                if(target != nullptr)
                    enemy->setTarget(target);
            }
            
            return enemy;
            break;
            
        case EntityTypes::Entity:
        default:
            CEntity* entity;
            if(entityParameterHolder.spriteContainerKey == "") {
                entity = new CEntity(entityParameterHolder.box, entityParameterHolder.color);
            } else {
                entity = new CEntity(entityParameterHolder.box, entityParameterHolder.spriteContainerKey);
            }
            return entity;
            break;
    }
}

void NFile::loadMap(std::string fileName, CInstance* instance) {
    
    rapidjson::Document d = loadJsonFile(fileName);
    
    if(d.HasMember("JSONParsingError")) {
        std::cout << "[ERROR] Loading map \"" << fileName << "\". (Reason: \"" << d["JSONParsingError"].GetString() << ")\n";
        return;
    }
    
    instance->entityManager.onCleanup();
    CAssetManager::onCleanup();
    
    const rapidjson::Value& fonts = d["fonts"];                                     // Fonts
    for(rapidjson::SizeType i = 0; i < fonts.Size(); i++) {
        const rapidjson::Value& font = fonts[i];
        if(!(font.HasMember("name") && font.HasMember("path")))
            continue;
        
        CAssetManager::addFont(font["name"].GetString(), font["path"].GetString());
    }
    
    const rapidjson::Value& shaders = d["shaders"];                                 // Shaders
    for(rapidjson::SizeType i = 0; i < shaders.Size(); i++) {
        const rapidjson::Value& shader = shaders[i];
        if(!(shader.HasMember("name") && shader.HasMember("path") && shader.HasMember("shaderType")))
            continue;
        if(!shader["shaderType"].IsInt() || !(0 <= shader["shaderType"].GetInt() >= 1))
            continue;
        
        CAssetManager::addShader(shader["name"].GetString(), shader["path"].GetString(), (sf::Shader::Type)shader["shaderType"].GetInt());
    }
    
    const rapidjson::Value& spriteSheets = d["spriteSheets"];                       // Sprite sheets
    for(rapidjson::SizeType i = 0; i < spriteSheets.Size(); i++) {
        const rapidjson::Value& spriteSheet = spriteSheets[i];
        if(!(spriteSheet.HasMember("name") && spriteSheet.HasMember("path")))
            continue;
        
        CAssetManager::addSpriteSheet(spriteSheet["name"].GetString(), spriteSheet["path"].GetString());
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
    
    const rapidjson::Value& entities = d["entities"];                               // Entities
    for(rapidjson::SizeType i = 0; i < entities.Size(); i++) {
        const rapidjson::Value& jsonEntity = entities[i];
        if(!(jsonEntity.HasMember("rect") && (jsonEntity.HasMember("spriteContainerKey") || jsonEntity.HasMember("colors"))))
            continue;
        
        CEntity* entity;
        
        if(jsonEntity.HasMember("spriteContainerKey")) {
            const rapidjson::Value& rect = jsonEntity["rect"];
            std::string spriteContainerKey = jsonEntity["spriteContainerKey"].GetString();
            entity = createEntity(instance, jsonEntity, EntityParameterHolder{Box{rect[0].GetInt(), rect[1].GetInt(), rect[2].GetInt(), rect[3].GetInt()}, spriteContainerKey});
        } else {
            const rapidjson::Value& rect = jsonEntity["rect"];
            const rapidjson::Value& colors = jsonEntity["colors"];
            entity = createEntity(instance, jsonEntity, EntityParameterHolder{Box{rect[0].GetInt(), rect[1].GetInt(), rect[2].GetInt(), rect[3].GetInt()}, sf::Color{(sf::Uint8)colors[0].GetInt(), (sf::Uint8)colors[1].GetInt(), (sf::Uint8)colors[2].GetInt(), (sf::Uint8)colors[3].GetInt()}});
        }
        
        std::string entityName;
        
        if(!jsonEntity.HasMember("name") || jsonEntity["name"] == "") {
            entityName = instance->entityManager.addEntity(entity);
        } else {
            entityName = instance->entityManager.addEntity(entity, jsonEntity["name"].GetString());
        }
        
        if(jsonEntity.HasMember("collisionLayers")) {
            entity->collisionLayer = jsonEntity["collisionLayers"].GetInt();
        }
        if(jsonEntity.HasMember("spriteFollowsCollisionBox")) {
            entity->spriteFollowsCollisionBox = jsonEntity["spriteFollowsCollisionBox"].GetBool();
        }
        if(jsonEntity.HasMember("spriteStateTypes")) {
            const rapidjson::Value& spriteStates = jsonEntity["spriteStateTypes"];
            
            for(int j = 0; j < SpriteStateTypes::TOTAL_SPRITESTATETYPES; j++) {
                if(spriteStates.HasMember(std::to_string(j).c_str())) {
                    entity->spriteStateTypes[j] = spriteStates[std::to_string(j).c_str()].GetString();
                }
            }
        }
        
    }
    
    std::cout << "Loaded map: \"" << d["name"].GetString() << "\" (\"" << fileName << "\")\n";
    
}

void NFile::error(std::string errorMsg) {
    // Todo add this
}

void NFile::warning(std::string warningMsg) {
    // Todo add this
}


