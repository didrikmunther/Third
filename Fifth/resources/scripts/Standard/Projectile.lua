-- Projectile.lua

local Projectile = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.livingTime = 5 -- seconds
        self.startTime = game:getTime()

        self.owner = nil

        self.positions = {}
        self.positionsSize = 0

    end
)

function Projectile:onInit()
    body = self.parent.body
    thisX = body.box.x
    thisY = body.box.y
    mX, mY = self.component:getRelativeMouse()
    dX = mX - thisX
    dY = mY - thisY

    angle = math.atan2(dY, dX)
    precision = 20
    spread = 200
    angle = angle + (math.random(precision) - precision / 2) / spread

    velocity = 20
    velX = math.cos(angle) * velocity
    velY = math.sin(angle) * velocity

    body.velX = velX
    body.velY = velY

end

function Projectile:onLoop()
    if(self.startTime + self.livingTime * 1000 < game:getTime()) then
        self.parent.toRemove = true
        do return end
    end

    box = self.parent.body.box
    table.insert(self.positions, {box.x, box.y})
    self.positionsSize = self.positionsSize + 1

    if(self.positionsSize > 100) then
        self.positionsSize = self.positionsSize - 1
        table.remove(self.positions, 1)
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

function Projectile:onRenderAdditional()
    box = self.parent.body.box
    oldVal = self.positions[1]

    for k, v in pairs(self.positions) do
        if(oldVal[1] ~= v[1] or oldVal[2] ~= v[2]) then -- don't render if they are exactly the same positions
            self.component:renderLine(v[1], v[2], oldVal[1], oldVal[2], 255, 0, 0, 255)
        end
        oldVal = v
    end
end

function create(parent, component)
    return Projectile(parent, component)
end