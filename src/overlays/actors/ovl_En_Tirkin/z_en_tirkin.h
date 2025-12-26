#ifndef Z_EN_TIRKIN_H
#define Z_EN_TIRKIN_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_Tirkin;

typedef void (*En_TirkinActionFunc)(struct En_Tirkin*, PlayState*);

typedef struct En_Tirkin {
    Actor actor;
    En_TirkinActionFunc actionFunc;
    /* 0x0170 */ SkelAnime skelanime;
    Vec3s jointTable[TIRKIN_SKEL_NUM_LIMBS];
    Vec3s morphTable[TIRKIN_SKEL_NUM_LIMBS];
    short damagedTimer;
    short CooldownTimer;
    ColliderCylinder collider;
} En_Tirkin;

#endif
