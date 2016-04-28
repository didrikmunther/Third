//
//  CServer.cpp
//  Fifth-Server
//
//  Created by Didrik Munther on 21/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <string>

#include "CServer.h"
#include "NFile.h"
#include "NNetwork.h"
#include "CEntity.h"


CServer::CServer(float tickrate, int maxClients /* = 10 */)
    : _isRunning(false)
    , _ns(1000.0f / tickrate), _delta(0), _frames(0), _updates(0)
    , _lastTime(SDL_GetTicks()), _timer(SDL_GetTicks())
    , maxClients(maxClients), maxSockets(maxClients + 1), clientCount(0)
    , currClientId(0)
{
}

int _handleInput(void* data) {
    
    auto self = static_cast<CServer*>(data);
    
    while(self->_isRunning) {
        std::string i;
        getline(std::cin, i);
        
        if (std::cin.eof())         { self->_isRunning = false; }
        
        if     (i == "")            {  }
        else if(i == "quit")        { self->_isRunning = false; }
        else                        { std::cout << "Command not recognized.\n"; }
    }
    
    return 1;
}

int _broadcastLoop(void* data) {
    auto self = static_cast<CServer*>(data);
    
    while(self->_isRunning) {
        float now = SDL_GetTicks();
        self->_delta += (now - self->_lastTime) / self->_ns;
        self->_lastTime = now;
        
        while(self->_delta >= 1) {
            self->_onLoop();
            
            self->_updates++;
            self->_delta--;
            
            if(SDL_GetTicks() - self->_timer > 1000) {
                self->_timer += 1000;
                self->_updates = 0;
            }
            
        }
    }
    
    return 1;
}

void resetKeyStates(Uint8* keystates) {
    for(int i = 0; i < SDL_NUM_SCANCODES; i++)
        keystates[i] = false;
}

int CServer::onExecute() {
    
    NFile::log(LogType::ALERT, "Starting server...");
    
    int port = 2000;
    
    if(_onInit() == -1)
        return -1;
    
    socketSet = SDLNet_AllocSocketSet(maxSockets);
    
    /*int hostResolved =*/ SDLNet_ResolveHost(&serverIP, nullptr, port);
    serverSocket = SDLNet_TCP_Open(&serverIP);
    SDLNet_TCP_AddSocket(socketSet, serverSocket);
    
    Uint8 keystates[SDL_NUM_SCANCODES];
    resetKeyStates(keystates);
    
    CServerInstance* inst = new CServerInstance("start");
    _instances["start"] = inst;
    
    while(_isRunning) {
        
        /*int activeSockets = */SDLNet_CheckSockets(socketSet, 1000);
        int serverSocketActivity = SDLNet_SocketReady(serverSocket);
        
        if(serverSocketActivity != 0) {
            if(clientCount < maxClients) {
                clients[currClientId] = new CClient(&socketSet, SDLNet_TCP_Accept(serverSocket), currClientId);
                auto client = clients[currClientId];
                clientCount++;
                
                strcpy(buffer, "OK");
                NNetwork::sendToSocket(client->socket, buffer);
                
                NFile::log(LogType::ALERT, "Client connected: ", currClientId, ".");
                
                _instances["start"]->addClient(client);
                client->initEntities();
                currClientId++;
                
            } else {
                NFile::log(LogType::ALERT, "Client tried to connect, but server was full.");
                TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);
                NNetwork::sendToSocket(tempSock, "FULL");
                SDLNet_TCP_Close(tempSock);
            }
        }
        
        auto it = clients.begin();
        while(it != clients.end()) {
            auto client = (*it).second;
            auto socket = (*it).second->socket;
            
            int clientSocketActivity = SDLNet_SocketReady(socket);
            if(clientSocketActivity != 0) {
                int recievedByteCount = SDLNet_TCP_Recv(socket, buffer, PACKET_SIZE - 1);
                if(recievedByteCount <= 0) {
                    buffer[recievedByteCount] = 0;
                    NFile::log(LogType::ALERT, "Client ", (client->clientId), " disconnected.");
                    client->close(&socketSet);
                    clientCount--;
                    delete client;
                    clients.erase(it++);
                    continue;
                } else {
                    auto result = NNetwork::recvPacket(socket, client->rest);
                    client->rest = result.second;
                    auto recieved = result.first;
                    
                    if(recieved != "") {
                        std::cout << recieved << "\n";
//                        broadcast(i, &recieved);
                        rapidjson::Document d;
                        d.Parse(recieved.c_str());
                        auto t = &d["this"];
                        
                        if(t->HasMember("keystates")) {
                            resetKeyStates(keystates);
                            const rapidjson::Value& jkeystates = (*t)["keystates"];                       // Sprite sheets
                            for(rapidjson::SizeType i = 0; i < jkeystates.Size(); i++) {
                                const rapidjson::Value& jkeystate = jkeystates[i];
                                keystates[jkeystate.GetInt()] = true;
                            }
                            std::cout << "here\n";
                            
                            auto mutex = client->instance->game->mutex;
                            SDL_LockMutex(mutex);
                            client->player->onKeyStates(&client->instance->game->instance, keystates);
                            SDL_UnlockMutex(mutex);
                        }
                    }
                }
            }
            
            ++it;
        }
        
    }
    
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(serverSocket);
    
    NFile::log(LogType::SUCCESS, "Shutting server down.");
    
    return _onCleanup();
        
}

void CServer::broadcast(CClient* except, std::string* data) {
    for(auto& client: clients) {
        if(client.second != except)
            client.second->send(data);
    }
}

int CServer::_onInit() {
    
    NFile::_initRelativePaths();
    SDLNet_Init();
    
    _isRunning = true;
    
    _inputThread = SDL_CreateThread(_handleInput, "handleInputThread", (void*)this);
    _broadcastThread = SDL_CreateThread(_broadcastLoop, "broadcastThread", (void*)this);
    
    NFile::log(LogType::SUCCESS, "Server started!");
    return 0;
}

void CServer::_onLoop() {
    {
        auto instances = _instances.begin();
        while(instances != _instances.end()) {
            auto sinstance = instances->second;
            
            rapidjson::Document d;
            d.Parse("{}");
            
            rapidjson::Value entityValues(rapidjson::kObjectType);
            
            SDL_LockMutex(sinstance->game->mutex);
            sinstance->game->instance.entityManager.onSerialize(&entityValues, &d.GetAllocator(), &sinstance->game->instance);
            SDL_UnlockMutex(sinstance->game->mutex);
            
            d.AddMember("this", entityValues, d.GetAllocator());
            
            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
            d.Accept(writer);
            
            std::string toSend = sb.GetString();
            
            for(auto& client: sinstance->clients) {
                client->send(&toSend);
            }
            
            ++instances;
        }
    }
}

int CServer::_onRestart() {
    _onCleanup();
    return 0;
}

int CServer::_onCleanup() {
    {
        auto i = _instances.begin();
        while(i != _instances.end()) {
            i->second->closeInstance();
            delete i->second;
            _instances.erase(i++->first);
        }
        _instances.clear();
    }
    {
        auto i = clients.begin();
        while(i != clients.end()) {
            delete i->second;
            clients.erase(i++->first);
        }
        clients.clear();
    }
    
    SDLNet_Quit();
    
    return 0;
}