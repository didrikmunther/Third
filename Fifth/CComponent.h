//
//  CComponent.h
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CComponent__
#define __Fifth__CComponent__

#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Define.h"
#include "CLuaObject.h"
#include "NSurface.h"


class CInstance;
class CWindow;
class CCamera;
class CEntity;
class CGuiText;

struct CollisionSides;

class CComponent {
    
public:
    CComponent(CEntity* parent, CLuaScript* script);
    ~CComponent();
    
    void onLoop(CInstance* instance);
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    void onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    bool onCollision(CEntity* target, CollisionSides* collisionSides);
    
    void serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc);
    void deserialize(rapidjson::Value* value);
    
    void callSimpleFunction(std::string function);
    
    void pushThis();
    
    CEntity* parent;
    CLuaObject object;
    
    CWindow* tempWindow;
    CCamera* tempCamera;
    RenderFlags* tempRenderflags;
    
    void renderRect(int x, int y, int w, int h, int r, int g, int b, int a); // These are only supposed to be called during an rendering process
    void renderLine(int x, int y, int x2, int y2, int r, int g, int b, int a);
    
//    void renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a = 255); // NSurface functions
//    void renderSprite(int x, int y, int w, int h, CSprite* sprite, CWindow* window, SDL_RendererFlip flip, int a = 255);
//    void renderText(int x, int y, CText* text, CWindow* window);
//    void renderTexture(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture *texture);
//    void renderLine(Line line, SDL_Renderer* renderer, CCamera* camera = nullptr);
//    void renderTriangle(Triangle triangle, SDL_Renderer* renderer, CCamera* camera = nullptr);
    
private:
    std::vector<CGuiText*>* guiTextVector();
    
};

#endif /* defined(__Fifth__CComponent__) */