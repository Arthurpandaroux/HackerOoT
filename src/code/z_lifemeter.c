#include "gfx.h"
#include "gfx_setupdl.h"
#include "sfx.h"
#include "sys_matrix.h"
#include "z_lib.h"
#include "widescreen.h"
#include "lifemeter.h"
#include "play_state.h"
#include "player.h"
#include "save.h"
#include "debug.h"

#include "assets/textures/parameter_static/parameter_static.h"

static s16 sHeartsPrimColors[3][3] = {
    { HEARTS_PRIM_R, HEARTS_PRIM_G, HEARTS_PRIM_B },
    { HEARTS_BURN_PRIM_R, HEARTS_BURN_PRIM_G, HEARTS_BURN_PRIM_B },    // unused
    { HEARTS_DROWN_PRIM_R, HEARTS_DROWN_PRIM_G, HEARTS_DROWN_PRIM_B }, // unused
};

static s16 sHeartsEnvColors[3][3] = {
    { HEARTS_ENV_R, HEARTS_ENV_G, HEARTS_ENV_B },
    { HEARTS_BURN_ENV_R, HEARTS_BURN_ENV_G },                       // unused
    { HEARTS_DROWN_ENV_R, HEARTS_DROWN_ENV_G, HEARTS_DROWN_ENV_B }, // unused
};

static s16 sHeartsPrimFactors[3][3] = {
    {
        HEARTS_PRIM_R - HEARTS_PRIM_R,
        HEARTS_PRIM_G - HEARTS_PRIM_G,
        HEARTS_PRIM_B - HEARTS_PRIM_B,
    },
    // unused
    {
        HEARTS_BURN_PRIM_R - HEARTS_PRIM_R,
        HEARTS_BURN_PRIM_G - HEARTS_PRIM_G,
        HEARTS_BURN_PRIM_B - HEARTS_PRIM_B,
    },
    // unused
    {
        HEARTS_DROWN_PRIM_R - HEARTS_PRIM_R,
        HEARTS_DROWN_PRIM_G - HEARTS_PRIM_G,
        HEARTS_DROWN_PRIM_B - HEARTS_PRIM_B,
    },
};

static s16 sHeartsEnvFactors[3][3] = {
    {
        HEARTS_ENV_R - HEARTS_ENV_R,
        HEARTS_ENV_G - HEARTS_ENV_G,
        HEARTS_ENV_B - HEARTS_ENV_B,
    },
    // unused
    {
        HEARTS_BURN_ENV_R - HEARTS_ENV_R,
        HEARTS_BURN_ENV_G - HEARTS_ENV_G,
        HEARTS_BURN_ENV_B - HEARTS_ENV_B,
    },
    // unused
    {
        HEARTS_DROWN_ENV_R - HEARTS_ENV_R,
        HEARTS_DROWN_ENV_G - HEARTS_ENV_G,
        HEARTS_DROWN_ENV_B - HEARTS_ENV_B,
    },
};

static s16 sHeartsDDPrimColors[3][3] = {
    { HEARTS_DD_PRIM_R, HEARTS_DD_PRIM_G, HEARTS_DD_PRIM_B },
    { HEARTS_BURN_PRIM_R, HEARTS_BURN_PRIM_G, HEARTS_BURN_PRIM_B },    // unused
    { HEARTS_DROWN_PRIM_R, HEARTS_DROWN_PRIM_G, HEARTS_DROWN_PRIM_B }, // unused
};

static s16 sHeartsDDEnvColors[3][3] = {
    { HEARTS_DD_ENV_R, HEARTS_DD_ENV_G, HEARTS_DD_ENV_B },
    { HEARTS_BURN_ENV_R, HEARTS_BURN_ENV_G, HEARTS_BURN_ENV_B },    // unused
    { HEARTS_DROWN_ENV_R, HEARTS_DROWN_ENV_G, HEARTS_DROWN_ENV_B }, // unused
};

