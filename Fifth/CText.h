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
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "CRenderable.h"


class CCamera;
class CWindow;

class CText : public CRenderable {
    
public:
    CText(std::string text, int size, std::string fontKey, SDL_Color color);
    
    void onRender(int x, int y, CWindow* window, CCamera* camera);
    void onRender(int x, int y, CWindow* window);
    
    TTF_Font* getFont();
    int getSize();
    std::string* getText();
    SDL_Color* getColor();
    
private:
    std::string _text;
    int _size;
    std::string _fontKey;
    SDL_Color _color;
    
};

#endif /* defined(__Third__CText__) */