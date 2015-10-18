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