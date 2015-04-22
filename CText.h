//
//  CText.h
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CText__
#define __Third__CText__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "CCamera.h"
#include <string>
#include "CAssetManager.h"

class CText {
    
public:
    CText(std::string text, std::string fontKey, CAssetManager* assetManager, SDL_Color color);
    
    void onRender(int x, int y, SDL_Renderer *renderer, CCamera* camera);
    void onRender(int x, int y, SDL_Renderer *renderer);
    
    TTF_Font* getFont();
    std::string* getText();
    SDL_Color* getColor();
    
private:
    std::string text;
    //TTF_Font* font;
    std::string fontKey;
    CAssetManager* assetManager;
    SDL_Color color;
    
};

#endif /* defined(__Third__CText__) */