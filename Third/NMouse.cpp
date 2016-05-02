//
//  NMouse.cpp
//  Third
//
//  Created by Didrik Munther on 16/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <SDL2/SDL.h>

#include "NMouse.h"
#include "CCamera.h"


int NMouse::absoluteMouseX() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x;
}

int NMouse::absoluteMouseY() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y;
}

int NMouse::relativeMouseX(CCamera* camera) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x + camera->offsetX();
}

int NMouse::relativeMouseY(CCamera* camera) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y + camera->offsetY();
}

bool NMouse::leftMouseButtonPressed() {
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool NMouse::rightMouseButtonPressed() {
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
}