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

#include <fstream>
#include <iostream>

struct EntityParameterHolder {
    Box box;
    sf::Color color;
    std::string spriteContainerKey;
    EntityParameterHolder(Box box, sf::Color color) : box(box), color(color), spriteContainerKey("") {}
    EntityParameterHolder(Box box, std::string spriteContainerKey) : box(box), spriteContainerKey(spriteContainerKey) {}
};

enum LogType {
    SUCCESS = 0,
    ERROR,
    WARNING,
    ALERT
};

class NFile {
    
public:
    
    // Load functions
    static rapidjson::Document loadJsonFile(std::string fileName);
    static void loadMap(std::string fileName, CInstance* instance);
    
    // Save functions
    template<typename... T>
    static void writeToFile(std::string fileName, T&&... t) {
        std::ofstream file(fileName, std::ios::app);
        _writeToFile(file, std::forward<T>(t)...);
        file.close();
    }
    
    static void clearFile(std::string fileName);
    
    // Misc functions
    
    template<typename... T>
    static void log(LogType type, T&&... t) {
        
        std::string alert = "";
        
        switch(type) {
            case LogType::SUCCESS:
                alert = "[SUCCESS] ";
                break;
                
            case LogType::ERROR:
                alert = "[ERROR] ";
                break;
                
            case LogType::WARNING:
                alert = "[WARN] ";
                break;
                
            case LogType::ALERT:
                alert = "[ALERT] ";
                break;
                
            default:
                alert = "[] ";
                break;
        }
        
        _print(alert, std::forward<T>(t)..., "\n");
        writeToFile(LOG_FILE, alert, std::forward<T>(t)..., "\n");
        
    }
    
private:
    static CEntity* _createEntity(CInstance* instance, const rapidjson::Value& jsonEntity, EntityParameterHolder entityParameterHolder);
    
    /* 
        Invalid operands error here means that
        you've sent a non-printable object to
        either of the logging functions
    */
    template<typename T>
    static void _print(T &&t) {
        std::cout << t;
    }
    
    template<typename Head, typename... Tail>
    static void _print(Head &&head, Tail&&... tail) {
        std::cout << head;
        _print(std::forward<Tail>(tail)...);
    }
    
    template<typename T>
    static void _writeToFile(std::ofstream& file, T &&t) {
        file << t;
    }
    
    template<typename Head, typename... Tail>
    static void _writeToFile(std::ofstream& file, Head &&head, Tail&&... tail) {
        file << head;
        _writeToFile(file, std::forward<Tail>(tail)...);
    }
    
};

#endif /* defined(__Third__NFile__) */
