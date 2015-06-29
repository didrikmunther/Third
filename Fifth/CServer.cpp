//
//  CServer.cpp
//  Fifth-Server
//
//  Created by Didrik Munther on 21/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CServer.h"
#include <iostream>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

CServer::CServer() :
    _isRunning(false), _inputThread(&CServer::_handleInput, this),
    _ns(1000.0f / (float)60), _delta(0), _frames(0), _updates(0),
    _lastTime(_clock.getElapsedTime().asMilliseconds()), _timer(_clock.getElapsedTime().asMilliseconds()) {
}

int CServer::onExecute() {
    
    std::cout << "Starting server...\n";
    
    if(_onInit() == -1)
        return -1;
    
    while(_isRunning) {
        
        float now = _clock.getElapsedTime().asMilliseconds();
        _delta += (now - _lastTime) / _ns;
        _lastTime = now;
        
        while(_delta >= 1) {
            _onLoop();
            
            _updates++;
            _delta--;
            
            _frames++;
            
            if(_clock.getElapsedTime().asMilliseconds() - _timer > 1000) {
                _timer += 1000;
                _updates = 0;
                _frames = 0;
            }
            
        }
        
    }
    
    std::cout << "Shutting server down.\n";
    
    return _onCleanup();
        
}

int CServer::_onInit() {
    
    const char* jsonString = "  {\"entities\":\
                                    [\
                                        {\"name\":\"m:aTree\",\"posX\":10,\"posY\":20},\
                                        {\"name\":\"l:aBush\",\"posX\":23,\"posY\":43}\
                                    ]\
                                }";
    
    rapidjson::Document d;
    
    if(!d.Parse<0>(jsonString).HasParseError()) {
        const rapidjson::Value& entities = d["entities"];
        for(rapidjson::SizeType i = 0; i < entities.Size(); i++) {
            const rapidjson::Value& entity = entities[i];
            printf("Entity: \"%s\", Pos: %s, %s\n", entity["name"].GetString(), std::to_string(entity["posX"].GetInt()).c_str(), std::to_string(entity["posY"].GetInt()).c_str());
            
        }
    }
    
    _inputThread.launch();
    
    _isRunning = true;
    
    std::cout << "Server started!\n";
    return 0;
}

void CServer::_onLoop() {
    {
        auto instances = _instanceVector.begin();
        while(instances != _instanceVector.end()) {
            auto instance = instances->second;
            instance->entityManager.onLoop();
            
            auto clients = instance->_clients.begin();
            while(clients != instance->_clients.end()) {
                auto client = clients->second;
                
                // ADD ALL ENTITIES INTO A JSON STRING AND SEND IT TO THE CLIENTS TCP SOCKET
            }
            
            ++instances;
        }
    }
}

void CServer::_handleInput() {
    
    while(_isRunning) {
        std::string i; getline(std::cin, i);
        
        if     (i == "")            {  }
        else if(i == "quit")        { _isRunning = false; }
        else if(i == "addentity")   { /* instance.entityManager.addEntity(Box{0, 0, 0, 0}, sf::Color{255,0,255}); */ }
        else                        { std::cout << "Command not recognized.\n"; }
    }
    
}

int CServer::_onRestart() {
    _onCleanup();
    return 0;
}

int CServer::_onCleanup() {
    
    {
        auto i = _instanceVector.begin();
        while(i != _instanceVector.end()) {
            delete i->second;
            _instanceVector.erase(i++->first);
        }
        _instanceVector.clear();
    }
    
    return 0;
}