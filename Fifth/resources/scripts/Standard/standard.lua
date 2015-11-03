-- standard.lua

CollisionLayers = {
    LAYER0      = 1,
    LAYER1      = 2,
    LAYER2      = 4,
    LAYER3      = 8,
    LAYER4      = 16,
    LAYER5      = 32,
    LAYER6      = 64,
    LAYER7      = 128
}

EntityProperty = {
    COLLIDABLE   = 1,
    GRAVITY_AFFECT = 2,
    HIDDEN       = 4,
    STATIC       = 8,
    FLIP         = 16,
    FLIP_FREEZED = 32
}

SpriteStateTypes = {
    IDLE        = 0,
    ASCENDING   = 1,
    DESCENDING  = 2,
    FLYING      = 3,
    WALKING     = 4,
    SNEAKING    = 5,
    RUNNING     = 6,
    DUCKING     = 7,
    TOTAL_SPRITESTATETYPES = 8
}

ChatBubbleType = {
    SAY = 0,
    YELL = 1,
    WHISPER = 2,
    INSTANT_TALK = 3
}

json = nil

function init(path)
    game:setLuaPath(path)
end

function requireModule(path, mod)
    oldPath = package.path
    package.path = package.path .. path
    toReturn = require(mod)
    package.path = oldPath
    return toReturn
end

function initJson()
_G.json = requireModule("json.lua", "json")
end

function getVal(var, val)
    return (val ~= nil and val or var)
end