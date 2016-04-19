//
//  NNetwork.cpp
//  Fifth
//
//  Created by Didrik Munther on 17/04/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "NNetwork.h"
#include "NFile.h"

#include "Define.h"


int NNetwork::sendToSocket(TCPsocket sock, const char* data, int len /* = -1 */) {
    len = len == -1 ? (int)strlen(data) + 1 : len;
    if(SDLNet_TCP_Send(sock, (void*)data, len) < len) {
        NFile::log(LogType::ERROR, "Failed to send message: ", SDL_GetError());
        return -1;
    }
    return 0;
}

unsigned long bufferContains(std::string buffer, const char* contains) {
    std::string c = contains;
    return buffer.find(contains);
}

void NNetwork::sendPacket(TCPsocket sock, std::string* data) {
    NNetwork::sendToSocket(sock, (PACKET_START + *data + PACKET_ENDING).c_str());
}


/*
 Function returns a pair where first is the recieved packet, and second is the beginning of the next packet.
 If recieved packet is an empty string the whole packet has not been recieved yet.
 Remeber to send the beginning of the next packet as rest.
 */
std::pair<std::string, std::string> NNetwork::recvPacket(TCPsocket sock, std::string rest) {
    std::string result = "";
    char buffer[PACKET_SIZE];
    
    int serverResponseByteCount = SDLNet_TCP_Recv(sock, buffer, PACKET_SIZE - 1);
    
    buffer[serverResponseByteCount] = 0;
    rest += buffer;
    
    unsigned long startPos = bufferContains(rest, PACKET_START);
    unsigned long endPos   = bufferContains(rest, PACKET_ENDING);
    
    if(startPos != std::string::npos && endPos != std::string::npos) {
        if(!(startPos > endPos))
            result = rest.substr(startPos + strlen(PACKET_START), endPos - startPos - strlen(PACKET_ENDING) - 2);
        rest = rest.substr(endPos + strlen(PACKET_ENDING));
    }
    
    return std::make_pair(result, rest);
}