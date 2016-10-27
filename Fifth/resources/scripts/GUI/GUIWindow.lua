-- GUIWindow.lua

local GUIWindow = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component
                        
        self.guiController = nil

        self.skin = "StandardGUI"
        self.x = 0
        self.y = 0
        self.w = 20
        self.h = 10
        self.size = 32
        self.offsetX = 0
        self.offsetY = 0
        
        self.showButtons = false
                        
        self.moveWindowArea = {}
        self.minimizeWindowArea = {}
        self.closeWindowArea = {}
                        
        self.isBeingMoved = false
        
        self.body = self.parent.body
        self.body.box = Box(0, 0, 0, 0)
        self.parent:removeProperty(EntityProperty.COLLIDABLE)
        self.parent:addProperty(EntityProperty.STATIC)
        --self.parent:addProperty(EntityProperty.HIDDEN)
                        
        self.topBordersTemp =       {2,   3,  4}
        self.rightBordersTemp =     {20, 27, 34}
        self.bottomBordersTemp =    {44, 45, 46}
        self.leftBordersTemp =      {14, 21, 28}
                        
        self.topBorders =       {}
        self.rightBorders =     {}
        self.bottomBorders =    {}
        self.leftBorders =      {}
                
        self.middlePiecesTemp = {16, 17, 18, 23, 24, 25, 30, 31, 32}
        self.middlePieces =     {}
    end
)

function GUIWindow:onInit()

end

function GUIWindow:onLoop()
    --self.body:setBox(self.component.camera:offsetX(), self.component.camera:offsetY(), (self.w + 4) * self.size, (self.h + 4) * self.size)

    size = self.size / 8
    right = (self.w - 3) * self.size

    self.moveWindowArea = {6 * size, 4 * size, (self.w + 1.70) * self.size, 4 * size}
    self.minimizeWindowArea = {44 * size + right, 5 * size, 4 * size, 4 * size}
    self.closeWindowArea = {48 * size + right, 5 * size, 3 * size, 4 * size}
    self.resizeWindowArea = {}

    if(self.isBeingMoved) then
        x, y = self.component:getMouse()
        self.x = x - self.offsetX
        self.y = y - self.offsetY
    end
end

function GUIWindow:renderBlock(x, y, block)
    self.component:renderSprite((x * self.size) + self.x, (y * self.size) + self.y, self.size, self.size, "GUI" .. self.skin .. block, 255, false, false)
end

function GUIWindow:addPieces(piece)
    biggest = self.w
    if(self.w < self.h) then biggest = self.h end
    while(table.getn(self[piece]) <= biggest) do
        table.insert(self[piece], self[piece .. "Temp"][math.random(table.getn(self[piece .. "Temp"]))])
    end
end

function GUIWindow:inArea(area, x, y)
    return (self.guiController:collides(self[area][1] + self.x, self[area][2] + self.y, self[area][3], self[area][4], x, y))
end

function GUIWindow:renderArea(area, r, b, g)
    self.component:renderRect(self[area][1] + self.x, self[area][2] + self.y, self[area][3], self[area][4], r, g, b, 255)
end

function GUIWindow:onClick(x, y)
    if(self:inArea("closeWindowArea", x, y)) then
        self.guiController:closeWindow(self)
        return
    end

    if(self.isBeingMoved) then
        self.isBeingMoved = false
        return
    elseif(self:inArea("moveWindowArea", x, y)) then
        self.isBeingMoved = true
        self.offsetX = x - self.x
        self.offsetY = y - self.y
    end
end

function GUIWindow:onRender()
    while(table.getn(self.middlePieces) <= ((self.w + 1) * (self.h + 1) + 1) * 2) do
        table.insert(self.middlePieces, self.middlePiecesTemp[math.random(table.getn(self.middlePiecesTemp))])
    end

    self:addPieces("topBorders")
    self:addPieces("bottomBorders")
    self:addPieces("rightBorders")
    self:addPieces("leftBorders")

    for y = 1, self.h + 2 do
        for x = 1, self.w + 2 do
            --print(self.w*(y-1)+(x-1) + 1)
            self:renderBlock(x, y, self.middlePieces[self.w * (y-1)+(x-1) + 1])
        end
    end

    self:renderBlock(0, 0, 0)
    self:renderBlock(1, 0, 1)
    self:renderBlock(0, 1, 7)
    self:renderBlock(1, 1, 8) -- right top corner

    self:renderBlock(self.w + 2, 0, 5)
    self:renderBlock(self.w + 3, 0, 6)
    self:renderBlock(self.w + 2, 1, 12)
    self:renderBlock(self.w + 3, 1, 13) -- left top corner

    self:renderBlock(0, self.h + 2, 35)
    self:renderBlock(1, self.h + 2, 36)
    self:renderBlock(0, self.h + 3, 42)
    self:renderBlock(1, self.h + 3, 43) -- right bottom corners

    self:renderBlock(self.w + 2, self.h + 2, 40)
    self:renderBlock(self.w + 3, self.h + 2, 41)
    self:renderBlock(self.w + 2, self.h + 3, 47)
    self:renderBlock(self.w + 3, self.h + 3, 48) -- left bottom corners

    for x = 1, self.w do
        self:renderBlock(x + 1, 0, self.topBorders[x])
        self:renderBlock(x + 1, self.h + 3, self.bottomBorders[x])
    end
    
    for y = 1, self.h do
        self:renderBlock(0, y + 1, self.leftBorders[y])
        self:renderBlock(self.w + 3, y + 1, self.rightBorders[y])
    end
    
    if(self.showButtons) then
        self:renderArea("closeWindowArea", 255, 0, 0)
        self:renderArea("moveWindowArea", 255, 0, 255)
        self:renderArea("minimizeWindowArea", 0, 0, 255)
    end
end

function create(parent, component)
    return GUIWindow(parent, component)
end