static s16 sHeartsDDPrimFactors[3][3] = {
    {
        HEARTS_DD_PRIM_R - HEARTS_DD_PRIM_R,
        HEARTS_DD_PRIM_G - HEARTS_DD_PRIM_G,
        HEARTS_DD_PRIM_B - HEARTS_DD_PRIM_B,
    },
    // unused
    {
        HEARTS_BURN_PRIM_R - HEARTS_DD_PRIM_R,
        HEARTS_BURN_PRIM_G - HEARTS_DD_PRIM_G,
        HEARTS_BURN_PRIM_B - HEARTS_DD_PRIM_B,
    },
    // unused
    {
        HEARTS_DROWN_PRIM_R - HEARTS_DD_PRIM_R,
        HEARTS_DROWN_PRIM_G - HEARTS_DD_PRIM_G,
        HEARTS_DROWN_PRIM_B - HEARTS_DD_PRIM_B,
    },
};

static s16 sHeartsDDEnvFactors[3][3] = {
    {
        HEARTS_DD_ENV_R - HEARTS_DD_ENV_R,
        HEARTS_DD_ENV_G - HEARTS_DD_ENV_G,
        HEARTS_DD_ENV_B - HEARTS_DD_ENV_B,
    },
    // unused
    {
        HEARTS_BURN_ENV_R - HEARTS_DD_ENV_R,
        HEARTS_BURN_ENV_G - HEARTS_DD_ENV_G,
        HEARTS_BURN_ENV_B - HEARTS_DD_ENV_B,
    },
    // unused
    {
        HEARTS_DROWN_ENV_R - HEARTS_DD_ENV_R,
        HEARTS_DROWN_ENV_G - HEARTS_DD_ENV_G,
        HEARTS_DROWN_ENV_B - HEARTS_DD_ENV_B,
    },
};

// Current colors for the double defense hearts
s16 sBeatingHeartsDDPrim[3];
s16 sBeatingHeartsDDEnv[3];
s16 sHeartsDDPrim[2][3];
s16 sHeartsDDEnv[2][3];

void Health_InitMeter(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    interfaceCtx->unk_228 = 0x140;
    interfaceCtx->unk_226 = gSaveContext.save.info.playerData.health;
    interfaceCtx->beatingHeartOscillator = interfaceCtx->heartColorOscillator = 0;
    interfaceCtx->beatingHeartOscillatorDirection = interfaceCtx->heartColorOscillatorDirection = 0;

    interfaceCtx->heartsPrimR[0] = HEARTS_PRIM_R;
    interfaceCtx->heartsPrimG[0] = HEARTS_PRIM_G;
    interfaceCtx->heartsPrimB[0] = HEARTS_PRIM_B;

    interfaceCtx->heartsEnvR[0] = HEARTS_ENV_R;
    interfaceCtx->heartsEnvG[0] = HEARTS_ENV_G;
    interfaceCtx->heartsEnvB[0] = HEARTS_ENV_B;

    interfaceCtx->heartsPrimR[1] = HEARTS_PRIM_R;
    interfaceCtx->heartsPrimG[1] = HEARTS_PRIM_G;
    interfaceCtx->heartsPrimB[1] = HEARTS_PRIM_B;

    interfaceCtx->heartsEnvR[1] = HEARTS_ENV_R;
    interfaceCtx->heartsEnvG[1] = HEARTS_ENV_G;
    interfaceCtx->heartsEnvB[1] = HEARTS_ENV_B;

    sHeartsDDPrim[0][0] = sHeartsDDPrim[1][0] = HEARTS_DD_PRIM_R;
    sHeartsDDPrim[0][1] = sHeartsDDPrim[1][1] = HEARTS_DD_PRIM_G;
    sHeartsDDPrim[0][2] = sHeartsDDPrim[1][2] = HEARTS_DD_PRIM_B;

    sHeartsDDEnv[0][0] = sHeartsDDEnv[1][0] = HEARTS_DD_ENV_R;
    sHeartsDDEnv[0][1] = sHeartsDDEnv[1][1] = HEARTS_DD_ENV_G;
    sHeartsDDEnv[0][2] = sHeartsDDEnv[1][2] = HEARTS_DD_ENV_B;
}

