-- Projectile.lua

local Projectile = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.livingTime = 5 -- seconds
        self.startTime = game:getTime()

        self.owner = nil

    end
)

function Projectile:onLoop()
    if(self.startTime + self.livingTime * 1000 < game:getTime()) then
        self.parent.toRemove = true
    end
end

function Projectile:onCollision(target, collisionSides)
    --if(self.owner == nil) then do return end

    if(self.owner:compare(target) or self.parent.isDead) then
        return false
    end

    targetLiving = target:getComponent("Standard/Living")
    if(targetLiving ~= nil) then
        targetLiving:damage(100, self.parent)
    end

    self.parent.isDead = true
    return true
end

function Projectile:onDeserialize(value)
    decoded = json.decode(value)

    if(decoded.owner ~= nil) then
        self.owner = getVal(self.owner, self.parent.entityManager:getEntity(decoded.owner))
    end
end

function create(parent, component)
    return Projectile(parent, component)
end