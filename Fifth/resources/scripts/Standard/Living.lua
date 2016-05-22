-- Living.lua

local Living = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component
        self.require = {"Standard/ExplodeOnDeath"}

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
        -- /temp

        self.listeners = {}

    end
)

function Living:listenForHealth(component)
    table.insert(self.listeners, component)
end

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

        --self.parent:say("Killed by: " .. self.parent.entityManager:getNameOfEntity(damager:getComponent("Standard/Projectile").owner), "TESTFONT", ChatBubbleType.SAY)
    end

    damageColor = Color(255, 0, 0, 255)
    textObject = CombatText(thisX, thisY, damageColor, 20, "-" .. damageDone, "TESTFONT")
    self.parent:addCombatText(textObject)

    for k,v in pairs(self.listeners) do
        v:onLivingDamage(damageDone)
    end

    game.playSound("hurt", 0)

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

function create(parent, component)
    return Living(parent, component)
end