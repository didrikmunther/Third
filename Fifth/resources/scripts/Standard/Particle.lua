-- Particle.lua

local Particle = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.startTime = game:getTime()
        self.livingTime = -1
    end
)

function Particle:onLoop()
    if(self.startTime + self.livingTime * 1000 < game:getTime()) then
        self.parent.toRemove = true
    end
end

function Particle:onDeserialize(value)
    decoded = json.decode(value)

    self.livingTime = getVal(self.livingTime, decoded.livingTime)
end

function create(parent, component)
    return Particle(parent, component)
end