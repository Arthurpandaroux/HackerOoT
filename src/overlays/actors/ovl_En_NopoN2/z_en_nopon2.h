#ifndef Z_EN_NOPON2_H
#define Z_EN_NOPON2_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_NopoN2;

typedef void (*En_NopoN2ActionFunc)(struct En_NopoN2*, PlayState*);

typedef struct En_NopoN2 {
    Actor actor;
    En_NopoN2ActionFunc actionFunc;
} En_NopoN2;

#endif
