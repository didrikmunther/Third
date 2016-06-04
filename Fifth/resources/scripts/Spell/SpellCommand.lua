-- SpellCommand.lua

local SpellCommand = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component
	end
)

function SpellCommand:onLoop(isFirstLoop, isLastLoop, commands)
    if(not isFirstLoop or commands[1] == nil) then do return end end

    chatCtrl = self.component.instance.controller:getComponent("Standard/ChatController")
    if(not chatCtrl) then do return end end

    chatCtrl:parse("/" .. commands[1])
end

function create(parent, component)
	return SpellCommand(parent, component)
end