-- SpellHeal.lua

local SpellHeal = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component

        self.timer = game.getTime()
        self.delay = 50
        self.healRangeTop = 50
        self.healRangeBot = 40
	end
)

function SpellHeal:onLoop(commands)
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
	return SpellHeal(parent, component)
end