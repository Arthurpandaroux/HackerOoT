#ifndef Z_EN_TIRKIN_H
#define Z_EN_TIRKIN_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

#include "assets/objects/object_tirkin/object_tirkin.h"

struct En_Tirkin;

typedef void (*En_TirkinActionFunc)(struct En_Tirkin*, PlayState*);

typedef struct En_Tirkin {
    Actor actor;
    En_TirkinActionFunc actionFunc;
    /* 0x0170 */ SkelAnime skelanime;
    Vec3s jointTable[TIRKIN_SKEL_NUM_LIMBS];
    Vec3s morphTable[TIRKIN_SKEL_NUM_LIMBS];
    short damagedTimer;
    short attackCooldown;
    short attackCooldownNormal;
    short attackCooldownFire;
        /* 0x022C */ Vec3f targetPos;
        f32 speedMod;
    ColliderCylinder collider;
    ColliderCylinder collider_fire;
} En_Tirkin;

#endif 
