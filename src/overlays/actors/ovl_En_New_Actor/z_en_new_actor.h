#ifndef Z_EN_NEW_ACTOR_H
#define Z_EN_NEW_ACTOR_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_New_Actor;

typedef void (*En_New_ActorActionFunc)(struct En_New_Actor*, PlayState*);

typedef struct En_New_Actor {
    Actor actor;
    En_New_ActorActionFunc actionFunc;
} En_New_Actor;

#endif