void Health_UpdateMeter(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    f32 factor = interfaceCtx->heartColorOscillator * 0.1f;
    f32 ddFactor;
    s32 type = 0;
    s32 ddType;
    s16 rFactor;
    s16 gFactor;
    s16 bFactor;

    if (interfaceCtx) {}

    if (interfaceCtx->heartColorOscillatorDirection != 0) {
        interfaceCtx->heartColorOscillator--;
        if (interfaceCtx->heartColorOscillator <= 0) {
            interfaceCtx->heartColorOscillator = 0;
            interfaceCtx->heartColorOscillatorDirection = 0;
        }
    } else {
        interfaceCtx->heartColorOscillator++;
        if (interfaceCtx->heartColorOscillator >= 10) {
            interfaceCtx->heartColorOscillator = 10;
            interfaceCtx->heartColorOscillatorDirection = 1;
        }
    }

    ddFactor = factor;

    interfaceCtx->heartsPrimR[0] = HEARTS_PRIM_R;
    interfaceCtx->heartsPrimG[0] = HEARTS_PRIM_G;
    interfaceCtx->heartsPrimB[0] = HEARTS_PRIM_B;

    interfaceCtx->heartsEnvR[0] = HEARTS_ENV_R;
    interfaceCtx->heartsEnvG[0] = HEARTS_ENV_G;
    interfaceCtx->heartsEnvB[0] = HEARTS_ENV_B;

    interfaceCtx->heartsPrimR[1] = sHeartsPrimColors[type][0];
    interfaceCtx->heartsPrimG[1] = sHeartsPrimColors[type][1];
    interfaceCtx->heartsPrimB[1] = sHeartsPrimColors[type][2];

    interfaceCtx->heartsEnvR[1] = sHeartsEnvColors[type][0];
    interfaceCtx->heartsEnvG[1] = sHeartsEnvColors[type][1];
    interfaceCtx->heartsEnvB[1] = sHeartsEnvColors[type][2];

    rFactor = sHeartsPrimFactors[0][0] * factor;
    gFactor = sHeartsPrimFactors[0][1] * factor;
    bFactor = sHeartsPrimFactors[0][2] * factor;

    interfaceCtx->beatingHeartPrim[0] = (u8)(rFactor + HEARTS_PRIM_R) & 0xFF;
    interfaceCtx->beatingHeartPrim[1] = (u8)(gFactor + HEARTS_PRIM_G) & 0xFF;
    interfaceCtx->beatingHeartPrim[2] = (u8)(bFactor + HEARTS_PRIM_B) & 0xFF;

    rFactor = sHeartsEnvFactors[0][0] * factor;
    gFactor = sHeartsEnvFactors[0][1] * factor;
    bFactor = sHeartsEnvFactors[0][2] * factor;

    if (1) {}
    ddType = type;

    interfaceCtx->beatingHeartEnv[0] = (u8)(rFactor + HEARTS_ENV_R) & 0xFF;
    interfaceCtx->beatingHeartEnv[1] = (u8)(gFactor + HEARTS_ENV_G) & 0xFF;
    interfaceCtx->beatingHeartEnv[2] = (u8)(bFactor + HEARTS_ENV_B) & 0xFF;

    sHeartsDDPrim[0][0] = HEARTS_DD_PRIM_R;
    sHeartsDDPrim[0][1] = HEARTS_DD_PRIM_G;
    sHeartsDDPrim[0][2] = HEARTS_DD_PRIM_B;

    sHeartsDDEnv[0][0] = HEARTS_DD_ENV_R;
    sHeartsDDEnv[0][1] = HEARTS_DD_ENV_G;
    sHeartsDDEnv[0][2] = HEARTS_DD_ENV_B;

    sHeartsDDPrim[1][0] = sHeartsDDPrimColors[ddType][0];
    sHeartsDDPrim[1][1] = sHeartsDDPrimColors[ddType][1];
    sHeartsDDPrim[1][2] = sHeartsDDPrimColors[ddType][2];

    sHeartsDDEnv[1][0] = sHeartsDDEnvColors[ddType][0];
    sHeartsDDEnv[1][1] = sHeartsDDEnvColors[ddType][1];
    sHeartsDDEnv[1][2] = sHeartsDDEnvColors[ddType][2];

    rFactor = sHeartsDDPrimFactors[ddType][0] * ddFactor;
    gFactor = sHeartsDDPrimFactors[ddType][1] * ddFactor;
    bFactor = sHeartsDDPrimFactors[ddType][2] * ddFactor;

    sBeatingHeartsDDPrim[0] = (u8)(rFactor + HEARTS_DD_PRIM_R) & 0xFF;
    sBeatingHeartsDDPrim[1] = (u8)(gFactor + HEARTS_DD_PRIM_G) & 0xFF;
    sBeatingHeartsDDPrim[2] = (u8)(bFactor + HEARTS_DD_PRIM_B) & 0xFF;

    rFactor = sHeartsDDEnvFactors[ddType][0] * ddFactor;
    gFactor = sHeartsDDEnvFactors[ddType][1] * ddFactor;
    bFactor = sHeartsDDEnvFactors[ddType][2] * ddFactor;

    sBeatingHeartsDDEnv[0] = (u8)(rFactor + HEARTS_DD_ENV_R) & 0xFF;
    sBeatingHeartsDDEnv[1] = (u8)(gFactor + HEARTS_DD_ENV_G) & 0xFF;
    sBeatingHeartsDDEnv[2] = (u8)(bFactor + HEARTS_DD_ENV_B) & 0xFF;
}

