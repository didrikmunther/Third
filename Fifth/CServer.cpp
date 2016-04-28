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

int CServer::onExecute() {
    
    NFile::log(LogType::ALERT, "Starting server...");
    
    int port = 2000;
    
    if(_onInit() == -1)
        return -1;
    
    socketSet = SDLNet_AllocSocketSet(maxSockets);
    
    /*int hostResolved =*/ SDLNet_ResolveHost(&serverIP, nullptr, port);
    serverSocket = SDLNet_TCP_Open(&serverIP);
    SDLNet_TCP_AddSocket(socketSet, serverSocket);
    
    CServerInstance* inst = new CServerInstance("start");
    _instances["start"] = inst;
    
    do {
        
        /*int activeSockets = */SDLNet_CheckSockets(socketSet, 1000);
        int serverSocketActivity = SDLNet_SocketReady(serverSocket);
        
        if(serverSocketActivity != 0) {
            if(clientCount < maxClients) {
                clients[currClientId] = new CClient(&socketSet, SDLNet_TCP_Accept(serverSocket), currClientId);
                auto client = clients[currClientId];
                clientCount++;
                
                strcpy(buffer, "OK");
                NNetwork::sendToSocket(client->socket, buffer);
                std::cout << "Client connected: " << currClientId << ".\n";
                
                _instances["start"]->clients.push_back(client);
                currClientId++;
                
            } else {
                std::cout << "Client tried to connect, but server was full.\n";
                TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);
                NNetwork::sendToSocket(tempSock, "FULL");
                SDLNet_TCP_Close(tempSock);
            }
        }
        
        auto it = clients.begin();
        while(it != clients.end()) {
            auto i = (*it).second;
            auto socket = (*it).second->socket;
            
            int clientSocketActivity = SDLNet_SocketReady(socket);
            if(clientSocketActivity != 0) {
                int recievedByteCount = SDLNet_TCP_Recv(socket, buffer, PACKET_SIZE);
                if(recievedByteCount <= 0) {
                    std::cout << "client " << (i->clientId) << " disconnected.\n";
                    i->close(&socketSet);
                    clientCount--;
                    clients.erase(it++);
                    continue;
                } else {
                    std::cout << "Recieved: >>> " << buffer << " <<< from client number: " << i->clientId << "\n";
                    broadcast(i, buffer);
                }
            }
            
            it++;
        }
        
    } while(_isRunning);
    
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(serverSocket);
    
    NFile::log(LogType::ALERT, "Shutting server down.");
    
    return _onCleanup();
        
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
            
            std::string toSend = PACKET_START;
            toSend += sb.GetString();
            toSend += PACKET_ENDING;
            
            for(auto& client: sinstance->clients) {
                client->send(toSend.c_str());
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