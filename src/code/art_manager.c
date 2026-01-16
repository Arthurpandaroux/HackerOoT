#include "src/code/art_manager.h"
#include "play_state.h"
#include "player.h"
#include "assets/textures/parameter_static/parameter_static.h"
#include "controller.h"
#include "zelda_arena.h"
#include "ultra64.h"
#include "sfx.h"
#include "gfx.h"
#include "gfx_setupdl.h"
#include "widescreen.h"
#include "regs.h"
#include "ultra64/gbi.f3dex3.h"
#include "message.h"


#include "assets/textures/parameter_static/parameter_static.h"


void ArtManager_Test(struct Player* this, PlayState* play) {
    Player_PlaySfx(this, NA_SE_IT_SWORD_SWING);
}
void ArtManager_Init(ArtManager* mgr) {
    // Clear all actions
     mgr->arts[0].id = ART_NONE;
     mgr->arts[1].id = ART_BACKSLASH;
     mgr->arts[2].id = ART_NONE;
     mgr->arts[3].id = ART_NONE;
    for (int i = 0; i < 4; i++) {
        if (mgr->arts[i].id == ART_BACKSLASH) {
            mgr->arts[i].Execute = ArtManager_Test;
        } 
        else if (mgr->arts[i].id == ART_NONE) {
            mgr->arts[i].Execute = NULL;
        } 
        mgr->arts[i].cooldownFrames = 0;
        mgr->arts[i].name = NULL;
        mgr->arts[i].iconTex = 0;
    }

    mgr->selectedIndex = -1;
    mgr->uiActive = 0;
    mgr->prevButtons = 0;
}

 void ArtManager_Update(ArtManager* mgr, PlayState* play, Player* this, Input* input) {
    mgr->selectedIndex = -1;

     // Update cooldowns

    if (input->press.button & BTN_DUP) {
        mgr->selectedIndex = 0;
    } else if (input->press.button & BTN_DRIGHT) {
        mgr->selectedIndex = 1;
    } else if (input->press.button & BTN_DDOWN) {
        mgr->selectedIndex = 2;
    } else if (input->press.button & BTN_DLEFT) {
        mgr->selectedIndex = 3;
    }

    if (mgr->selectedIndex < 0 || mgr->selectedIndex > 3) {
        return;
    }

    ArtAction* art = &mgr->arts[mgr->selectedIndex];

    if (art->Execute != NULL) {
        art->Execute(this, play);
        mgr->selectedIndex = -1;
    }
};

static const Vec2f sArtIconPos[4] = {
    { 162.0f, 179.0f },
    { 182.0f, 199.0f },
    { 162.0f, 219.0f },
    { 142.0f, 199.0f },
};


static void Art_DrawIcon(
    Gfx** p,
    ArtAction* art,
    f32 centerX,
    f32 centerY,
    f32 halfSize
) {
    u8 tint;

    // Cooldown tint
    if (art->cooldownMax > 0 && art->cooldownFrames > 0) {
        tint = 120;
    } else {
        tint = 255;
    }

    gDPPipeSync((*p)++);
    gDPSetCombineMode((*p)++, G_CC_DECALRGBA, G_CC_DECALRGBA);

    // Load 32x32 RGBA32 icon
    gDPLoadTextureBlock(
        (*p)++,
        gAlvisMindBlast,
        G_IM_FMT_RGBA,
        G_IM_SIZ_32b,
        32, 32,
        0,
        G_TX_CLAMP, G_TX_CLAMP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );

    // Draw scaled rectangle (THIS controls size)
    gSPTextureRectangle(
        (*p)++,
        (s32)((centerX - halfSize) * 4),
        (s32)((centerY - halfSize) * 4),
        (s32)((centerX + halfSize) * 4),
        (s32)((centerY + halfSize) * 4),
        G_TX_RENDERTILE,
        0, 0,
        32 << 5,   // texture width in 16.5 fixed-point
        32 << 5    // texture height in 16.5 fixed-point
    );
}

void ArtManager_DrawUI(ArtManager* mgr, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (player->heldItemAction == PLAYER_IA_NONE) {
        return;
    }

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    Gfx_SetupDL_39Overlay(play->state.gfxCtx);

    Gfx* overlayP = play->state.gfxCtx->overlay.p;

    // Make icons bigger here
    f32 halfSize = 32.0f; // 64x64 icons on screen

    for (s32 i = 0; i < 4; i++) {
        ArtAction* art = &mgr->arts[i];

        Art_DrawIcon(
            &overlayP,
            art,
            sArtIconPos[i].x,
            sArtIconPos[i].y,
            halfSize
        );
    }

    play->state.gfxCtx->overlay.p = overlayP;
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}