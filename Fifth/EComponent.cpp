//
//  EComponent.cpp
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EComponent.h"
#include "CEntity.h"


EComponent::EComponent(CEntity* parent, LuaScript* script)
    : parent(parent)
    , object(parent, script)
{
    
}

EComponent::~EComponent() {
    
}

void EComponent::onLoop(CInstance* instance) {
    if(!object.hasReference("onLoop"))
        return;
    
    object.beginCall("onLoop");
    luabridge::Stack<CEntity*>::push(object.getScript()->getState(), parent);
    object.endCall(1, 0);
}

void EComponent::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if(!object.hasReference("onRender"))
        return;
    
    object.beginCall("onRender");
    luabridge::Stack<CEntity*>::push(object.getScript()->getState(), parent);
    object.endCall(1, 0);
}

void EComponent::onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
}

bool EComponent::onCollision(CEntity* target, CollisionSides* collisionSides) {
    return true;
}

void EComponent::serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc) {
    
}

void EComponent::deserialize(rapidjson::Value* value) {
    
}

void EComponent::callSimpleFunction(std::string function) {
    if(!object.hasReference(function.c_str()))
        return;
    
    object.beginCall(function.c_str());
    luabridge::Stack<CEntity*>::push(object.getScript()->getState(), parent);
    object.endCall(1, 0);
}

void EComponent::pushThis() {
    object.pushObject();
}

std::vector<CGuiText*>* EComponent::guiTextVector() {
    return &parent->_GuiTextVector;
}