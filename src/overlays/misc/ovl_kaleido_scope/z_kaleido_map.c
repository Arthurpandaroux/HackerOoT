#include "z_kaleido_scope.h"

#include "gfx.h"
#include "gfx_setupdl.h"
#include "language_array.h"
#include "map.h"
#include "printf.h"
#include "regs.h"
#include "sfx.h"
#include "sys_ucode.h"
#include "versions.h"
#include "play_state.h"
#include "save.h"

#include "assets/textures/icon_item_24_static/icon_item_24_static.h"
#if OOT_NTSC
#include "assets/textures/icon_item_jpn_static/icon_item_jpn_static.h"
#include "assets/textures/icon_item_nes_static/icon_item_nes_static.h"
#else
#include "assets/textures/icon_item_nes_static/icon_item_nes_static.h"
#include "assets/textures/icon_item_ger_static/icon_item_ger_static.h"
#include "assets/textures/icon_item_fra_static/icon_item_fra_static.h"
#endif
#include "assets/textures/icon_item_field_static/icon_item_field_static.h"
#include "assets/textures/icon_item_dungeon_static/icon_item_dungeon_static.h"
#include "assets/textures/icon_item_nes_static/icon_item_nes_static.h"

void KaleidoScope_DrawDungeonMap(PlayState* play, GraphicsContext* gfxCtx) {
    static void* dungeonItemTexs[] = {
        gQuestIconDungeonBossKeyTex,
        gQuestIconDungeonCompassTex,
        gQuestIconDungeonMapTex,
    };
    static void* dungeonTitleTexs[] = {
        gPauseDekuTitleENGTex, gPauseDodongoTitleENGTex,   gPauseJabuTitleENGTex,   gPauseForestTitleENGTex,
        gPauseFireTitleENGTex, gPauseWaterTitleENGTex,     gPauseSpiritTitleENGTex, gPauseShadowTitleENGTex,
        gPauseBotWTitleENGTex, gPauseIceCavernTitleENGTex,
    };
    static void* floorIconTexs[] = {
        gDungeonMapBlankFloorButtonTex, gDungeonMap8FButtonTex, gDungeonMap7FButtonTex, gDungeonMap6FButtonTex,
        gDungeonMap5FButtonTex,         gDungeonMap4FButtonTex, gDungeonMap3FButtonTex, gDungeonMap2FButtonTex,
        gDungeonMap1FButtonTex,         gDungeonMapB1ButtonTex, gDungeonMapB2ButtonTex, gDungeonMapB3ButtonTex,
        gDungeonMapB4ButtonTex,         gDungeonMapB5ButtonTex, gDungeonMapB6ButtonTex, gDungeonMapB7ButtonTex,
        gDungeonMapB8ButtonTex,
    };
    static u16 mapBgPulseColors[][3] = {
        { 0 / 8, 80 / 8, 255 / 8 },
        { 0 / 8, 200 / 8, 140 / 8 },
    };
    static s16 mapBgPulseR = 0 / 8;
    static s16 mapBgPulseG = 200 / 8;
    static s16 mapBgPulseB = 140 / 8;
    static u16 mapBgPulseTimer = 20;
    static u16 mapBgPulseStage = 0;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 i;
    s16 j;
    s16 oldCursorPoint;
    s16 stepR;
    s16 stepG;
    s16 stepB;
    u16 rgba16;

    OPEN_DISPS(gfxCtx, "../z_kaleido_map_PAL.c", 123);

    if ((pauseCtx->state == PAUSE_STATE_MAIN) && (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
        (pauseCtx->pageIndex == PAUSE_MAP)) {
        pauseCtx->cursorColorSet = 0;
        oldCursorPoint = pauseCtx->cursorPoint[PAUSE_MAP];

        if (pauseCtx->cursorSpecialPos == 0) {
            if (pauseCtx->stickAdjX > 30) {
                if (pauseCtx->cursorX[PAUSE_MAP] != 0) {
                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                } else {
                    pauseCtx->cursorX[PAUSE_MAP] = 1;
                    pauseCtx->cursorPoint[PAUSE_MAP] = 0;
                    if (!CHECK_DUNGEON_ITEM(DUNGEON_BOSS_KEY, gSaveContext.mapIndex)) {
                        pauseCtx->cursorPoint[PAUSE_MAP]++;
                        if (!CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, gSaveContext.mapIndex)) {
                            pauseCtx->cursorPoint[PAUSE_MAP]++;
                            if (!CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex)) {
                                KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                            }
                        }
                    }
                }
            } else if (pauseCtx->stickAdjX < -30) {
                if (pauseCtx->cursorX[PAUSE_MAP] == 0) {
                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);
                } else {
                    pauseCtx->cursorX[PAUSE_MAP] = 0;
                    pauseCtx->cursorPoint[PAUSE_MAP] = pauseCtx->dungeonMapSlot;
                    PRINTF("kscope->cursor_point=%d\n", pauseCtx->cursorPoint[PAUSE_MAP]);
                    R_MAP_TEX_INDEX =
                        R_MAP_TEX_INDEX_BASE +
                        gMapData->floorTexIndexOffset[gSaveContext.mapIndex][pauseCtx->cursorPoint[PAUSE_MAP] - 3];
                    KaleidoScope_UpdateDungeonMap(play);
                }
            }

            if (pauseCtx->cursorPoint[PAUSE_MAP] < 3) {
                if (pauseCtx->stickAdjY > 30) {
                    if (pauseCtx->cursorPoint[PAUSE_MAP] != 0) {
                        for (i = pauseCtx->cursorPoint[PAUSE_MAP] - 1; i >= 0; i--) {
                            if (CHECK_DUNGEON_ITEM(i, gSaveContext.mapIndex)) {
                                pauseCtx->cursorPoint[PAUSE_MAP] = i;
                                break;
                            }
                        }
                    }
                } else {
                    if (pauseCtx->stickAdjY < -30) {
                        if (pauseCtx->cursorPoint[PAUSE_MAP] != 2) {
                            for (i = pauseCtx->cursorPoint[PAUSE_MAP] + 1; i < 3; i++) {
                                if (CHECK_DUNGEON_ITEM(i, gSaveContext.mapIndex)) {
                                    pauseCtx->cursorPoint[PAUSE_MAP] = i;
                                    break;
                                }
                            }
                        }
                    }
                }
            } else {
                if (pauseCtx->stickAdjY > 30) {
                    if (pauseCtx->cursorPoint[PAUSE_MAP] >= 4) {
                        for (i = pauseCtx->cursorPoint[PAUSE_MAP] - 3 - 1; i >= 0; i--) {
                            if ((gSaveContext.save.info.sceneFlags[gSaveContext.mapIndex].floors & gBitFlags[i]) ||
                                (CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex) &&
                                 (gMapData->floorID[interfaceCtx->unk_25A][i] != 0))) {
                                pauseCtx->cursorPoint[PAUSE_MAP] = i + 3;
                                break;
                            }
                        }
                    }
                } else if (pauseCtx->stickAdjY < -30) {
                    if (pauseCtx->cursorPoint[PAUSE_MAP] != 10) {
                        for (i = pauseCtx->cursorPoint[PAUSE_MAP] - 3 + 1; i < 11; i++) {
                            if ((gSaveContext.save.info.sceneFlags[gSaveContext.mapIndex].floors & gBitFlags[i]) ||
                                (CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex) &&
                                 (gMapData->floorID[interfaceCtx->unk_25A][i] != 0))) {
                                pauseCtx->cursorPoint[PAUSE_MAP] = i + 3;
                                break;
                            }
                        }
                    }
                }

                i = R_MAP_TEX_INDEX;
                R_MAP_TEX_INDEX =
                    R_MAP_TEX_INDEX_BASE +
                    gMapData->floorTexIndexOffset[gSaveContext.mapIndex][pauseCtx->cursorPoint[PAUSE_MAP] - 3];
                pauseCtx->dungeonMapSlot = pauseCtx->cursorPoint[PAUSE_MAP];
                if (i != R_MAP_TEX_INDEX) {
                    KaleidoScope_UpdateDungeonMap(play);
                }
            }
        } else if (pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_LEFT) {
            if (pauseCtx->stickAdjX > 30) {
                pauseCtx->nameDisplayTimer = 0;
                pauseCtx->cursorSpecialPos = 0;
                pauseCtx->cursorSlot[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_MAP] = pauseCtx->dungeonMapSlot;
                pauseCtx->cursorX[PAUSE_MAP] = 0;
                j = (pauseCtx->cursorSlot[PAUSE_MAP] + 18) * 4;
                KaleidoScope_SetCursorPos(pauseCtx, j, pauseCtx->mapPageVtx);
                SFX_PLAY_CENTERED(NA_SE_SY_CURSOR);
            }
        } else {
            if (pauseCtx->stickAdjX < -30) {
                pauseCtx->nameDisplayTimer = 0;
                pauseCtx->cursorSpecialPos = 0;
                pauseCtx->cursorX[PAUSE_MAP] = 1;
                pauseCtx->cursorPoint[PAUSE_MAP] = 0;
                if (!CHECK_DUNGEON_ITEM(DUNGEON_BOSS_KEY, gSaveContext.mapIndex)) {
                    pauseCtx->cursorPoint[PAUSE_MAP]++;
                    if (!CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, gSaveContext.mapIndex)) {
                        pauseCtx->cursorPoint[PAUSE_MAP]++;
                        if (!CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex)) {
                            pauseCtx->cursorX[PAUSE_MAP] = 0;
                            pauseCtx->cursorSlot[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_MAP] =
                                pauseCtx->dungeonMapSlot;
                            R_MAP_TEX_INDEX =
                                R_MAP_TEX_INDEX_BASE +
                                gMapData
                                    ->floorTexIndexOffset[gSaveContext.mapIndex][pauseCtx->cursorPoint[PAUSE_MAP] - 3];
                            KaleidoScope_UpdateDungeonMap(play);
                        }
                    }
                } else {
                    pauseCtx->cursorSlot[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_MAP];
                }

                PRINTF("kscope->cursor_point====%d\n", pauseCtx->cursorPoint[PAUSE_MAP]);
                j = (pauseCtx->cursorSlot[PAUSE_MAP] + 18) * 4;
                KaleidoScope_SetCursorPos(pauseCtx, j, pauseCtx->mapPageVtx);
                SFX_PLAY_CENTERED(NA_SE_SY_CURSOR);
            }
        }

        if (oldCursorPoint != pauseCtx->cursorPoint[PAUSE_MAP]) {
            SFX_PLAY_CENTERED(NA_SE_SY_CURSOR);
        }
    }

    if (pauseCtx->cursorSpecialPos == 0) {
        if (pauseCtx->cursorPoint[PAUSE_MAP] < 3) {
            pauseCtx->cursorItem[PAUSE_MAP] = ITEM_DUNGEON_BOSS_KEY + pauseCtx->cursorPoint[PAUSE_MAP];
        } else {
            pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
        }

        pauseCtx->cursorSlot[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_MAP];

        j = (pauseCtx->cursorSlot[PAUSE_MAP] + 18) * 4;
        KaleidoScope_SetCursorPos(pauseCtx, j, pauseCtx->mapPageVtx);

        if (pauseCtx->cursorX[PAUSE_MAP] == 0) {
            pauseCtx->mapPageVtx[j + 0].v.ob[0] = pauseCtx->mapPageVtx[j + 2].v.ob[0] =
                pauseCtx->mapPageVtx[j + 0].v.ob[0] - 2;
            pauseCtx->mapPageVtx[j + 1].v.ob[0] = pauseCtx->mapPageVtx[j + 3].v.ob[0] =
                pauseCtx->mapPageVtx[j + 1].v.ob[0] + 4;
            pauseCtx->mapPageVtx[j + 0].v.ob[1] = pauseCtx->mapPageVtx[j + 1].v.ob[1] =
                pauseCtx->mapPageVtx[j + 0].v.ob[1] + 2;
            pauseCtx->mapPageVtx[j + 2].v.ob[1] = pauseCtx->mapPageVtx[j + 3].v.ob[1] =
                pauseCtx->mapPageVtx[j + 2].v.ob[1] - 4;
        }
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA, G_CC_MODULATEIA);

    gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[68], 16, 0);

    gDPLoadTextureBlock(POLY_OPA_DISP++, dungeonTitleTexs[gSaveContext.mapIndex], G_IM_FMT_IA, G_IM_SIZ_8b, 96, 16, 0,
                        G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    for (i = 0, j = 4; i < 3; i++, j += 4) {
        if (CHECK_DUNGEON_ITEM(i, gSaveContext.mapIndex)) {
            gDPLoadTextureBlock(POLY_OPA_DISP++, dungeonItemTexs[i], G_IM_FMT_RGBA, G_IM_SIZ_32b, 24, 24, 0,
                                G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);

            gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
        }
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 200, pauseCtx->alpha);

    gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[84], 32, 0);

    for (i = j = 0; i < 8; i++, j += 4) {
        if ((gSaveContext.save.info.sceneFlags[gSaveContext.mapIndex].floors & gBitFlags[i]) ||
            CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex)) {
            if (i != (pauseCtx->dungeonMapSlot - 3)) {
                gDPLoadTextureBlock(POLY_OPA_DISP++, floorIconTexs[gMapData->floorID[interfaceCtx->unk_25A][i]],
                                    G_IM_FMT_IA, G_IM_SIZ_8b, 24, 16, 0, G_TX_WRAP | G_TX_NOMIRROR,
                                    G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

                gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
            }
        }
    }

    j = (pauseCtx->dungeonMapSlot - 3) * 4;

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 150, 150, 255, pauseCtx->alpha);

    gDPLoadTextureBlock(POLY_OPA_DISP++,
                        floorIconTexs[gMapData->floorID[interfaceCtx->unk_25A][pauseCtx->dungeonMapSlot - 3]],
                        G_IM_FMT_IA, G_IM_SIZ_8b, 24, 16, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
                        G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);

    pauseCtx->mapPageVtx[124].v.ob[0] = pauseCtx->mapPageVtx[126].v.ob[0] = pauseCtx->mapPageVtx[124].v.ob[0] + 2;
    pauseCtx->mapPageVtx[125].v.ob[0] = pauseCtx->mapPageVtx[127].v.ob[0] = pauseCtx->mapPageVtx[124].v.ob[0] + 19;
    pauseCtx->mapPageVtx[124].v.ob[1] = pauseCtx->mapPageVtx[125].v.ob[1] = pauseCtx->mapPageVtx[124].v.ob[1] - 2;
    pauseCtx->mapPageVtx[126].v.ob[1] = pauseCtx->mapPageVtx[127].v.ob[1] = pauseCtx->mapPageVtx[124].v.ob[1] - 19;

    gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[116], 12, 0);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    pauseCtx->mapPageVtx[116].v.ob[1] = pauseCtx->mapPageVtx[117].v.ob[1] =
        pauseCtx->pagesYOrigin1 + 50 - (VREG(30) * 14) - 1;
    pauseCtx->mapPageVtx[118].v.ob[1] = pauseCtx->mapPageVtx[119].v.ob[1] = pauseCtx->mapPageVtx[116].v.ob[1] - 16;

    gDPLoadTextureBlock(POLY_OPA_DISP++, gDungeonMapLinkHeadTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
                        G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);

    if (CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, gSaveContext.mapIndex) &&
        (gMapData->skullFloorIconY[gSaveContext.mapIndex] != -99)) {
        pauseCtx->mapPageVtx[120].v.ob[1] = pauseCtx->mapPageVtx[121].v.ob[1] =
            gMapData->skullFloorIconY[gSaveContext.mapIndex] + pauseCtx->pagesYOrigin1;
        pauseCtx->mapPageVtx[122].v.ob[1] = pauseCtx->mapPageVtx[123].v.ob[1] = pauseCtx->mapPageVtx[120].v.ob[1] - 16;

        gDPLoadTextureBlock(POLY_OPA_DISP++, gDungeonMapSkullTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);

        gSP1Quadrangle(POLY_OPA_DISP++, 4, 6, 7, 5, 0);
    }

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    if (GET_GS_FLAGS(gSaveContext.mapIndex) == gAreaGsFlags[gSaveContext.mapIndex]) {
        KaleidoScope_DrawQuadTextureRGBA32(gfxCtx, gQuestIconGoldSkulltulaTex, QUEST_ICON_WIDTH, QUEST_ICON_HEIGHT, 8);
    }

    if ((play->sceneId >= SCENE_DEKU_TREE) && (play->sceneId <= SCENE_TREASURE_BOX_SHOP)) {
        stepR = (mapBgPulseR - mapBgPulseColors[mapBgPulseStage][0]) / mapBgPulseTimer;
        stepG = (mapBgPulseG - mapBgPulseColors[mapBgPulseStage][1]) / mapBgPulseTimer;
        stepB = (mapBgPulseB - mapBgPulseColors[mapBgPulseStage][2]) / mapBgPulseTimer;
        mapBgPulseR -= stepR;
        mapBgPulseG -= stepG;
        mapBgPulseB -= stepB;

        rgba16 = ((mapBgPulseR & 0x1F) << 11) | ((mapBgPulseG & 0x1F) << 6) | ((mapBgPulseB & 0x1F) << 1) | 1;
        interfaceCtx->mapPalette[28] = (rgba16 & 0xFF00) >> 8;
        interfaceCtx->mapPalette[29] = rgba16 & 0xFF;

        mapBgPulseTimer--;
        if (mapBgPulseTimer == 0) {
            mapBgPulseStage ^= 1;
            mapBgPulseTimer = 20;
        }
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    gDPLoadTLUT_pal16(POLY_OPA_DISP++, 0, interfaceCtx->mapPalette);
    gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_RGBA16);

    gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[60], 8, 0);

    gDPLoadTextureBlock_4b(POLY_OPA_DISP++, interfaceCtx->mapSegment, G_IM_FMT_CI, 48, 85, 0, G_TX_WRAP | G_TX_NOMIRROR,
                           G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);

    gDPLoadTextureBlock_4b(POLY_OPA_DISP++, interfaceCtx->mapSegment + 0x800, G_IM_FMT_CI, 48, 85, 0,
                           G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                           G_TX_NOLOD);

    gSP1Quadrangle(POLY_OPA_DISP++, 4, 6, 7, 5, 0);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_BILERP);

    CLOSE_DISPS(gfxCtx, "../z_kaleido_map_PAL.c", 388);
}

void KaleidoScope_DrawWorldMap(PlayState* play, GraphicsContext* gfxCtx) {

}