// Unused
s32 func_80078E18(PlayState* play) {
    gSaveContext.save.info.playerData.health = play->interfaceCtx.unk_226;
    return 1;
}

// Unused
s32 func_80078E34(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    interfaceCtx->unk_228 = 0x140;
    interfaceCtx->unk_226 += 0x10;

    if (interfaceCtx->unk_226 >= gSaveContext.save.info.playerData.health) {
        interfaceCtx->unk_226 = gSaveContext.save.info.playerData.health;
        return 1;
    }

    return 0;
}

// Unused
s32 func_80078E84(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (interfaceCtx->unk_228 != 0) {
        interfaceCtx->unk_228--;
    } else {
        interfaceCtx->unk_228 = 0x140;
        interfaceCtx->unk_226 -= 0x10;
        if (interfaceCtx->unk_226 <= 0) {
            interfaceCtx->unk_226 = 0;
            play->damagePlayer(play, -(gSaveContext.save.info.playerData.health + 1));
            return 1;
        }
    }
    return 0;
}

static void* sHeartTextures[] = {
    gHeartFullTex,         gHeartQuarterTex,      gHeartQuarterTex,      gHeartQuarterTex,
    gHeartQuarterTex,      gHeartQuarterTex,      gHeartHalfTex,         gHeartHalfTex,
    gHeartHalfTex,         gHeartHalfTex,         gHeartHalfTex,         gHeartThreeQuarterTex,
    gHeartThreeQuarterTex, gHeartThreeQuarterTex, gHeartThreeQuarterTex, gHeartThreeQuarterTex,
};

static void* sHeartDDTextures[] = {
    gDefenseHeartFullTex,         gDefenseHeartQuarterTex,      gDefenseHeartQuarterTex,
    gDefenseHeartQuarterTex,      gDefenseHeartQuarterTex,      gDefenseHeartQuarterTex,
    gDefenseHeartHalfTex,         gDefenseHeartHalfTex,         gDefenseHeartHalfTex,
    gDefenseHeartHalfTex,         gDefenseHeartHalfTex,         gDefenseHeartThreeQuarterTex,
    gDefenseHeartThreeQuarterTex, gDefenseHeartThreeQuarterTex, gDefenseHeartThreeQuarterTex,
    gDefenseHeartThreeQuarterTex,
};

