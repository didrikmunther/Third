//
//  CEntity.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntity__
#define __Third__CEntity__

#include <stdio.h>
#include <memory>
#include <map>
#include <vector>
#include <sstream>

#include "CRenderable.h"
#include "CChatBubble.h"
#include "CBody.h"

#include "EComponent.h"


class CParticle;
class CSpriteContainer;
class CGuiText;
class CCamera;
class CEntityManager;
class CWindow;
class CInstance;
class EUtility;

enum class BasicUtilities;

namespace CollisionLayers {
    enum CollisionLayers {
        LAYER0      = 1 << 0,   // 1
        LAYER1      = 1 << 1,   // 2
        LAYER2      = 1 << 2,   // 4
        LAYER3      = 1 << 3,   // 8
        LAYER4      = 1 << 4,   // 16
        LAYER5      = 1 << 5,   // 32
        LAYER6      = 1 << 6,   // 64
        LAYER7      = 1 << 7,   // 128
    };
}

namespace EntityProperty {
    enum EntityProperty {
        COLLIDABLE  = 1 << 0,
        FLYING      = 1 << 1,
        HIDDEN      = 1 << 2,
        STATIC      = 1 << 3,
        FLIP        = 1 << 4,
        FLIP_FREEZED= 1 << 5
    };
}
    
namespace SpriteStateTypes {
    enum SpriteStateTypes {
        IDLE = 0,
        ASCENDING,
        DESCENDING,
        FLYING,
        WALKING,
        SNEAKING,
        RUNNING,
        DUCKING,
        TOTAL_SPRITESTATETYPES
    };
}

struct GridCoordinates {
    int x, y;
};

struct CollisionSides {
    CollisionSides() { (*this) = false; }
    
    bool top,
         bottom,
         right,
         left;
    
    void operator=(bool b) {
        top    =
        bottom =
        right  =
        left   = b;
    }
    
    void operator|=(CollisionSides b) {
        top    |= b.top;
        bottom |= b.bottom;
        right  |= b.right;
        left   |= b.left;
    }
};

class CEntity : public CRenderable {
    
friend class EComponent;
    friend class CEntityManager;
    
public:
    CEntity(Box rect, SDL_Color color);
    CEntity(Box rect, std::string spriteContainerKey);
    ~CEntity();
    
    void init();
    void onLoop(CInstance* instance);
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
    void serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc);
    void deserialize(rapidjson::Value* value);
    
    int collisionLayer;
    bool isOnCollisionLayer(int collisionLayer);
    void toggleCollisionLayer(int collisionLayer) { this->collisionLayer ^= collisionLayer; }
    void addCollisionLayer(int collisionLayer) { this->collisionLayer |= collisionLayer; }
    void removeCollisionLayer(int collisionLayer) { if(isOnCollisionLayer(collisionLayer)) toggleCollisionLayer(collisionLayer); }
    
    int properties;
    bool hasProperty(int property);
    void toggleProperty(int property);
    void addProperty(int property);
    void removeProperty(int property);
    
    void move(std::vector<CEntity*>* entities);
    bool coordinateCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);
    bool coordinateCollision(int x, int y, int w, int h);
    std::vector<GridCoordinates> gridCoordinates;
    
    CBody body;
    CollisionSides collisionSides;
    
    void say(std::string text, std::string fontKey, ChatBubbleType type);
    
    std::string spriteStateTypes[SpriteStateTypes::TOTAL_SPRITESTATETYPES];
    
    void setSpriteContainer(std::string spriteContainerKey);
    CSpriteContainer* getSpriteContainer();
    std::string getSpriteContainerKey();
    std::string spriteContainerKey;
    bool hasSprite();
    SDL_Color color;
    
    bool hasMoved() { return _hasMoved; }
    
    void shoot(float angle, BasicUtilities basicUtility);
    std::vector<CEntity*> particlesToAdd;
    
    template<typename T, typename ... A>
    void addComponent(A && ... args) {
        
        std::string type = _getType<T>();
        
        if(components.count(type) != 0)
            delete components[type];
        
        T* component = new T(this, std::forward<A>(args) ...);
        component->type = type;
        components[type] = component;
    }
    
    template<typename T>
    void addComponent(T* component) {
        
        std::string type = _getType<T>();
        component->type = type;
        
        if(components.count(type) != 0)
            delete components[type];
        
        components[type] = component;
        
    }
    
    template<typename T>
    T* getComponent() {
        
        std::string type = _getType<T>();
        
        if(components.count(type))
            return static_cast<T*>(components[type]);
        else
            return nullptr;
    }
    
    EComponent* getComponent(std::string type) {
        
        if(components.count(type))
            return components[type];
        else
            return nullptr;
    }
    
    bool isDead;
    bool toRemove;
    
protected:
    std::vector<CGuiText*> _GuiTextVector;
    void _cleanUpTextVector();
    void _cleanUpComponents();
    
    bool _onCollision(CEntity* target, CollisionSides* collisionSides);
    
    std::map<std::string, EComponent*> components;
    
private:
    bool _collision(int x, int y, std::vector<CEntity*>* entities);
    bool _hasMoved;
    
    template<typename T>
    std::string _getType() {
        std::stringstream type;
        type << typeid(T).name();
        return type.str();
    }
    
};

#endif /* defined(__Third__CEntity__) */
