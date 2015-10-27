-- Bullet.lua

local Bullet = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.livingTime = 5 -- seconds
        self.startTime = game:getTime()

        self.owner = nil

    end
)

function Bullet:onLoop()
    if(self.startTime + self.livingTime * 1000 < game:getTime()) then
        self.parent.toRemove = true
    end
end

function Bullet:onCollision(target, collisionSides)
    if(self.owner:compare(target)) then
        --return false
    end

    targetLiving = target:getComponent("Standard/Living")
    if(targetLiving ~= nil) then
        targetLiving:damage(1, self.parent)
    end

    self.parent.isDead = true
    return true
end

function Bullet:setOwner(entity)
    self.owner = entity
end

function create(parent, component)
    return Bullet(parent, component)
end