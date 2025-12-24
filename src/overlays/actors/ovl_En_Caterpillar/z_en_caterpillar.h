#ifndef Z_EN_CATERPILLAR_H
#define Z_EN_CATERPILLAR_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_Caterpillar;

typedef void (*En_CaterpillarActionFunc)(struct En_Caterpillar*, PlayState*);

typedef struct En_Caterpillar {
    Actor actor;
    En_CaterpillarActionFunc actionFunc;
} En_Caterpillar;

#endif
