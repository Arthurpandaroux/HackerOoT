#ifndef Z_EN_LACERATER_TELETHIA_H
#define Z_EN_LACERATER_TELETHIA_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_Lacerater_Telethia;

typedef void (*En_Lacerater_TelethiaActionFunc)(struct En_Lacerater_Telethia*, PlayState*);

typedef struct En_Lacerater_Telethia {
    Actor actor;
    En_Lacerater_TelethiaActionFunc actionFunc;
} En_Lacerater_Telethia;

#endif
