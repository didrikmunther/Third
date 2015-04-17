//
//  CSpriteSheet.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSpriteSheet.h"
#include <SDL2_image/SDL_image.h>

CSpriteSheet::CSpriteSheet(SDL_Renderer* renderer, std::string fileName) {
    openFile(renderer, fileName);
}

void CSpriteSheet::openFile(SDL_Renderer* renderer, std::string fileName) {
    
    SDL_Surface* surface = IMG_Load(fileName.c_str());
    //SDL_Surface* surface = SDL_LoadBMP(fileName.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

SDL_Texture* CSpriteSheet::getTexture() {
    return texture;
}

void CSpriteSheet::onCleanup() {
    SDL_DestroyTexture(texture);
}