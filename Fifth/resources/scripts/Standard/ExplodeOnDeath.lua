-- ExplodeOnDeath.lua

local ExplodeOnDeath = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.hasExploded = false
    end
)

function ExplodeOnDeath:onLoop()
    if(self.parent.isDead and not self.hasExploded) then
        self.hasExploded = true
        self:explode()
    end
end

function ExplodeOnDeath:explode()
    if(not self.parent:hasSprite()) then do return end end

    sprite = self.parent:getSprite()
    source = sprite:getSource()
    spriteSheet = sprite:getSpriteSheet()
    parentName = self.parent.entityManager:getNameOfEntity(self.parent)
    entityManager = self.parent.entityManager
    box = self.parent.body.box
    parent = self.parent
    isFlipped = parent:hasProperty(EntityProperty.FLIP)

    thisX = box.x
    thisY = box.y
    thisW = box.w / 2
    thisH = box.h / 2

    self:explodeParticle(source.x, source.y, source.w / 2, source.h / 2, thisX, thisY, thisW, thisH, parentName .. "sprite1")
    self:explodeParticle(source.x + source.w / 2, source.y, source.w / 2, source.h / 2, thisX + thisW, thisY, thisW, thisH, parentName .. "sprite2")
    self:explodeParticle(source.x, source.y + source.h / 2, source.w / 2, source.h / 2, thisX, thisY + thisH, thisW, thisH, parentName .. "sprite3")
    self:explodeParticle(source.x + source.w / 2, source.y + source.h / 2, source.w / 2, source.h / 2, thisX + thisW, thisY + thisH, thisW, thisH, parentName .. "sprite4")
end

function ExplodeOnDeath:explodeParticle(sx, sy, sw, sh, tx, ty, tw, th, spriteName)
    entityManager = self.parent.entityManager
    sprite = self.parent:getSprite()
    spriteSheet = sprite:getSpriteSheet()

    sBox = Box(sx, sy, sw, sh)

    sprite = game.createSprite(spriteSheet, sBox)
    game.addSprite(sprite, spriteName)

    eBox = Box(tx, ty, tw, th)
    particle = entityManager:createSpriteEntity(eBox, spriteName)

    --if(parent:hasProperty(EntityProperty.FLIP)) then
    --   particle:addProperty(EntityProperty.FLIP)
    --end

    force = 20

    body = particle.body
    body.velX = self.parent.body.velX + math.random(force) - force / 2
    body.velY = self.parent.body.velY + -10 + -math.random(force) / 2

    particle:addComponent(self.component.instance, game.getScript("Standard/Particle"))
    particle:getComponent("Standard/Particle"):onDeserialize('{"livingTime":5}')

    if(not self.parent:hasProperty(EntityProperty.COLLIDABLE)) then
        particle:removeProperty(EntityProperty.COLLIDABLE)
    end

    entityManager:addParticle(particle, self.component.instance)

end

function create(parent, component)
    return ExplodeOnDeath(parent, component)
end