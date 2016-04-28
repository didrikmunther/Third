//
//  CServerInstance.h
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CServerInstance__
#define __Fifth__CServerInstance__

#include <stdio.h>
#include <map>

#include "CEntityManager.h"
#include "CClient.h"
#include "CInstance.h"
#include "CGame.h"


class CServerInstance {
    
public:
    CServerInstance(std::string name, CGame* game = new CGame(GameType::SERVER));
    
    std::string name;
    std::vector<CClient*> clients;
    
    CGame* game;
    SDL_Thread* loopThread;
    
    void closeInstance();
    
};

#endif /* defined(__Fifth__CServerInstance__) */
