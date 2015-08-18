//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"
#include "Define.h"
#include "CSpriteContainer.h"
#include <iostream>

void NSurface::renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a) {
    SDL_Rect rect;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    SDL_SetRenderDrawColor(window->getRenderer(), r, g, b, 255);
    SDL_RenderFillRect(window->getRenderer(), &rect);
}

void NSurface::renderRect(SDL_Rect* rect, CWindow* window, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(window->getRenderer(), r, g, b, 255);
    SDL_RenderFillRect(window->getRenderer(), rect);
}

void NSurface::renderEntity(CEntity* entity, CWindow* window, int x, int y) {
    if(entity->getSpriteContainer()->getSprite() == nullptr) return;
    SDL_Rect* src = entity->getSpriteContainer()->getSprite()->getSource();
    SDL_Rect destination = {x, y, 0, 0};
    SDL_SetTextureAlphaMod(entity->getSpriteContainer()->getSprite()->getSpriteSheet()->getTexture(), entity->getTransparency());
    
    int spriteWidth, spriteHeight = 0;
    if(entity->spriteFollowsCollisionBox) {
        destination.w = entity->body.getW();
        destination.h = entity->body.getH();
    } else {
        destination.w = entity->getSpriteContainer()->spriteArea.w;
        destination.h = entity->getSpriteContainer()->spriteArea.h;
    }
    
    SDL_RendererFlip flip;
    if(entity->hasProperty(EntityProperty::FLIP))
        flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
    else
        flip = SDL_RendererFlip::SDL_FLIP_NONE;
    
    SDL_RenderCopyEx(window->getRenderer(), entity->getSpriteContainer()->getSprite()->getSpriteSheet()->getTexture(), src, &destination, 0, nullptr, flip);
    
}

void NSurface::renderText(int x, int y, CText* text, CWindow* window) {
    SDL_Surface *surface = TTF_RenderText_Blended(text->getFont(), (*text->getText()).c_str(), *text->getColor());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    renderTexture(window->getRenderer(), SDL_Rect{x, y, w, h}, texture);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void NSurface::renderTexture(SDL_Renderer* renderer, SDL_Rect destination, SDL_Texture *texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &destination);
}


