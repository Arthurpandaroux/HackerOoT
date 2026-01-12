#ifndef ART_MANAGER_H
#define ART_MANAGER_H

#include "ultra64.h"
#include "play_state.h"
#include "actor.h"
#include "controller.h"
#include "ultra64.h"


    struct Player;   // forward declare
typedef struct Player Player;

typedef void (*ArtExecuteFunc)(Player* this, PlayState* play);

typedef struct {
    u32 id;
    char* name;

    u16 iconTex;

    s16 cooldownFrames;
    s16 cooldownMax;
    ArtExecuteFunc Execute;

} ArtAction;

typedef struct {

    ArtAction arts[4];
    s32 selectedIndex;
    u8 uiActive;

    // Needed to avoid repeated activation every frame
    u16 prevButtons;

} ArtManager;

typedef enum Arts {
    ART_NONE,
    ART_BACKSLASH,
    ART_BREAKSLASH
} Arts;

void ArtManager_Init(ArtManager* mgr);
void ArtManager_Test(Player* this, PlayState* play);
void ArtManager_Update(ArtManager* mgr, PlayState* play, struct Player* this, Input* input);
void ArtManager_DrawUI(ArtManager* mgr, PlayState* play);
void Player_StartRoll(Player* this, PlayState* play);
#endif