void Health_DrawMeter(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    s16 health    = gSaveContext.save.info.playerData.health;
    s16 maxHealth = gSaveContext.save.info.playerData.healthCapacity;

    if (health <= 0 || maxHealth <= 0) {
        return;
    }

    f32 healthFrac = (f32)health / (f32)maxHealth;
    healthFrac = CLAMP(healthFrac, 0.0f, 1.0f);

    // ---- HUD placement ----
    s16 x = 3;
    s16 y = 69;
    s16 width  = 50;
    s16 height = 7;

#if IS_INV_EDITOR_ENABLED
    if (IS_INV_EDITOR_ACTIVE) {
        y += gDebug.invDebug.miscDebug.hudTopPosY;
    }
#endif

    // ---- Bouncing shine scroll ----
    s32 texWidthFP = 16 << 5; // 16px texture, fixed-point
    s32 t = play->gameplayFrames * 2;
    s32 phase = t % (texWidthFP * 2);
    s32 shineScroll = (phase < texWidthFP) ? phase : (texWidthFP * 2 - phase);

    OPEN_DISPS(gfxCtx, "../z_lifebar.c", 1);
    Gfx_SetupDL_39Overlay(gfxCtx);

    gDPLoadMultiBlock_4b(
        OVERLAY_DISP++, gMagicMeterFillTex, 0x0000, G_TX_RENDERTILE,
        G_IM_FMT_I, 16, 16, 0,
        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );

    // =========================================================
    // 1) BACKGROUND (empty health)
    // =========================================================
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 60, 70, interfaceCtx->healthAlpha);
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_PASS, G_RM_AA_XLU_SURF2);

    gDPSetCombineLERP(OVERLAY_DISP++,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, PRIMITIVE, 0
    );

    gSPTextureRectangle(
        OVERLAY_DISP++,
        x << 2, y << 2,
        (x + width) << 2, (y + height) << 2,
        G_TX_RENDERTILE,
        0, 0,
        1 << 10, 1 << 10
    );

    // =========================================================
    // 2) FILLED HEALTH BAR
    // =========================================================
    s16 filledW = (s16)(width * healthFrac);

    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 255, 240, interfaceCtx->healthAlpha);

    gSPTextureRectangle(
        OVERLAY_DISP++,
        x << 2, y << 2,
        (x + filledW) << 2, (y + height) << 2,
        G_TX_RENDERTILE,
        0, 0,
        1 << 10, 1 << 10
    );

    // =========================================================
    // 3) EDGE GLOW (top + bottom)
    // =========================================================
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_PASS, G_RM_AA_XLU_SURF2);

// Additive-style combine
gDPSetCombineLERP(OVERLAY_DISP++,
    TEXEL0, 0, PRIMITIVE, 0,
    TEXEL0, 0, PRIMITIVE, 0,
    TEXEL0, 0, PRIMITIVE, 0,
    TEXEL0, 0, PRIMITIVE, 0
);

// Bright aqua glow
gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 220, 255, 220);

// Top edge (2px)
gSPTextureRectangle(
    OVERLAY_DISP++,
    x << 2,
    y << 2,
    (x + filledW) << 2,
    (y + 2) << 2,
    G_TX_RENDERTILE,
    0, 0,
    1 << 10, 1 << 10
);

// Bottom edge (2px)
gSPTextureRectangle(
    OVERLAY_DISP++,
    x << 2,
    (y + height - 2) << 2,
    (x + filledW) << 2,
    (y + height) << 2,
    G_TX_RENDERTILE,
    0, 0,
    1 << 10, 1 << 10
);

    // =========================================================
    // 4) FULL-HEIGHT BOUNCING SHINE
    // =========================================================
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 255, 70);

    gSPTextureRectangle(
        OVERLAY_DISP++,
        x << 2, y << 2,
        (x + filledW) << 2, (y + height) << 2,
        G_TX_RENDERTILE,
        shineScroll, 0,
        1 << 10, 1 << 10
    );

    CLOSE_DISPS(gfxCtx, "../z_lifebar.c", 120);
}

