#include "z_kaleido_scope.h"

#include "controller.h"
#include "gfx.h"
#include "gfx_setupdl.h"
#include "printf.h"
#include "regs.h"
#include "sfx.h"
#include "translation.h"
#include "play_state.h"
#include "player.h"
#include "save.h"
#include "debug.h"

#include "assets/textures/icon_item_static/icon_item_static.h"
#include "assets/textures/parameter_static/parameter_static.h"

static u8 sChildUpgrades[] = {
    UPG_BULLET_BAG, // EQUIP_QUAD_UPG_BULLETBAG_QUIVER
    UPG_BOMB_BAG,   // EQUIP_QUAD_UPG_BOMB_BAG
    UPG_STRENGTH,   // EQUIP_QUAD_UPG_STRENGTH
    UPG_SCALE,      // EQUIP_QUAD_UPG_SCALE
};
static u8 sAdultUpgrades[] = {
    UPG_QUIVER,   // EQUIP_QUAD_UPG_BULLETBAG_QUIVER
    UPG_BOMB_BAG, // EQUIP_QUAD_UPG_BOMB_BAG
    UPG_STRENGTH, // EQUIP_QUAD_UPG_STRENGTH
    UPG_SCALE,    // EQUIP_QUAD_UPG_SCALE
};

static u8 sChildUpgradeItemBases[] = {
    ITEM_BULLET_BAG_30,            // EQUIP_QUAD_UPG_BULLETBAG_QUIVER
    ITEM_BOMB_BAG_20,              // EQUIP_QUAD_UPG_BOMB_BAG
    ITEM_STRENGTH_GORONS_BRACELET, // EQUIP_QUAD_UPG_STRENGTH
    ITEM_SCALE_SILVER,             // EQUIP_QUAD_UPG_SCALE
};
static u8 sAdultUpgradeItemBases[] = {
    ITEM_QUIVER_30,                // EQUIP_QUAD_UPG_BULLETBAG_QUIVER
    ITEM_BOMB_BAG_20,              // EQUIP_QUAD_UPG_BOMB_BAG
    ITEM_STRENGTH_GORONS_BRACELET, // EQUIP_QUAD_UPG_STRENGTH
    ITEM_SCALE_SILVER,             // EQUIP_QUAD_UPG_SCALE
};

static u8 sUpgradeItemOffsets[] = {
    0,                                              // unused
    ITEM_BOMB_BAG_20 - ITEM_QUIVER_30,              // UPG_BOMB_BAG
    ITEM_STRENGTH_GORONS_BRACELET - ITEM_QUIVER_30, // UPG_STRENGTH
    ITEM_SCALE_SILVER - ITEM_QUIVER_30,             // UPG_SCALE
};

static u8 sEquipmentItemOffsets[] = {
    // EQUIP_TYPE_SWORD
    0,                                       // unused
    ITEM_SWORD_KOKIRI - ITEM_SWORD_KOKIRI,   // EQUIP_VALUE_SWORD_KOKIRI
    ITEM_SWORD_MASTER - ITEM_SWORD_KOKIRI,   // EQUIP_VALUE_SWORD_MASTER
    ITEM_SWORD_BIGGORON - ITEM_SWORD_KOKIRI, // EQUIP_VALUE_SWORD_BIGGORON
    // EQUIP_TYPE_SHIELD
    0,                                      // unused
    ITEM_SHIELD_DEKU - ITEM_SWORD_KOKIRI,   // EQUIP_VALUE_SHIELD_DEKU
    ITEM_SHIELD_HYLIAN - ITEM_SWORD_KOKIRI, // EQUIP_VALUE_SHIELD_HYLIAN
    ITEM_SHIELD_MIRROR - ITEM_SWORD_KOKIRI, // EQUIP_VALUE_SHIELD_MIRROR
    // EQUIP_TYPE_TUNIC
    0,                                     // unused
    ITEM_TUNIC_KOKIRI - ITEM_SWORD_KOKIRI, // EQUIP_VALUE_TUNIC_KOKIRI
    ITEM_TUNIC_GORON - ITEM_SWORD_KOKIRI,  // EQUIP_VALUE_TUNIC_GORON
    ITEM_TUNIC_ZORA - ITEM_SWORD_KOKIRI,   // EQUIP_VALUE_TUNIC_ZORA
    // EQUIP_TYPE_BOOTS
    0,                                     // unused
    ITEM_BOOTS_KOKIRI - ITEM_SWORD_KOKIRI, // EQUIP_VALUE_BOOTS_KOKIRI
    ITEM_BOOTS_IRON - ITEM_SWORD_KOKIRI,   // EQUIP_VALUE_BOOTS_IRON
    ITEM_BOOTS_HOVER - ITEM_SWORD_KOKIRI,  // EQUIP_VALUE_BOOTS_HOVER
};

