-- Living.lua

local Living = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

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
    -- if not self.parent.isDead then self:damage(1, self.parent) end
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