void Health_DrawTensionAlvis(PlayState* play) {
    u32 curColorSet;
    f32 offsetX;
    f32 offsetY;
    f32 halfHeartLength;
    f32 heartCenterX;
    f32 heartCenterY;
    f32 heartTexCoordPerPixel;
    offsetY = 26.0f;
    offsetX = -15.0f;
   heartCenterY = 28.0f + offsetY;
            heartCenterX = 32.0f + offsetX;
            heartTexCoordPerPixel = 1.0f;
            heartTexCoordPerPixel /= 1.8f;
            heartTexCoordPerPixel *= 2 << 10;
            halfHeartLength = 17.0f;
            halfHeartLength *= 0.84f;


    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    gDPPipeSync(OVERLAY_DISP++);
    Gfx_SetupDL_39Overlay(play->state.gfxCtx);

    gDPSetCombineMode(OVERLAY_DISP++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetTextureFilter(OVERLAY_DISP++, G_TF_POINT);

    gDPLoadTextureBlock(
        OVERLAY_DISP++,
        gAlvisTensionTex,
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

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void Health_DrawMeterParty1(PlayState* play) {
     if (play->party.members[1] == NULL) {
        return;
        };
    s32 pad[5];
    void* heartBgImg;
    Actor* member = play->party.members[1];
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    GraphicsContext* gfxCtx = play->state.gfxCtx;


    s16 health = member->colChkInfo.health;
    s16 maxHealth = member->maxHealth;
     s32 texWidthFP = 16 << 5; // 16px texture, fixed-point
    s32 t = play->gameplayFrames * 2;
    s32 phase = t % (texWidthFP * 2);
         s32 shineScroll = (phase < texWidthFP) ? phase : (texWidthFP * 2 - phase);
    f32 healthFrac = (f32)health / (f32)maxHealth;
    healthFrac = CLAMP(healthFrac, 0.0f, 1.0f);

    if (maxHealth == 0)
        return;
    if (health < 0)
        health = 0;

    // --- Screen position (top-left HUD style) ---
    s16 x = 3;
    s16 y = 90;

    s16 width  = 50;
    s16 height = 8;

#if IS_INV_EDITOR_ENABLED
    if (IS_INV_EDITOR_ACTIVE) {
        y += gDebug.invDebug.miscDebug.hudTopPosY;
    }
#endif

    OPEN_DISPS(gfxCtx, "../z_lifebar.c", 1);
    gDPPipeSync(OVERLAY_DISP++);
    Gfx_SetupDL_39Overlay(gfxCtx);
    gSPTexture(OVERLAY_DISP++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    gDPLoadMultiBlock_4b(
        OVERLAY_DISP++, gMagicMeterFillTex, 0x0000, G_TX_RENDERTILE,
        G_IM_FMT_I, 16, 16, 0,
        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );

    // =========================================================
    // 1) BACKGROUND (empty health)
    // =========================================================
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 60, 70, interfaceCtx->healthAlpha);
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_PASS, G_RM_AA_XLU_SURF2);

    gDPSetCombineLERP(OVERLAY_DISP++,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, PRIMITIVE, 0
    );

    gSPTextureRectangle(
        OVERLAY_DISP++,
        x << 2, y << 2,
        (x + width) << 2, (y + height) << 2,
        G_TX_RENDERTILE,
        0, 0,
        1 << 10, 1 << 10
    );

    // =========================================================
    // 2) FILLED HEALTH BAR
    // =========================================================
    s16 filledW = (s16)(width * healthFrac);

    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 255, 240, interfaceCtx->healthAlpha);

    gSPTextureRectangle(
        OVERLAY_DISP++,
        x << 2, y << 2,
        (x + filledW) << 2, (y + height) << 2,
        G_TX_RENDERTILE,
        0, 0,
        1 << 10, 1 << 10
    );

    // =========================================================
    // 3) EDGE GLOW (top + bottom)
    // =========================================================
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_PASS, G_RM_AA_XLU_SURF2);

// Additive-style combine
gDPSetCombineLERP(OVERLAY_DISP++,
    TEXEL0, 0, PRIMITIVE, 0,
    TEXEL0, 0, PRIMITIVE, 0,
    TEXEL0, 0, PRIMITIVE, 0,
    TEXEL0, 0, PRIMITIVE, 0
);

// Bright aqua glow
gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 220, 255, 220);

