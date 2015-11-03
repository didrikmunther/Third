//
//  CComponent.cpp
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CComponent.h"
#include "CEntity.h"
#include "NMouse.h"


CComponent::CComponent(CEntity* parent, CLuaScript* script)
    : parent(parent)
    , object(parent, this, script)
    , tempWindow(nullptr)
    , tempCamera(nullptr)
    , tempRenderflags(nullptr)
{
    callSimpleFunction("onInit");
}

CComponent::~CComponent() {
    
}

void CComponent::onLoop(CInstance* instance) {
    if(!object.hasReference("onLoop"))
        return;
    
    tempInstance = instance;
    
    object.beginCall("onLoop");
    object.endCall(0, 0);
}

void CComponent::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if(!object.hasReference("onRender"))
        return;
    
    tempWindow = window;
    tempCamera = camera;
    tempRenderflags = &renderFlags;
    
    object.beginCall("onRender");
    object.endCall(0, 0);
    
    tempWindow = nullptr;
    tempCamera = nullptr;
    tempRenderflags = nullptr;
}

void CComponent::onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if(!object.hasReference("onRenderAdditional"))
        return;
    
    tempWindow = window;
    tempCamera = camera;
    tempRenderflags = &renderFlags;
    
    object.beginCall("onRenderAdditional");
    object.endCall(0, 0);
    
    tempWindow = nullptr;
    tempCamera = nullptr;
    tempRenderflags = nullptr;
    
}

bool CComponent::onCollision(CEntity* target, CollisionSides* collisionSides) {
    if(!object.hasReference("onCollision"))
        return true;
    
    object.beginCall("onCollision");
    object.pushObject(target);
    object.pushObject(collisionSides);
    object.endCall(2, 1);
    
    if(!lua_isboolean(object.getScript()->getState(), -1))
        return true;
    else
        return lua_toboolean(object.getScript()->getState(), -1);
    
}

void CComponent::onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc) {
    if(!object.hasReference("onSerialize"))
        return;
}

void CComponent::onDeserialize(rapidjson::Value* value) {
    onDeserialize(value->GetString());
}

void CComponent::onDeserialize(std::string value) {
    if(!object.hasReference("onDeserialize"))
        return;
    
    object.beginCall("onDeserialize");
    object.pushObject(value);
    object.endCall(1, 0);
}

void CComponent::callSimpleFunction(std::string function) {
    if(!object.hasReference(function.c_str()))
        return;
    
    object.beginCall(function.c_str());
    object.endCall(0, 0);
}

void CComponent::pushThis() {
    object.pushThisObject();
}

void CComponent::renderRect(int x, int y, int w, int h, int r, int g, int b, int a) {
    if(tempCamera == nullptr || tempWindow == nullptr)
        return;
    
    NSurface::renderRect(x - tempCamera->offsetX(), y - tempCamera->offsetY(), w, h, tempWindow, r, g, b, a);
}

void CComponent::renderLine(int x, int y, int x2, int y2, int r, int g, int b, int a) {
    if(tempCamera == nullptr || tempWindow == nullptr)
        return;
    
    NSurface::renderLine(Line(x, y, x2, y2, Color(r, g, b, a)), tempWindow->getRenderer(), tempCamera);
}

int CComponent::getRelativeMouse(lua_State* L) {
    int x, y;
    if(tempInstance != nullptr) {
        x = NMouse::relativeMouseX(&tempInstance->camera);
        y = NMouse::relativeMouseY(&tempInstance->camera);
    } else if(tempCamera != nullptr) {
        x = NMouse::relativeMouseX(tempCamera);
        y = NMouse::relativeMouseY(tempCamera);
    } else {
        return 0;
    }
    
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    
    return 2;
}

int CComponent::getMouse(lua_State* L) {
    int x = NMouse::absoluteMouseX();
    int y = NMouse::absoluteMouseY();
    
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    
    return 2;
}