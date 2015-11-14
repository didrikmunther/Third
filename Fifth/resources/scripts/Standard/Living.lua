-- Living.lua

local Living = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.hasDied = false

        self.VALUE_HEALTH = 0
        self.VALUE_KEVLAR = 1

        self.maxValues = {}
        self.maxValues[self.VALUE_HEALTH] = 1000
        self.maxValues[self.VALUE_KEVLAR] = 1000

        self.values = {}
        self.values[self.VALUE_HEALTH] = self.maxValues[self.VALUE_HEALTH]
        self.values[self.VALUE_KEVLAR] = self.maxValues[self.VALUE_KEVLAR]

        -- temp
        self.values[self.VALUE_HEALTH] = 750
        self.values[self.VALUE_KEVLAR] = 500

    end
)

function Living:onRenderAdditional()

    if(self.parent.isDead) then
        do return end
    end

    camera = self.component.camera
    cameraX = camera:offsetX()
    cameraY = camera:offsetY()

    box = self.parent.body.box
    thisX = box.x - cameraX
    thisY = box.y - cameraY
    thisW = box.w
    thisH = box.h

    floatOverHead = 10
    bgWidth = 100
    bgHeight = 15
    healthWidth = (self.values[self.VALUE_HEALTH] / self.maxValues[self.VALUE_HEALTH]) * bgWidth
    kevlarWidth = (self.values[self.VALUE_KEVLAR] / self.maxValues[self.VALUE_KEVLAR]) * bgWidth

    self.component:renderRect(thisX + thisW / 2 - bgWidth / 2, thisY - bgHeight - floatOverHead, bgWidth, bgHeight, 255, 0, 0, 255) -- background
    self.component:renderRect(thisX + thisW / 2 - bgWidth / 2, thisY - bgHeight - floatOverHead, healthWidth, bgHeight, 0, 255, 0, 255) -- health
    self.component:renderRect(thisX + thisW / 2 - bgWidth / 2, thisY - bgHeight - floatOverHead, kevlarWidth, bgHeight / 2, 128, 128, 128, 255) -- health

end

function Living:onLoop()
    if(self.parent.isDead and not self.hasDied and self.parent:hasSprite()) then

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

    self.hasDied = true

    end
end

function Living:onSerialize()
    c = self.component

    c:addInt("health", self.values[self.VALUE_HEALTH])
    c:addInt("kevlar", self.values[self.VALUE_KEVLAR])
end

function Living:onDeserialize(value)
    d = json.decode(value)

    self.values[self.VALUE_HEALTH] = getVal(self.values[self.VALUE_HEALTH], d.health)
    self.values[self.VALUE_KEVLAR] = getVal(self.values[self.VALUE_KEVLAR], d.kevlar)
end

function Living:explodeParticle(sx, sy, sw, sh, tx, ty, tw, th, spriteName)
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

    entityManager:addParticle(particle, self.component.instance)

end

function Living:damage(amount, damager)

    box = self.parent.body.box
    thisX = box.x
    thisY = box.y

    afterKevlar = 0
    self.values[self.VALUE_KEVLAR] = self.values[self.VALUE_KEVLAR] - amount
    if(self.values[self.VALUE_KEVLAR] <= 0) then
        afterKevlar = -self.values[self.VALUE_KEVLAR]
        self.values[self.VALUE_KEVLAR] = 0
    end

    overDamage = 0 -- - 1
    if(self.values[self.VALUE_HEALTH] - afterKevlar <= 0) then
        overDamage = amount - (self.values[self.VALUE_HEALTH] - afterKevlar)
    end

    damageDone = 0
    if(amount - overDamage > 0) then
        damageDone = amount - overDamage
    end

    self.values[self.VALUE_HEALTH] = self.values[self.VALUE_HEALTH] - afterKevlar
    if(self.values[self.VALUE_HEALTH] <= 0) then
        self.values[self.VALUE_HEALTH] = 0
        self.parent.isDead = true

        self.parent:say("Killed by: " .. self.parent.entityManager:getNameOfEntity(damager:getComponent("Standard/Projectile").owner), "TESTFONT", ChatBubbleType.SAY)
    end

    damageColor = Color(255, 0, 0, 255)
    textObject = CombatText(thisX, thisY, damageColor, 20, "-" .. damageDone, "TESTFONT")
    self.parent:addCombatText(textObject)

    return damageDone

end

function Living:heal(amount, healer)

    box = self.parent.body.box
    thisX = box.x
    thisY = box.y

    self.values[self.VALUE_HEALTH] = self.values[self.VALUE_HEALTH] + amount

    overHeal = 0
    if(self.values[self.VALUE_HEALTH] >= self.maxValues[self.VALUE_HEALTH]) then
        overHeal = self.values[self.VALUE_HEALTH] - self.maxValues[self.VALUE_HEALTH]
        self.values[self.VALUE_HEALTH] = self.maxValues[self.VALUE_HEALTH]
    end
    healed = amount - overHeal

    healingColor = Color(0, 255, 0, 255)
    textObject = CombatText(thisX, thisY - 100, healingColor, 20, "+" .. healed, "TESTFONT")
    self.parent:addCombatText(textObject)

    return healed
end

function Living:onCollision(target, sides)
    if(self.parent.body.velX > 10) then

    end
end

function create(parent, component)
    return Living(parent, component)
end