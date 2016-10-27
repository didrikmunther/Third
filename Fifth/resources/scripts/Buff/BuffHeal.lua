-- BuffHeal.lua

local BuffHeal = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component
        self.isBuff = true
        -- self.commands = { healRangeTop, healRangeBottom, critChance, critPower }

        self.healRangeTop = 50
        self.healRangeBot = 40
        self.critChance = 0
        self.critPower = 0

        self.timer = game.getTime()
        self.delay = 50
	end
)

function BuffHeal:onInit()
    c = self.commands
    if(c[1]) then self.healRangeTop = c[1] end
    if(c[2]) then self.healRangeBot = c[2] end
    if(c[3]) then self.critChance = c[3] end
    if(c[4]) then self.critPower = c[4] end
end

function BuffHeal:onLoop(isFirstLoop, isLastLoop)
    if(self.parent.isDead) then do return end end
    living = self.parent:getComponent("Standard/Living")
    if(living == nil) then do return end end

    time = game.getTime()

    if(time - self.timer > self.delay) then
        self.timer = time
        living:heal(math.floor(math.random() * (self.healRangeTop - self.healRangeBot) + self.healRangeBot), self.parent)
    end
end

function create(parent, component)
	return BuffHeal(parent, component)
end