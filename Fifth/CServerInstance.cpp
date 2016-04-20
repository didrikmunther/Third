//
//  CServerInstance.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CServerInstance.h"
#include "CGame.h"
#include "CClient.h"


int gameLoop(void* data) {
    auto game = static_cast<CGame*>(data);
    game->onExecute("");
    
    return 1;
}

CServerInstance::CServerInstance(std::string name, CGame* game)
    : game(game)
    , name(name)
{
    game->_onInit();
    
    auto quickSave = NFile::readFromFile("resources/quicksave.save");
    
    if(quickSave != "") {
        rapidjson::Document d;
        d.Parse(quickSave.c_str());
        game->instance.entityManager.onCleanup();
        game->instance.entityManager.onDeserialize(&d["this"], &game->instance);
//        NFile::loadTemplate("player.templ", &game->instance);
    }
    
    loopThread = SDL_CreateThread(gameLoop, (name + "gameLoop").c_str(), (void*)game);
}

void CServerInstance::addClient(CClient* client) {
    clients.push_back(client);
    client->instance = this;
}

int CServerInstance::removeClient(CClient* client) {
    auto it = clients.begin();
    while(it != clients.end()) {
        if(*it == client) {
            clients.erase(it);
            return 0;
        }
        
        ++it;
    }
    return -1;
}

void CServerInstance::closeInstance() {
    game->instance.closeInstance();
}