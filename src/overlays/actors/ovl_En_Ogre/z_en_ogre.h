#ifndef Z_EN_OGRE_H
#define Z_EN_OGRE_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

struct En_Ogre;

typedef void (*En_OgreActionFunc)(struct En_Ogre*, PlayState*);

typedef struct En_Ogre {
    Actor actor;
    En_OgreActionFunc actionFunc;
} En_Ogre;

#endif
