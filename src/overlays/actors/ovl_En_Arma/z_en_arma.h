#ifndef Z_EN_ARMA_H
#define Z_EN_ARMA_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_Arma;

typedef void (*En_ArmaActionFunc)(struct En_Arma*, PlayState*);

typedef struct En_Arma {
    Actor actor;
    En_ArmaActionFunc actionFunc;
} En_Arma;

#endif
