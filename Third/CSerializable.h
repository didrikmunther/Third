//
//  CSerializable.h
//  Fifth
//
//  Created by Didrik Munther on 23/11/15.
//  Copyright © 2015 Didrik Munther. All rights reserved.
//

#ifndef CSerializable_h
#define CSerializable_h

#include <stdio.h>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


class CInstance;

enum SerializeType {
    SERIALIZE_ALL = 0,
    SERIALIZE_NEW
};

class CSerializable {
    
public:
    CSerializable() {}
    
    virtual void onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance, SerializeType type = SERIALIZE_ALL) { }
    virtual void onDeserialize(const rapidjson::Value* value, CInstance* instance) { }
    
    
    void removeDuplicates(rapidjson::Value* value, const rapidjson::Value* valueBefore);

    void assignString(const rapidjson::Value* value, std::string key, std::string* toAssign);
    void assignFloat(const rapidjson::Value* value, std::string key, float* toAssign);
    template<typename T>
    void assignInt(const rapidjson::Value* value, std::string key, T toAssign) {
        if(!value->HasMember(key.c_str()) && (!value->IsInt() || !value->IsDouble()))
            return;
        
        *toAssign = (*value)[key.c_str()].GetInt();
    }
    
    void addString(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, std::string key, std::string toAdd);
    void addValue(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, std::string key, rapidjson::Value* toAdd);
    template<typename T>
    void addNumber(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, std::string key, T toAdd) {
        value->AddMember(rapidjson::Value(key.c_str(), *alloc), rapidjson::Value(toAdd), *alloc);
    }
    
};

#endif /* CSerializable_h */
