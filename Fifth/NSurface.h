//
//  NSurface.h
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__NSurface__
#define __Third__NSurface__

#include <stdio.h>
#include <SDL2/SDL.h>

#include "CBody.h"


class CWindow;
class CText;
class CSprite;
class CCamera;

struct Color {
    Uint8 r, g, b, a;
    
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    : r(r), g(g), b(b), a(a) {  }
    
    operator SDL_Color() const { return SDL_Color{r, g, b, a}; }
};

struct Line {
    int x, y;
    int x2, y2;
    Color color;
    
    Line(int x, int y, int x2, int y2, Color color = Color{255, 0, 0, 255})
    : x(x), y(y), x2(x2), y2(y2), color(color) {  }
    
    Line(Position pos1, Position pos2, Color color = Color{255, 0, 0, 255})
    : x(pos1.x), y(pos1.y), x2(pos2.x), y2(pos2.y), color(color) {  }
    
    Line fixCamera(CCamera* camera);
};

struct Triangle {
    Line a, b, c;
    
    Triangle(Line a, Line b, Line c)
    : a(a), b(b), c(c) {  }
    
    
    
    Triangle fixCamera(CCamera* camera);
};

namespace NSurface {

    void renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a = 255);
    void renderSprite(int x, int y, int w, int h, CSprite* sprite, CWindow* window, SDL_RendererFlip flip, int a = 255);
    void renderText(int x, int y, CText* text, CWindow* window, float size = 1);
    void renderTexture(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture *texture);
    void renderTexture(Box destination, SDL_Rect src, SDL_Renderer* renderer, SDL_Texture* texture, SDL_RendererFlip flip, int angle, int a = 255);
    void renderLine(Line line, SDL_Renderer* renderer, CCamera* camera = nullptr);
    void renderTriangle(Triangle triangle, SDL_Renderer* renderer, CCamera* camera = nullptr);
    void renderPixel(int x, int y, SDL_Renderer* renderer, int r, int g, int b, int a = 255);
    void renderFilledTriangle(Position v1, Position v2, Position v3, Color color, SDL_Renderer* renderer, CCamera* camera = nullptr);
    
    int crossProduct(Position v1, Position v2);
    
    template<typename T>
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
    template<typename T>
    T max(T a, T b) {
        return a > b ? a : b;
    }
    template<typename T>
    T min(T a, T b) {
        return a < b ? a : b;
    }
    
};


#endif /* defined(__Third__NSurface__) */
