//
//  CSpriteSheet.h
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CSpriteSheet__
#define __Third__CSpriteSheet__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>

class CSpriteSheet {
    
public:
    CSpriteSheet();
    int openFile(SDL_Renderer* renderer, std::string fileName);
    void onCleanup();
    
    SDL_Texture* getTexture();
    
private:
    SDL_Texture* _texture;
    
};

#endif /* defined(__Third__CSpriteSheet__) */
