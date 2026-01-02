#ifndef Z_EN_SPIDER_TELETHIA_H
#define Z_EN_SPIDER_TELETHIA_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_Spider_Telethia;

typedef void (*En_Spider_TelethiaActionFunc)(struct En_Spider_Telethia*, PlayState*);

typedef struct En_Spider_Telethia {
    Actor actor;
    En_Spider_TelethiaActionFunc actionFunc;
} En_Spider_Telethia;

#endif
