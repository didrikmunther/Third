//
//  main.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include "CServer.h"


int main(int argc, const char * argv[]){
    
    int returned = -1;
    
    for(int i = 0; i < argc; i++)
        std::cout << "argnr " << i << ": " << argv[i] << "\n";
    
//    CServer server(60);
//    returned = server.onExecute();
    
    if(argc >= 2) {
        std::string argv1 = argv[1];
        if(argv1 == "+server") {
            std::string tickrate = argv[2];
            CServer server(stoi(tickrate));
            returned = server.onExecute();
        } else {
            CGame game;
            returned = game.onExecute();
        }
    }
    
    printf("Good bye!");
    return returned;
}