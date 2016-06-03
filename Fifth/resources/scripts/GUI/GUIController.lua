-- GUIController.lua

local GUIController = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component
	end
)

function create(parent, component)
	return GUIController(parent, component)
end