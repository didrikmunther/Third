//
//  CClient.cpp
//  Fifth-Server
//
//  Created by Didrik Munther on 21/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CClient.h"
#include "CServerInstance.h"
#include "CEntity.h"

#include "NNetwork.h"


CClient::CClient(SDLNet_SocketSet* socketSet, TCPsocket socket, int clientId)
    : socket(socket)
    , clientId(clientId)
    , rest("")
{
    SDLNet_TCP_AddSocket(*socketSet, socket);
}

void CClient::initEntities() {
    CEntityManager* entityManager = &instance->game->instance.entityManager;
    controller = entityManager->getEntity(entityManager->addEntity(new CEntity(Box{0, 0, 0, 0}, Color{0, 0, 0, 0})));
    player = entityManager->getEntity("5:Player");
}

void CClient::close(SDLNet_SocketSet* socketSet) {
    SDLNet_TCP_DelSocket(*socketSet, socket);
    SDLNet_TCP_Close(socket);
    
    instance->removeClient(this);
}

void CClient::send(std::string* data) {
    NNetwork::sendPacket(socket, data);
}

void CClient::onEvent(std::string event) {
    
    rapidjson::Document d;
    d.Parse(event.c_str());
    
    
    
//    &d["this"]
    
}