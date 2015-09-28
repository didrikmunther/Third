//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"
#include "CSpriteContainer.h"
#include "CSpriteSheet.h"
#include "CSprite.h"
#include "CText.h"
#include "CWindow.h"
#include "CCamera.h"


Triangle Triangle::normalizeWithCamera(CCamera* camera) {
    return Triangle{a.normalizeWithCamera(camera), b.normalizeWithCamera(camera), c.normalizeWithCamera(camera)};
}

Line Line::normalizeWithCamera(CCamera* camera) {
    return Line{x - camera->offsetX(), y - camera->offsetY(), x2 - camera->offsetX(), y2 - camera->offsetY(), color};
}

void NSurface::renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a /* = 255 */) {
    SDL_Rect rect;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    SDL_SetRenderDrawColor(window->getRenderer(), r, g, b, a);
    SDL_RenderFillRect(window->getRenderer(), &rect);
}

void NSurface::renderSprite(int x, int y, int w, int h, CSprite *sprite, CWindow *window, SDL_RendererFlip flip, int a /* = 255 */) {
    if(sprite == nullptr) return;
    SDL_Rect* src = sprite->getSource();
    SDL_Rect destination = {x, y, w, h};
    SDL_SetTextureAlphaMod(sprite->getSpriteSheet()->getTexture(), a);
    SDL_RenderCopyEx(window->getRenderer(), sprite->getSpriteSheet()->getTexture(), src, &destination, 0, nullptr, flip);
}

void NSurface::renderText(int x, int y, CText* text, CWindow* window) {
    SDL_Surface *surface = TTF_RenderText_Blended(text->getFont(), (*text->getText()).c_str(), *text->getColor());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    renderTexture(x, y, w, h, window->getRenderer(), texture);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void NSurface::renderTexture(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture *texture) {
    SDL_Rect destination = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, nullptr, &destination);
}

void NSurface::renderLine(Line line, SDL_Renderer* renderer, CCamera* camera /* = nullptr */) {
    if(camera) {
        line = line.normalizeWithCamera(camera);
//        if(!camera->collision(line.x, line.y, line.x2, line.y2))
//            return;
    }
    
    SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
    SDL_RenderDrawLine(renderer, line.x, line.y, line.x2, line.y2);
}

void NSurface::renderTriangle(Triangle triangle, SDL_Renderer* renderer, CCamera* camera /* = nullptr */) {
    if(camera)
        triangle = triangle.normalizeWithCamera(camera);
    
    renderLine(triangle.a, renderer);
    renderLine(triangle.b, renderer);
    renderLine(triangle.c, renderer);
}


