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
    

void ArtManager_DrawUI(ArtManager* mgr, PlayState* play) {
     u32 curColorSet;
    f32 offsetX;
    f32 offsetY;
    f32 offsetX2;
    f32 offsetY2;
    f32 offsetX3;
    f32 offsetY3;
    f32 offsetX4;
    f32 offsetY4;
    f32 halfHeartLength;
    f32 heartCenterX;
    f32 heartCenterY;
    f32 heartCenterX2;
    f32 heartCenterY2;
    f32 heartCenterX3;
    f32 heartCenterY3;
    f32 heartCenterX4;
    f32 heartCenterY4;
    f32 heartTexCoordPerPixel;
    offsetY = 151.0f;
    offsetX = 130.0f;
    offsetX2 = 150.0f;
    offsetY2 = 171.0f;
    offsetX3 = 130.0f;
    offsetY3= 191.0f;
    offsetX4 = 110.0f;
    offsetY4 = 171.0f;
   heartCenterY = 28.0f + offsetY;
            heartCenterX = 32.0f + offsetX;
            heartCenterY2 = 28.0f + offsetY2;
            heartCenterX2 = 32.0f + offsetX2;
            heartCenterY3 = 28.0f + offsetY3;
            heartCenterX3 = 32.0f + offsetX3;
            heartCenterY4 = 28.0f + offsetY4;
            heartCenterX4 = 32.0f + offsetX4;
            heartTexCoordPerPixel = 1.19f;
            heartTexCoordPerPixel /= 0.75f;
            heartTexCoordPerPixel *= 1 << 10;
            halfHeartLength = 16.0f;
            halfHeartLength *= 0.64f;

     OPEN_DISPS(play->state.gfxCtx, "../art_manager.c", 3079);
    gDPPipeSync(OVERLAY_DISP++);
Gfx_SetupDL_39Overlay(play->state.gfxCtx);

// raw texture, no tint
gDPSetCombineMode(OVERLAY_DISP++, G_CC_DECALRGBA, G_CC_DECALRGBA);

// proper render mode for alpha-blended UI
gDPSetRenderMode(OVERLAY_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

// Load your 32×32 RGBA texture
gDPLoadTextureBlock(OVERLAY_DISP++, gCarrotIconTex,
    G_IM_FMT_RGBA, G_IM_SIZ_32b,
    32, 32, 0,
    G_TX_CLAMP, G_TX_CLAMP,
    G_TX_NOMASK, G_TX_NOMASK,
    G_TX_NOLOD, G_TX_NOLOD
);
     gSPTextureRectangle(OVERLAY_DISP++, WIDE_DIV(((heartCenterX - halfHeartLength) * 4), WIDE_GET_4_3),
                                (s32)((heartCenterY - halfHeartLength) * 4),
                                WIDE_DIV(((heartCenterX + halfHeartLength) * 4), WIDE_GET_RATIO),
                                (s32)((heartCenterY + halfHeartLength) * 4), G_TX_RENDERTILE, 0, 0,
                                WIDE_DIV(heartTexCoordPerPixel, WIDE_GET_RATIO), (s32)heartTexCoordPerPixel);

gDPLoadTextureBlock(OVERLAY_DISP++, gCarrotIconTex,
    G_IM_FMT_RGBA, G_IM_SIZ_32b,
    32, 32, 0,
    G_TX_CLAMP, G_TX_CLAMP,
    G_TX_NOMASK, G_TX_NOMASK,
    G_TX_NOLOD, G_TX_NOLOD
);
     gSPTextureRectangle(OVERLAY_DISP++, WIDE_DIV(((heartCenterX2 - halfHeartLength) * 4), WIDE_GET_4_3),
                                (s32)((heartCenterY2 - halfHeartLength) * 4),
                                WIDE_DIV(((heartCenterX2 + halfHeartLength) * 4), WIDE_GET_RATIO),
                                (s32)((heartCenterY2 + halfHeartLength) * 4), G_TX_RENDERTILE, 0, 0,
                                WIDE_DIV(heartTexCoordPerPixel, WIDE_GET_RATIO), (s32)heartTexCoordPerPixel);

gDPLoadTextureBlock(OVERLAY_DISP++, gCarrotIconTex,
    G_IM_FMT_RGBA, G_IM_SIZ_32b,
    32, 32, 0,
    G_TX_CLAMP, G_TX_CLAMP,
    G_TX_NOMASK, G_TX_NOMASK,
    G_TX_NOLOD, G_TX_NOLOD
);
     gSPTextureRectangle(OVERLAY_DISP++, WIDE_DIV(((heartCenterX3 - halfHeartLength) * 4), WIDE_GET_4_3),
                                (s32)((heartCenterY3 - halfHeartLength) * 4),
                                WIDE_DIV(((heartCenterX3 + halfHeartLength) * 4), WIDE_GET_RATIO),
                                (s32)((heartCenterY3 + halfHeartLength) * 4), G_TX_RENDERTILE, 0, 0,
                                WIDE_DIV(heartTexCoordPerPixel, WIDE_GET_RATIO), (s32)heartTexCoordPerPixel);

gDPLoadTextureBlock(OVERLAY_DISP++, gCarrotIconTex,
    G_IM_FMT_RGBA, G_IM_SIZ_32b,
    32, 32, 0,
    G_TX_CLAMP, G_TX_CLAMP,
    G_TX_NOMASK, G_TX_NOMASK,
    G_TX_NOLOD, G_TX_NOLOD
);
     gSPTextureRectangle(OVERLAY_DISP++, WIDE_DIV(((heartCenterX4 - halfHeartLength) * 4), WIDE_GET_4_3),
                                (s32)((heartCenterY4 - halfHeartLength) * 4),
                                WIDE_DIV(((heartCenterX4 + halfHeartLength) * 4), WIDE_GET_RATIO),
                                (s32)((heartCenterY4 + halfHeartLength) * 4), G_TX_RENDERTILE, 0, 0,
                                WIDE_DIV(heartTexCoordPerPixel, WIDE_GET_RATIO), (s32)heartTexCoordPerPixel);
    

    CLOSE_DISPS(play->state.gfxCtx, "../art_manager.c", 3094);
}