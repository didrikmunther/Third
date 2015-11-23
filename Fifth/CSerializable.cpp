//
//  CSerializable.cpp
//  Fifth
//
//  Created by Didrik Munther on 23/11/15.
//  Copyright © 2015 Didrik Munther. All rights reserved.
//

#include "CSerializable.h"


void CSerializable::assignString(const rapidjson::Value* value, std::string key, std::string* toAssign) {
    if(!value->HasMember(key.c_str()) && !value->IsString())
        return;
    
    *toAssign = (*value)[key.c_str()].GetString();
}

void CSerializable::assignFloat(const rapidjson::Value* value, std::string key, float* toAssign) {
    if(!value->HasMember(key.c_str()) && (!value->IsDouble() || !value->IsInt()))
        return;
    
    *toAssign = (*value)[key.c_str()].GetDouble();
}

void CSerializable::addString(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, std::string key, std::string toAdd) {
    value->AddMember(rapidjson::Value(key.c_str(), *alloc), rapidjson::Value(toAdd.c_str(), *alloc), *alloc);
}

void CSerializable::addValue(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, std::string key, rapidjson::Value* toAdd) {
    value->AddMember(rapidjson::Value(key.c_str(), *alloc), *toAdd, *alloc);
}