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

void NSurface::renderSprite(CSprite* sprite, SDL_Renderer* renderer, SDL_Rect destination) {
    
    SDL_Rect src{sprite->getSource()->x, sprite->getSource()->y, sprite->getSource()->w, sprite->getSource()->h};
    SDL_RenderCopy(renderer, sprite->getSpriteSheet()->getTexture(), &src, &destination);
}

void NSurface::renderSprite(CSprite* sprite, SDL_Renderer* renderer, SDL_Rect destination, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect src{sprite->getSource()->x, sprite->getSource()->y, sprite->getSource()->w, sprite->getSource()->h};
    SDL_RenderCopyEx(renderer, sprite->getSpriteSheet()->getTexture(), &src, &destination, angle, center, flip);
}

void NSurface::renderText(int x, int y, CText* text, SDL_Renderer* renderer) {
    SDL_Surface *surface = TTF_RenderText_Blended(text->getFont(), (*text->getText()).c_str(), *text->getColor());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    renderTexture(renderer, SDL_Rect{x, y, w, h}, texture);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void NSurface::renderTexture(SDL_Renderer* renderer, SDL_Rect destination, SDL_Texture *texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &destination);
}





