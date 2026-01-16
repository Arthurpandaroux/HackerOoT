/*
 * File: z_title.c
 * Overlay: ovl_title
 * Description: Displays the Nintendo Logo
 */

#include "helpers.h"
#include "libu64/gfxprint.h"
#if PLATFORM_N64
#include "cic6105.h"
#include "n64dd.h"
#endif

#include "alloca.h"
#include "build.h"
#include "console_logo_state.h"
#include "gfx.h"
#include "gfx_setupdl.h"
#include "padmgr.h"
#include "printf.h"
#include "regs.h"
#include "segment_symbols.h"
#include "sequence.h"
#include "sys_matrix.h"
#include "sys_debug_controller.h"
#include "sys_freeze.h"
#include "title_setup_state.h"
#include "versions.h"
#include "widescreen.h"
#include "debug_opening_state.h"
#include "letterbox.h"
#include "actor.h"
#include "environment.h"
#include "save.h"

#include "assets/textures/nintendo_rogo_static/nintendo_rogo_static.h"

void ConsoleLogo_Calc(ConsoleLogoState* this) {
#if !PLATFORM_GC
    if ((this->coverAlpha == 0) && (this->visibleDuration != 0)) {
        this->timer--;
        this->visibleDuration--;
        if (this->timer == 0) {
            this->timer = 400;
        }
    } else {
        if (this->coverAlpha == 0 && this->visibleDuration != 0) {
            this->timer--;
            this->visibleDuration--;
            if (this->timer == 0) {
                this->timer = 400;
            }
        } else {
            this->coverAlpha += this->addAlpha;
            if (this->coverAlpha <= 0) {
                this->coverAlpha = 0;
                this->addAlpha = MM_N64_BOOT_LOGO ? 12 : 3;
            } else if (this->coverAlpha >= 255) {
                this->coverAlpha = 255;
                this->exit = true;
            }
        }
        this->uls = this->ult & 0x7F;
        this->ult++;
    }
#endif
}

void ConsoleLogo_SetupView(ConsoleLogoState* this, f32 x, f32 y, f32 z) {
    View* view = &this->view;
    Vec3f eye;
    Vec3f lookAt;
    Vec3f up;

    eye.x = x;
    eye.y = y;
    eye.z = z;
    up.x = up.z = 0.0f;
    up.y = 1.0f;
    lookAt.x = lookAt.y = lookAt.z = 0.0f;

    View_SetPerspective(view, 30.0f, 10.0f, 12800.0f);
    View_LookAt(view, &eye, &lookAt, &up);
    View_Apply(view, VIEW_ALL);
}

void ConsoleLogo_Draw(ConsoleLogoState* this) {
}

void ConsoleLogo_Main(GameState* thisx) {
    ConsoleLogoState* this = (ConsoleLogoState*)thisx;

    OPEN_DISPS(this->state.gfxCtx, "../z_title.c", 494);

    gSPSegment(POLY_OPA_DISP++, 0, NULL);
    gSPSegment(POLY_OPA_DISP++, 1, this->staticSegment);
    Gfx_SetupFrame(this->state.gfxCtx, true, 0, 0, 0);
    Gfx_ClearZBuffer(this->state.gfxCtx);

#if SKIP_N64_BOOT_LOGO
    this->exit = true;
#else
    ConsoleLogo_Calc(this);
    ConsoleLogo_Draw(this);
#endif

    if (this->exit) {
#if ENABLE_HACKER_DEBUG
#if BOOT_TO_SCENE
        Helpers_LoadDefinedScene(thisx);
#elif IS_DEBUG_BOOT_ENABLED && BOOT_TO_DEBUG_OPENING
        this->state.running = false;
        SET_NEXT_GAMESTATE(&this->state, DebugOpening_Init, DebugOpeningState);
#else
        Helpers_LoadTitleScreen(thisx);
#endif
#else
        Helpers_LoadTitleScreen(thisx);
#endif
    }

    CLOSE_DISPS(this->state.gfxCtx, "../z_title.c", 541);
}

void ConsoleLogo_Destroy(GameState* thisx) {
    ConsoleLogoState* this = (ConsoleLogoState*)thisx;

#if PLATFORM_N64
    if (this->unk_1E0) {
        if (func_801C7818() != 0) {
            func_800D31A0();
        }
        func_801C7268();
    }
#endif

    Sram_InitSram(&this->state, &this->sramCtx);

#if PLATFORM_N64
    func_800014E8();
#endif

#if ENABLE_NEW_LETTERBOX
    ShrinkWindow_Destroy();
#endif
}

void ConsoleLogo_Init(GameState* thisx) {
    u32 size = (uintptr_t)_nintendo_rogo_staticSegmentRomEnd - (uintptr_t)_nintendo_rogo_staticSegmentRomStart;
    ConsoleLogoState* this = (ConsoleLogoState*)thisx;

#if PLATFORM_N64
    if ((D_80121210 != 0) && (D_80121211 != 0) && (D_80121212 == 0)) {
        if (func_801C7658() != 0) {
            func_800D31A0();
        }
        this->unk_1E0 = true;
    } else {
        this->unk_1E0 = false;
    }
#endif

    this->staticSegment = GAME_STATE_ALLOC(&this->state, size, "../z_title.c", 611);
    PRINTF("z_title.c\n");
    ASSERT(this->staticSegment != NULL, "this->staticSegment != NULL", "../z_title.c", 614);
    DMA_REQUEST_SYNC(this->staticSegment, (uintptr_t)_nintendo_rogo_staticSegmentRomStart, size, "../z_title.c", 615);
    R_UPDATE_RATE = 1;
    Matrix_Init(&this->state);
#if ENABLE_NEW_LETTERBOX
    ShrinkWindow_Init();
#endif
    View_Init(&this->view, this->state.gfxCtx);
    this->state.main = ConsoleLogo_Main;
    this->state.destroy = ConsoleLogo_Destroy;
    this->exit = false;

#if OOT_VERSION < GC_US || PLATFORM_IQUE
    if (!(gPadMgr.validCtrlrsMask & 1)) {
        gSaveContext.fileNum = 0xFEDC;
    } else {
        gSaveContext.fileNum = 0xFF;
    }
#else
    gSaveContext.fileNum = 0xFF;
#endif

    Sram_Alloc(&this->state, &this->sramCtx);
    this->ult = 0;
    this->timer = 20;
    this->coverAlpha = 255;
    this->addAlpha = MM_N64_BOOT_LOGO ? -12 : -3;
    this->visibleDuration = 0x3C;
}
