//
//  NFile.h
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__NFile__
#define __Third__NFile__

#include <stdio.h>
#include "CInstance.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

struct EntityParameterHolder {
    Box box;
    sf::Color color;
    std::string spriteContainerKey;
    EntityParameterHolder(Box box, sf::Color color) : box(box), color(color), spriteContainerKey("") {}
    EntityParameterHolder(Box box, std::string spriteContainerKey) : box(box), spriteContainerKey(spriteContainerKey) {}
};

class NFile {
    
public:
    
    // Load functions
    static rapidjson::Document loadJsonFile(std::string fileName);
    static void loadMap(std::string fileName, CInstance* instance);
    
    // Save functions
    
    
    // Misc functions
    static void success(std::string successMsg);
    static void error(std::string errorMsg);       // Todo replace all couts with this
    static void warning(std::string warningMsg);
    
private:
    static CEntity* createEntity(CInstance* instance, const rapidjson::Value& jsonEntity, EntityParameterHolder entityParameterHolder);
    
};

#endif /* defined(__Third__NFile__) */
