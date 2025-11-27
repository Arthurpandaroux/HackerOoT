#ifndef Z_EN_NOPON_H
#define Z_EN_NOPON_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"
#include "assets/objects/object_en_nopon/object_en_nopon.h"
struct En_Nopon;

typedef void (*En_NoponActionFunc)(struct En_Nopon*, PlayState*);

typedef struct En_Nopon {
    Actor actor;
    En_NoponActionFunc actionFunc;
    Vec3s jointTable[NOPON_SKEL_NUM_LIMBS];
    Vec3s morphTable[NOPON_SKEL_NUM_LIMBS];
    /* 0x0170 */ SkelAnime skelanime;
    short damagedTimer;
    ColliderCylinder collider;
    ColliderCylinder collider_Attack1;
    short attackCooldown;
    s16 Attackx;
    s16 Attacky;
    s16 Attackz;
} En_Nopon;

#endif

