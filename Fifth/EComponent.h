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

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Define.h"


class CInstance;
class CWindow;
class CCamera;
class CEntity;
class CGuiText;

struct CollisionSides;

class EComponent {
    
public:
    EComponent(CEntity* parent);
    virtual ~EComponent();
    
    virtual void onLoop(CInstance* instance) {  }
    virtual void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {  }
    virtual void onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {  }
    virtual bool onCollision(CEntity* target, CollisionSides* collisionSides) { return true; }
    
    virtual void serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc) {  }
    virtual void deserialize(rapidjson::Value* value) {  }
    
    CEntity* parent;
    
    std::string type;
    
protected:
    
    std::vector<CGuiText*>* guiTextVector();
    
};

#endif /* defined(__Fifth__EComponent__) */
