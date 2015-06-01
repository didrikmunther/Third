//
//  CEntityManager.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntityManager__
#define __Third__CEntityManager__

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <memory>
#include <vector>
#include "CEntity.h"
#include "CParticle.h"
#include "CParticleEmitter.h"
#include "CSprite.h"
#include <map>
#include <string>
#include "CAssetManager.h"
#include "CGuiText.h"

class CCamera;

class CEntityManager {
    
public:
    CEntityManager();
    
    CEntity* addEntity(sf::IntRect rect, sf::Color color, std::string name = "");
    CEntity* addEntity(sf::IntRect rect, std::string spriteKey, std::string name = "");
    CEntity* getEntity(std::string name);
    CEntity* getEntityAtCoordinate(int x, int y);
    std::string getNameOfEntity(CEntity* entity);
    void addEntity(CEntity* entity, std::string name = "");
    void addParticle(sf::IntRect rect, sf::Color color, int livingTime);
    void addParticle(CParticle* particle);
    void addParticleEmitter(sf::IntRect rect, sf::Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, float velocity);
    void addGuiText(CGuiText* guiText);
    
    void onRender(CWindow* window, CCamera* camera);
    void onLoop();
    void onCleanup();
    
    void entityCleanup();
    void particleEmitterCleanup();
    void particleCleanup();
    void guiTextCleanup();
    
    int entityID;
    
    int renderFlags;
    void addRenderFlag(int renderFlag);
    void removeRenderFlag(int renderFlag);
    void toggleRenderFlag(int renderFlag);
    
private:
    std::map<std::string, CEntity*> _EntityVector;
    std::vector<CParticle*> _ParticleVector;
    std::vector<CParticleEmitter*> _ParticleEmitterVector;
    std::vector<CGuiText*> _GuiTextVector;
    
};

#endif /* defined(__Third__CEntityManager__) */
