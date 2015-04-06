//
//  CEntity.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntity__
#define __Third__CEntity__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "CBody.h"

class CCamera;

class CEntity {
    
public:
    CEntity(SDL_Rect rect, SDL_Color color);
    
    void onLoop(std::vector<std::shared_ptr<CEntity>>* entities);
    virtual void doLogic();
    void move(std::vector<std::shared_ptr<CEntity>>* entities);
    bool collision(int x, int y, std::vector<std::shared_ptr<CEntity>>* entities);
    void onRender(SDL_Renderer *renderer, CCamera* camera);
    
    CBody body;
    bool toRemove;
    
protected:
    SDL_Color color;
    
};

#endif /* defined(__Third__CEntity__) */
