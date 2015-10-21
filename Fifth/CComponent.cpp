//
//  CComponent.cpp
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CComponent.h"
#include "CEntity.h"


CComponent::CComponent(CEntity* parent, CLuaScript* script)
    : parent(parent)
    , object(parent, this, script)
{
    
}

CComponent::~CComponent() {
    
}

void CComponent::onLoop(CInstance* instance) {
    if(!object.hasReference("onLoop"))
        return;
    
    object.beginCall("onLoop");
    //luabridge::Stack<CEntity*>::push(object.getScript()->getState(), parent);
    object.endCall(0, 0);
}

void CComponent::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if(!object.hasReference("onRender"))
        return;
    
    object.beginCall("onRender");
    //luabridge::Stack<CEntity*>::push(object.getScript()->getState(), parent);
    object.endCall(0, 0);
}

void CComponent::onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
}

bool CComponent::onCollision(CEntity* target, CollisionSides* collisionSides) {
    return true;
}

void CComponent::serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc) {
    
}

void CComponent::deserialize(rapidjson::Value* value) {
    
}

void CComponent::callSimpleFunction(std::string function) {
    if(!object.hasReference(function.c_str()))
        return;
    
    object.beginCall(function.c_str());
    luabridge::Stack<CEntity*>::push(object.getScript()->getState(), parent);
    object.endCall(1, 0);
}

void CComponent::pushThis() {
    object.pushObject();
}

std::vector<CGuiText*>* CComponent::guiTextVector() {
    return &parent->_GuiTextVector;
}