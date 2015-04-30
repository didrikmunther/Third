//
//  CWindow.h
//  Third
//
//  Created by Didrik Munther on 19/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CWindow__
#define __Third__CWindow__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>

class CWindow {
    
public:
    CWindow();
    
    int onInit(std::string title, int width, int height, int window_flags, int renderer_flags);
    int newWindow(std::string title, int width, int height, int window_flags, int renderer_flags);
    void setTitle(std::string title);
    void onCleanup();
    
    int getWidth();
    int getHeight();
    
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    
private:
    int screenHeight, screenWidth;
    
    SDL_Renderer* renderer;
    SDL_Window* window;
    
    
};

#endif /* defined(__Third__CWindow__) */