void KaleidoScope_DrawEquipmentImage(PlayState* play, void* source, u32 width, u32 height) {
    PauseContext* pauseCtx = &play->pauseCtx;
    u8* curTexture;
    s32 vtxIndex;
    s32 textureCount;
    s32 textureHeight;
    s32 remainingSize;
    s32 textureSize;
    s32 pad;
    s32 i;

    s16 alpha = pauseCtx->alpha;
#if IS_INV_EDITOR_ENABLED
    if (gDebug.invDebug.showInfoScreen || gDebug.invDebug.miscDebug.showMiscScreen) {
        alpha = gDebug.invDebug.elementsAlpha;
    } else {
        alpha = gDebug.invDebug.elementsAlpha < 255 ? gDebug.invDebug.elementsAlpha : pauseCtx->alpha;
    }
#endif

    OPEN_DISPS(play->state.gfxCtx, "../z_kaleido_equipment.c", 68);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, alpha);

    curTexture = source;
    remainingSize = width * height * G_IM_SIZ_16b_BYTES;
    textureHeight = TMEM_SIZE / (width * G_IM_SIZ_16b_BYTES);
    textureSize = width * textureHeight * G_IM_SIZ_16b_BYTES;
    textureCount = remainingSize / textureSize;
    if ((remainingSize % textureSize) != 0) {
        textureCount += 1;
    }

    vtxIndex = EQUIP_QUAD_PLAYER_FIRST * 4;

    gDPSetTileCustom(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, width - 1, textureHeight - 1, 0,
                     G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                     G_TX_NOLOD);

    remainingSize -= textureSize;

    for (i = 0; i < textureCount; i++) {
        gSPVertex(POLY_OPA_DISP++, &pauseCtx->equipVtx[vtxIndex], 4, 0);

        gDPSetTextureImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, curTexture);

        gDPLoadSync(POLY_OPA_DISP++);
        gDPLoadTile(POLY_OPA_DISP++, G_TX_LOADTILE, 0, 0, (width - 1) << 2, (textureHeight - 1) << 2);

        gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);

        curTexture += textureSize;

        if ((remainingSize - textureSize) < 0) {
            if (remainingSize > 0) {
                textureHeight = remainingSize / (s32)(width * G_IM_SIZ_16b_BYTES);
                remainingSize -= textureSize;

                gDPSetTileCustom(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, width - 1, textureHeight - 1, 0,
                                 G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);
            }
        } else {
            remainingSize -= textureSize;
        }

        vtxIndex += 4;
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_kaleido_equipment.c", 122);
}

void KaleidoScope_DrawPlayerWork(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Vec3f pos;
    Vec3s rot;
    f32 scale;

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        pos.x = 2.0f;
        pos.y = -130.0f;
        pos.z = -150.0f;
        scale = 0.046f;
    } else if (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) != EQUIP_VALUE_SWORD_MASTER) {
        pos.x = 25.0f;
        pos.y = -228.0f;
        pos.z = 60.0f;
        scale = 0.056f;
    } else {
        pos.x = 20.0f;
        pos.y = -180.0f;
        pos.z = -40.0f;
        scale = 0.047f;
    }

    rot.y = 32300;
    rot.x = rot.z = 0;
    Player_DrawPause(play, pauseCtx->playerSegment, &pauseCtx->playerSkelAnime, &pos, &rot, scale,
                     SWORD_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD)),
                     TUNIC_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC)),
                     SHIELD_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD)),
                     BOOTS_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_BOOTS)));
}

#ifndef AVOID_UB
// Wrong prototype; this function is called with `play` even though it has no arguments
void KaleidoScope_ProcessPlayerPreRender(PlayState* play);
#endif

void KaleidoScope_DrawEquipment(PlayState* play) {

}
