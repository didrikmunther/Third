//
//  EComponent.h
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EComponent__
#define __Fifth__EComponent__

#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Define.h"
#include "LuaObject.h"


class CInstance;
class CWindow;
class CCamera;
class CEntity;
class CGuiText;

struct CollisionSides;

class EComponent {
    
public:
    EComponent(CEntity* parent, LuaScript* script);
    ~EComponent();
    
    void onLoop(CInstance* instance);
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    void onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    bool onCollision(CEntity* target, CollisionSides* collisionSides);
    
    void serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc);
    void deserialize(rapidjson::Value* value);
    
    void callSimpleFunction(std::string function);
    
    CEntity* parent;
    LuaObject object;
    
private:
    std::vector<CGuiText*>* guiTextVector();
    
};

#endif /* defined(__Fifth__EComponent__) */
