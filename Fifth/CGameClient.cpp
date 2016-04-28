//
//  CGameClient.cpp
//  Fifth
//
//  Created by Didrik Munther on 17/04/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//


#include "CGameClient.h"
#include "NNetwork.h"


CGameClient::CGameClient(CGame* game)
    : game(game)
{
    
}

//unsigned long bufferContains(std::string buffer, const char* contains) {
//    std::string c = contains;
//    return buffer.find(contains);
//}

int recvLoop(void* data) {
    auto self = static_cast<CGameClient*>(data);
    
    std::string rest = "";
    
    while(self->game->_isRunning) {
        
        int socketActive = SDLNet_CheckSockets(self->socketSet, 0);
        if(socketActive != 0) {
            int messageFromServer = SDLNet_SocketReady(self->clientSocket);
            if(messageFromServer != 0) {
                auto result = NNetwork::recvPacket(self->clientSocket, rest);
                std::string toDeserialize = result.first;
                rest = result.second;
                
                if(toDeserialize != "") {
                    rapidjson::Document d;
                    d.Parse(toDeserialize.c_str());
                    
                    SDL_LockMutex(self->game->mutex);
                    self->game->instance.entityManager.onDeserialize(&d["this"], &self->game->instance);
                    SDL_UnlockMutex(self->game->mutex);
                    
                    self->game->instance.camera->setTarget(self->game->instance.entityManager.getEntity("5:Player"));
                }
            }
        }
        
    }
    
    return 1;
}

int CGameClient::connect(std::string host, int port) {
    this->host = host;
    this->port = port;
    
    socketSet = SDLNet_AllocSocketSet(1);
    
    int hostResolved = SDLNet_ResolveHost(&serverIP, host.c_str(), port);
    if(hostResolved == -1) {
        NFile::log(LogType::ALERT, "Failed to resolve hostname ", SDL_GetError());
        return -1;
    }
    clientSocket = SDLNet_TCP_Open(&serverIP);
    if(!clientSocket) {
        NFile::log(LogType::ALERT, "Failed to connect to server: ", SDL_GetError());
        return -2;
    }
    
    if(clientSocket) {
        SDLNet_TCP_AddSocket(socketSet, clientSocket);
        /*int activeSockets = */SDLNet_CheckSockets(socketSet, 5000);
        
        int gotResponse = SDLNet_SocketReady(clientSocket);
        
        if(gotResponse != 0) {
            /*int serverResponseByteCount = */SDLNet_TCP_Recv(clientSocket, buffer, PACKET_SIZE);
            NFile::log(LogType::ALERT, "Got response from the server: ", buffer);
            
            if(strcmp(buffer, "OK") == 0) {
                NFile::log(LogType::SUCCESS, "Succesfully joined server");
            } else {
                NFile::log(LogType::ALERT, "Failed to join server, is full.");
                return -3;
            }
        }
    }
    
    recvThread = SDL_CreateThread(recvLoop, "recvLoop", (void*)this);
    
    return 0;
}

void CGameClient::onEvent(SDL_Event* event) {
    int type = event->type;
    int toSend = -1;
    std::string forbiddenCharacter = ""; // This is some mysko
    
    switch(type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            toSend = event->key.keysym.sym;
            break;
    }
    
    std::string complete = "{\"this\":{\"event:\":[" + std::to_string(toSend) + "," + std::to_string(type) + "]}}";

    NNetwork::sendPacket(clientSocket, &complete);
}

void CGameClient::handleKeyStates(const Uint8* keystate) {
    std::string complete = "{\"this\":{\"keystates\":[";
    
    bool isFirst = true;
    for(int i = 0; i < 512; i++) {
        if(keystate[i]) {
            if(!isFirst)
                complete += ", ";
            complete += std::to_string(i);
            isFirst = false;
        }
    }
    complete += "]}}";
    
    NNetwork::sendPacket(clientSocket, &complete);
}