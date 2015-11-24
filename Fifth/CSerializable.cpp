//
//  CSerializable.cpp
//  Fifth
//
//  Created by Didrik Munther on 23/11/15.
//  Copyright © 2015 Didrik Munther. All rights reserved.
//

#include "CSerializable.h"

#include <iostream>


void CSerializable::removeDuplicates(rapidjson::Value* value, const rapidjson::Value* valueBefore) {
    
    for(rapidjson::Value::ConstMemberIterator i = value->MemberBegin(); i != value->MemberEnd(); i++) {
        const rapidjson::Value* v = &i->value;
        std::string n = i->name.GetString();
        
        std::cout << (valueBefore == nullptr) << "\n";
        
        if(!valueBefore->HasMember(n.c_str()))
            continue;
        
        if(v->IsObject()) {
            if((*valueBefore)[n.c_str()].IsObject()) {
                const rapidjson::Value* tempValue = &(*valueBefore)[n.c_str()];
                std::remove_const<rapidjson::Value*>::type v;
                removeDuplicates(v, tempValue);
            }
            
            continue;
        }
        
        if(v->IsString()) {
            if((*valueBefore)[n.c_str()].IsString()) {
                if(v->GetString() == (*valueBefore)[n.c_str()].GetString())
                    value->RemoveMember(n.c_str());
            }
            
            continue;
        }
        
        if(v->IsInt()) {
            if((*valueBefore)[n.c_str()].IsInt()) {
                if(v->GetInt() == (*valueBefore)[n.c_str()].GetInt())
                    value->RemoveMember(n.c_str());
            }
            
            continue;
        }
        
        if(v->IsDouble()) {
            if((*valueBefore)[n.c_str()].IsDouble()) {
                if(v->GetDouble() == (*valueBefore)[n.c_str()].GetDouble())
                    value->RemoveMember(n.c_str());
            }
            
            continue;
        }
    }
    
}

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