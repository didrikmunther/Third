-- SpellSpeed.lua

local SpellSpeed = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component
	end
)

function SpellSpeed:onLoop(isFirstLoop, isLastLoop, commands)
    movable = self.parent:getComponent("Standard/Movable")
    if(movable == nil) then do return end end

    if(isFirstLoop) then
    --    movable.speedMultiplier = 2
        movable.isFlying = true
    end

    if(isLastLoop) then
    --    movable.speedMultiplier = 1
        movable.isFlying = false
    end

end

function create(parent, component)
	return SpellSpeed(parent, component)
end