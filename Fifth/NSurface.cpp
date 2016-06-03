//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"
#include "CSpriteSheet.h"
#include "CSprite.h"
#include "CText.h"
#include "CWindow.h"
#include "CCamera.h"


Triangle Triangle::fixCamera(CCamera* camera) {
    return Triangle{a.fixCamera(camera), b.fixCamera(camera), c.fixCamera(camera)};
}

Line Line::fixCamera(CCamera* camera) {
    return Line{x - camera->offsetX(), y - camera->offsetY(), x2 - camera->offsetX(), y2 - camera->offsetY(), color};
}

int NSurface::crossProduct(Position v1, Position v2) {
    return v1.x * v2.y - v2.x * v1.y;
}

void NSurface::renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a /* = 255 */) {
    SDL_Rect rect{x, y, w, h};
    
    SDL_SetRenderDrawColor(window->getRenderer(), r, g, b, a);
    SDL_RenderFillRect(window->getRenderer(), &rect);
}

void NSurface::renderSprite(int x, int y, int w, int h, CSprite *sprite, CWindow *window, SDL_RendererFlip flip, int a /* = 255 */) {
    if(sprite == nullptr) return;
    SDL_Rect src = *sprite->getSource();
    SDL_Rect destination = {x, y, w, h};
    SDL_SetTextureAlphaMod(sprite->getSpriteSheet()->getTexture(), a);
    SDL_RenderCopyEx(window->getRenderer(), sprite->getSpriteSheet()->getTexture(), &src, &destination, 0, nullptr, flip);
}

void NSurface::renderText(int x, int y, CText* text, CWindow* window, float size /* = 1 */) {    
    SDL_Surface *surface = TTF_RenderText_Blended(text->getFont(), (*text->getText()).c_str(), *text->getColor());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    SDL_SetTextureAlphaMod(texture, text->getColor()->a);
    renderTexture(x, y, (float)w * size, (float)h * size, window->getRenderer(), texture);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void NSurface::renderTexture(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture *texture) {
    SDL_Rect destination = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, nullptr, &destination);
}

void NSurface::renderTexture(Box destination, SDL_Rect src, SDL_Renderer* renderer, SDL_Texture* texture, SDL_RendererFlip flip, int angle, int a /* = 255 */) {
    if(!texture || !renderer)
        return;
    
    SDL_SetTextureAlphaMod(texture, a);
    SDL_Rect dest = destination;
    SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, nullptr, flip);
}

void NSurface::renderLine(Line line, SDL_Renderer* renderer, CCamera* camera /* = nullptr */) {
    if(camera) {
        line = line.fixCamera(camera);
//        if(!camera->collision(line.x, line.y, line.x2, line.y2))
//            return;
    }
    
    SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
    SDL_RenderDrawLine(renderer, line.x, line.y, line.x2, line.y2);
}

void NSurface::renderTriangle(Triangle triangle, SDL_Renderer* renderer, CCamera* camera /* = nullptr */) {
    if(camera)
        triangle = triangle.fixCamera(camera);
    
    renderLine(triangle.a, renderer);
    renderLine(triangle.b, renderer);
    renderLine(triangle.c, renderer);
}

void NSurface::renderPixel(int x, int y, SDL_Renderer* renderer, int r, int g, int b, int a /* = 255 */) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawPoint(renderer, x, y);
}

void NSurface::renderFilledTriangle(Position v1, Position v2, Position v3, Color color, SDL_Renderer* renderer, CCamera* camera /* = nullptr */) {
    if(camera) {
        int ox = camera->offsetX();
        int oy = camera->offsetY();
        
        v1.x -= ox;
        v2.x -= ox;
        v3.x -= ox;
        v1.y -= oy;
        v2.y -= oy;
        v3.y -= oy;
    }
    
    int maxX = max(v1.x, max(v2.x, v3.x));
    int minX = min(v1.x, min(v2.x, v3.x));
    int maxY = max(v1.y, max(v2.y, v3.y));
    int minY = min(v1.y, min(v2.y, v3.y));
    
    Position vs1(v2.x - v1.x, v2.y - v1.y);
    Position vs2(v3.x - v1.x, v3.y - v1.y);
    
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Position q(x - v1.x, y - v1.y);
            
            float s = (float)crossProduct(q, vs2) / crossProduct(vs1, vs2);
            float t = (float)crossProduct(vs1, q) / crossProduct(vs1, vs2);
            
            if ( (s >= 0) && (t >= 0) && (s + t <= 1)) {
                renderPixel(x, y, renderer, color.r, color.g, color.b);
            }
        }
    }
}


