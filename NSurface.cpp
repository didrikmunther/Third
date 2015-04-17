//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"

void NSurface::renderRect(int x, int y, int w, int h, SDL_Renderer *renderer, int r, int g, int b) {
    SDL_Rect rect;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void NSurface::renderRect(SDL_Rect rect, SDL_Renderer *renderer, int r, int g, int b) {
    
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void NSurface::renderSprite(CSprite* sprite, SDL_Renderer* renderer, SDL_Rect playerRect) {
    
    SDL_Rect src{sprite->getSource()->x, sprite->getSource()->y, sprite->getSource()->w, sprite->getSource()->h};
    SDL_RenderCopy(renderer, sprite->getSpriteSheet()->getTexture(), &src, &playerRect);
}