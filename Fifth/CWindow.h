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
#include <string>
#include <SDL2/SDL.h>


class CWindow {
    
public:
    CWindow();
    
    int onInit(std::string title, int width, int height);
    int newWindow(std::string title, int width, int height);
    void setTitle(std::string title);
    void onCleanup();
    
    int getWidth();
    int getHeight();
    
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
    
//    void updateView(int width, int height);
//    void setSize(int width, int height);
    
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    
    //sf::View _getLetterboxView(sf::View view, int windowWidth, int windowHeight);
    
    int _screenHeight, _screenWidth;
    
    
    
    
};

#endif /* defined(__Third__CWindow__) */