// Top edge (2px)
gSPTextureRectangle(
    OVERLAY_DISP++,
    x << 2,
    y << 2,
    (x + filledW) << 2,
    (y + 2) << 2,
    G_TX_RENDERTILE,
    0, 0,
    1 << 10, 1 << 10
);

// Bottom edge (2px)
gSPTextureRectangle(
    OVERLAY_DISP++,
    x << 2,
    (y + height - 2) << 2,
    (x + filledW) << 2,
    (y + height) << 2,
    G_TX_RENDERTILE,
    0, 0,
    1 << 10, 1 << 10
);

    // =========================================================
    // 4) FULL-HEIGHT BOUNCING SHINE
    // =========================================================
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 255, 70);

    gSPTextureRectangle(
        OVERLAY_DISP++,
        x << 2, y << 2,
        (x + filledW) << 2, (y + height) << 2,
        G_TX_RENDERTILE,
        shineScroll, 0,
        1 << 10, 1 << 10
    );

    CLOSE_DISPS(gfxCtx, "../z_lifebar.c", 120);
}
void Health_DrawTensionParty1(PlayState* play) {

    if (play->party.members[1] == NULL) {
    return;
}

    u32 curColorSet;
    f32 offsetX;
    f32 offsetY;
    f32 halfHeartLength;
    f32 heartCenterX;
    f32 heartCenterY;
    f32 heartTexCoordPerPixel;
    offsetY = 64.0f;
    offsetX = -15.0f;
   heartCenterY = 28.0f + offsetY;
            heartCenterX = 32.0f + offsetX;
            heartTexCoordPerPixel = 1.0f;
            heartTexCoordPerPixel /= 1.8f;
            heartTexCoordPerPixel *= 2 << 10;
            halfHeartLength = 17.0f;
            halfHeartLength *= 0.84f;


    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    gDPPipeSync(OVERLAY_DISP++);
    Gfx_SetupDL_39Overlay(play->state.gfxCtx);

    gDPSetCombineMode(OVERLAY_DISP++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetTextureFilter(OVERLAY_DISP++, G_TF_POINT);

    gDPLoadTextureBlock(
        OVERLAY_DISP++,
        gTirkinTensionTex,
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

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}


void Health_UpdateBeatingHeart(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (interfaceCtx->beatingHeartOscillatorDirection != 0) {
        interfaceCtx->beatingHeartOscillator--;
        if (interfaceCtx->beatingHeartOscillator <= 0) {
            interfaceCtx->beatingHeartOscillator = 0;
            interfaceCtx->beatingHeartOscillatorDirection = 0;
            if (ENABLE_LOW_HEALTH_BEEP && !Player_InCsMode(play) && !IS_PAUSED(&play->pauseCtx) &&
                Health_IsCritical() && !Play_InCsMode(play)) {
                Sfx_PlaySfxCentered(NA_SE_SY_HITPOINT_ALARM);
            }
        }
    } else {
        interfaceCtx->beatingHeartOscillator++;
        if (interfaceCtx->beatingHeartOscillator >= 10) {
            interfaceCtx->beatingHeartOscillator = 10;
            interfaceCtx->beatingHeartOscillatorDirection = 1;
        }
    }
}

u32 Health_IsCritical(void) {
    s32 criticalHealth;

    if (gSaveContext.save.info.playerData.healthCapacity <= 0x50) {
        criticalHealth = 0x10;
    } else if (gSaveContext.save.info.playerData.healthCapacity <= 0xA0) {
        criticalHealth = 0x18;
    } else if (gSaveContext.save.info.playerData.healthCapacity <= 0xF0) {
        criticalHealth = 0x20;
    } else {
        criticalHealth = 0x2C;
    }

    if ((criticalHealth >= gSaveContext.save.info.playerData.health) &&
        (gSaveContext.save.info.playerData.health > 0)) {
        return true;
    } else {
        return false;
    }
}
