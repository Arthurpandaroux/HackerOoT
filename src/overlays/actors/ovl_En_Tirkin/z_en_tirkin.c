/*
 * File: z_en_tirkin.c
 * Overlay: ovl_En_Tirkin
 * Description: Custom Actor
 */

#include "z_en_tirkin.h"

#include "play_state.h"
#include "sys_matrix.h"
#include "libc64/qrand.h"
#include "array_count.h"
#include "gfx.h"
#include "gfx_setupdl.h"
#include "ichain.h"
#include "rand.h"
#include "sfx.h"
#include "sys_matrix.h"
#include "versions.h"
#include "z_en_item00.h"
#include "z_lib.h"
#include "effect.h"
#include "player.h"
#include "collision_check.h"
#include "save.h"

#include "assets/objects/object_tirkin/object_tirkin.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_FRIENDLY)

void En_Tirkin_Init(Actor* thisx, PlayState* play);
void En_Tirkin_Destroy(Actor* thisx, PlayState* play);
void En_Tirkin_Update(Actor* thisx, PlayState* play);
void En_Tirkin_Draw(Actor* thisx, PlayState* play);

void En_Tirkin_SetupIdle(En_Tirkin* this);
void En_Tirkin_SetupMove(En_Tirkin* this);
void En_Tirkin_SetupAttack(En_Tirkin* this);
void En_Tirkin_AttackEnemyMoveSetup(En_Tirkin* this);
void En_Tirkin_AttackEnemyMoveThinkSetup(En_Tirkin* this); 
void En_Tirkin_HealSetup(En_Tirkin* this);
void En_Tirkin_DownSetup(En_Tirkin* this);
void En_Tirkin_DamagedSetup(En_Tirkin* this);
void En_Tirkin_CastFiresetup(En_Tirkin* this);

void En_Tirkin_Idle(En_Tirkin* this, PlayState* play);
void En_Tirkin_Move(En_Tirkin* this, PlayState* play);
void En_Tirkin_Swim(En_Tirkin* this, PlayState* play);
void En_Tirkin_Attack(En_Tirkin* this, PlayState* play);
void EnTirkin_Die(En_Tirkin* this, PlayState* play);
void EnTirkin_Damaged(En_Tirkin* this, PlayState* play);
void En_Tirkin_AttackEnemyMove(En_Tirkin* this, PlayState* play);
void En_Tirkin_AttackEnemyMoveThink(En_Tirkin* this, PlayState* play);
void En_Tirkin_Heal(En_Tirkin* this, PlayState* play);
void En_Tirkin_Down(En_Tirkin* this, PlayState* play);
void En_Tirkin_CastFire(En_Tirkin* this, PlayState* play);

void En_Tirkin_SetupFlameMove(En_Tirkin* this);
void En_Tirkin_FlameMove(En_Tirkin* this, PlayState* play);
void En_Tirkin_SetupFlameChase(En_Tirkin* this);
void En_Tirkin_FlameChase(En_Tirkin* this, PlayState* play);
void En_Tirkin_SetupFlameVanish(En_Tirkin* this);
void En_Tirkin_FlameVanish(En_Tirkin* this, PlayState* play);

ActorProfile En_Tirkin_Profile = {
    ACTOR_EN_TIRKIN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_EN_TIRKIN,
    sizeof(En_Tirkin),
    (ActorFunc)En_Tirkin_Init,
    (ActorFunc)En_Tirkin_Destroy,
    (ActorFunc)En_Tirkin_Update,
    (ActorFunc)En_Tirkin_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
         COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        ATELEM_ON,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 25, 45, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
     {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER | AT_MAGIC,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x00000001, HIT_SPECIAL_EFFECT_FIRE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 10, 25, 0, { 0, 0, 0 } },
};


void En_Tirkin_SetupAction(En_Tirkin* this, En_TirkinActionFunc actionFunc) {
    this->actionFunc = actionFunc;
};

static InitChainEntry sInitChain[] = { ICHAIN_S8(naviEnemyId, NAVI_ENEMY_DEFAULT, ICHAIN_CONTINUE),
    ICHAIN_U8(attentionRangeType, ATTENTION_RANGE_5, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3000, ICHAIN_CONTINUE) 
};

void En_Tirkin_Init(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;

    Player* player = GET_PLAYER(play);
    this->actor.world.pos.x = (Math_SinS(this->actor.yawTowardsPlayer) * -30.0f) + player->actor.world.pos.x;
    this->actor.world.pos.y = player->actor.world.pos.y + 10.0f;
    this->actor.world.pos.z = (Math_CosS(this->actor.yawTowardsPlayer) * -30.0f) + player->actor.world.pos.z;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.shape.rot = this->actor.world.rot;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    SkelAnime_InitFlex(play, &this->skelanime, &Tirkin_skel, &Tirkin_idleTirkin_idle_001Anim, this->jointTable, this->morphTable, TIRKIN_SKEL_NUM_LIMBS);
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 10.0f);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.colChkInfo.health = 30;
    this->actor.speed = 2.0f;
     this->actor.world.pos.y += 20.0f;
    this->attackCooldown = 160.0f;
    this->damagedTimer = 100;
    this->attackCooldownFire = 100.0f;
    for (int i = 0; i < ARRAY_COUNT(play->partyMembers); i++) {
        if (play->partyMembers[i] == NULL) {
            play->partyMembers[i] = &this->actor;
            play->partyMemberCount++;
            break;
        }
    }

     En_Tirkin_SetupIdle(this);
}

Actor* En_Tirkin_FindNearestEnemy(En_Tirkin* this, PlayState* play, f32 maxDist) {
    Actor* current;
    Actor* closest = NULL;
    f32 closestDist = maxDist;

    // Enemy lists we want to scan
    static u8 enemyLists[] = {
        ACTORCAT_ENEMY,
        ACTORCAT_BOSS,
    };

    for (int i = 0; i < ARRAY_COUNT(enemyLists); i++) {
        current = play->actorCtx.actorLists[enemyLists[i]].head;

        while (current != NULL) {

            if (current->update != NULL) { // must be alive
                f32 dist = Actor_WorldDistXYZToActor(&this->actor, current);

                if (dist < closestDist) {
                    closestDist = dist;
                    closest = current;
                }
            }

            current = current->next;
        }
    }

    return closest;
}

void EnTirkin_FlameCollisionCheck(En_Tirkin* this, PlayState* play) {

    Collider_UpdateCylinder(&this->actor, &this->collider_fire);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider_fire.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider_fire.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider_fire.base);
}

void En_Tirkin_InitFlame(Actor* thisx, PlayState* play) {
    
    En_Tirkin* this = (En_Tirkin*)thisx;
     Actor* enemy = En_Tirkin_FindNearestEnemy(this, play, 300.0f);

    Actor_SetScale(&this->actor, 2.0f);
    Collider_InitCylinder(play, &this->collider_fire);
    Collider_SetCylinder(play, &this->collider_fire, &this->actor, &sCylinderInit2);
    Collider_UpdateCylinder(&this->actor, &this->collider_fire);
    this->actor.minVelocityY = -0.2f;
    this->targetPos = enemy->world.pos;
    this->targetPos.y += 10.0f;
    En_Tirkin_SetupFlameMove(this);
    Actor_PlaySfx(&this->actor, NA_SE_EV_FLAME_IGNITION);
}



void EnTirkin_DownSetup(En_Tirkin* this) {
    this->actionFunc = En_Tirkin_Down;
    Animation_MorphToLoop(&this->skelanime, &Tirkin_downTirkin_downAnim, 1.0f);
}


void EnTirkin_DamagedSetup(En_Tirkin* this) {
    this->actionFunc = EnTirkin_Damaged;
    Animation_PlayOnceSetSpeed(&this->skelanime, &Tirkin_damagedTirkin_damagedAnim, 4.0f);
}
void EnTirkin_CheckDamage(En_Tirkin* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if ((this->collider.base.acFlags & AC_HIT)) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_ApplyDamage(&this->actor);
        if (this->actor.colChkInfo.health == 0) {
            EnTirkin_DownSetup(this);
            Enemy_StartFinishingBlow(play, &this->actor);
        } else {
            EnTirkin_DamagedSetup(this);
        }
    }
}

void EnTirkin_Damaged(En_Tirkin* this, PlayState* play) {
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->actor.speed = 0.0f;
    if (SkelAnime_Update(&this->skelanime)) {
    En_Tirkin_SetupMove(this);
    };
}

void En_Tirkin_Down(En_Tirkin* this, PlayState* play) {
    this->actor.speed = 0.0f;
    if (this->damagedTimer = 0) {
    En_Tirkin_SetupMove(this);
    this->actor.colChkInfo.health = 30;
    this->damagedTimer = 100;
    };
}


void En_Tirkin_Destroy(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;
    Collider_DestroyCylinder(play, &this->collider);
}

void En_Tirkin_SetupIdle(En_Tirkin* this) {
    this->actionFunc = En_Tirkin_Idle;
     Animation_MorphToLoop(&this->skelanime, &Tirkin_idleTirkin_idle_001Anim, 1.0f);
     this->actor.speed = 0.0f;
}

void En_Tirkin_SetupMove(En_Tirkin* this) {
     Animation_PlayLoopSetSpeed(&this->skelanime, &Tirkin_runTirkin_runAnim, 1.0f);
     this->actionFunc = En_Tirkin_Move;
     this->actor.speed = 2.0f;
}

void En_Tirkin_SetupFlameMove(En_Tirkin* this) {
    /* Start the flame moving toward targetPos */
    this->actionFunc = En_Tirkin_FlameMove;
    this->speedMod = 3.0f; /* units per frame, adjust as needed */
    /* Face the target immediately */
    this->actor.world.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &this->targetPos);
    this->actor.shape.rot = this->actor.world.rot;
}

void En_Tirkin_SetupFlameVanish(En_Tirkin* this) {
    this->actionFunc = En_Tirkin_FlameVanish;
}


void En_Tirkin_FlameMove(En_Tirkin* this, PlayState* play) {
    Vec3f dir;
    f32 dist;
    /* Compute vector from flame to target */
    dir.x = this->targetPos.x - this->actor.world.pos.x;
    dir.y = this->targetPos.y - this->actor.world.pos.y;
    dir.z = this->targetPos.z - this->actor.world.pos.z;
    dist = sqrtf((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z));

    /* If we've reached the target (or are very close), vanish */
    if (dist <= 10.0f || dist == 0.0f) {
        En_Tirkin_SetupFlameVanish(this);
        return;
    }

    dir.x /= dist;
    dir.y /= dist;
    dir.z /= dist;

    this->actor.velocity.x = dir.x * this->speedMod;
    this->actor.velocity.y = dir.y * this->speedMod;
    this->actor.velocity.z = dir.z * this->speedMod;

    this->actor.world.pos.x += this->actor.velocity.x;
    this->actor.world.pos.y += this->actor.velocity.y;
    this->actor.world.pos.z += this->actor.velocity.z;

    this->actor.world.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &this->targetPos);
    this->actor.shape.rot = this->actor.world.rot;

    EnTirkin_FlameCollisionCheck(this, play);

    /* If collision with an actor or environment caused the flame to be "consumed",
       the collision callbacks / flags should handle changing state; additionally,
       vanish if we pass the target or go out of bounds */
    if (this->actor.world.pos.y < -500.0f || this->actor.world.pos.y > 2000.0f) {
        En_Tirkin_SetupFlameVanish(this);
    }

    /* Check if flame touched an enemy and vanish */
    if (this->collider_fire.base.atFlags & AT_HIT) {
        En_Tirkin_SetupFlameVanish(this);
    }
}

void En_Tirkin_FlameVanish(En_Tirkin* this, PlayState* play) {

    Actor_MoveXZGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 7.0f, 10.0f, 0.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
        Actor_Kill(&this->actor);
}




void En_Tirkin_Idle(En_Tirkin* this, PlayState* play) {
     SkelAnime_Update(&this->skelanime);
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.shape.rot = this->actor.world.rot;
    this->actor.speed = 0.0f;
    if ((!(this->actor.xzDistToPlayer > 150.0f))  && !(this->actor.xzDistToPlayer < 50.0f)) {
         En_Tirkin_SetupMove(this);
    }  else if (!(this->actor.xzDistToPlayer > 500.0f) && !(this->actor.xzDistToPlayer < 150.0f)) {
         En_Tirkin_SetupMove(this);
    }
}

void En_Tirkin_AttackEnemyMoveSetup(En_Tirkin* this) {
    Animation_MorphToLoop(&this->skelanime, &Tirkin_runTirkin_runAnim, 1.0f);
    this->actionFunc = En_Tirkin_AttackEnemyMove;
    this->actor.speed = 4.0f;
}


void En_Tirkin_HealSetup(En_Tirkin* this) {
    // Play a healing animation (reuse the "think" anim or replace with a dedicated heal anim if available)
    Animation_PlayOnce(&this->skelanime, &Tirkin_castTirkin_castAnim);
    this->actionFunc = En_Tirkin_Heal;
    this->actor.speed = 0.0f;  // Stop moving during heal
}

void En_Tirkin_CastFiresetup(En_Tirkin* this) {
    Animation_PlayOnce(&this->skelanime, &Tirkin_castTirkin_castAnim);
    this->actionFunc = En_Tirkin_CastFire;
    this->actor.speed = 0.0f;  // Stop moving during cast
}

void En_Tirkin_AttackEnemyMove(En_Tirkin* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    Actor* enemy = En_Tirkin_FindNearestEnemy(this, play, 300.0f);
    Actor* enemy2 = En_Tirkin_FindNearestEnemy(this, play, 100.0f);
    if (enemy != NULL && enemy->destroy == NULL) {
    enemy = NULL;
    };
     if (enemy2 != NULL && enemy->destroy == NULL) {
    enemy = NULL;
    };

    if (enemy != NULL && enemy->destroy != NULL) {
        s16 targetYaw = Actor_WorldYawTowardActor(&this->actor, enemy);
    Math_SmoothStepToS(&this->actor.world.rot.y, targetYaw, 5, 1000, 1);
    this->actor.shape.rot.y = this->actor.world.rot.y;

    this->actor.speed = 4.0f;
    };

    // If an enemy is out of range, return to normal movement
    if (enemy == NULL) {
        En_Tirkin_SetupMove(this);
        return;
    }
    if (enemy2 != NULL) {
        En_Tirkin_AttackEnemyMoveThinkSetup(this);
        return;
    }
}

void En_Tirkin_AttackEnemyMoveThinkSetup(En_Tirkin* this) {
    this->actionFunc = En_Tirkin_AttackEnemyMoveThink;
    Animation_MorphToLoop(&this->skelanime, &Tirkin_thinkTirkin_think_001Anim, 2.0f);
}

void En_Tirkin_AttackEnemyMoveThink(En_Tirkin* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    Actor* enemy = En_Tirkin_FindNearestEnemy(this, play, 100.0f);
    if (enemy != NULL && enemy->destroy == NULL) {
    enemy = NULL;
    };
    if (enemy != NULL && enemy->destroy != NULL) {
        s16 targetYaw = Actor_WorldYawTowardActor(&this->actor, enemy);
    Math_SmoothStepToS(&this->actor.world.rot.y, targetYaw, 5, 1000, 1);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if (this->attackCooldown == 0) {
        En_Tirkin_HealSetup(this);
    }   
    this->actor.speed = 0.4f;
    };

    // If an enemy is out of range, return to normal movement
    if (enemy == NULL) {
        En_Tirkin_AttackEnemyMoveSetup(this);
        return;
    }
}

void En_Tirkin_Heal(En_Tirkin* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    
    if (SkelAnime_Update(&this->skelanime)) {  // Animation finished
        // Heal all party members (including the Tirkin himself)
        for (int i = 0; i < play->partyMemberCount; i++) {
            if (play->partyMembers[i] != NULL) {
                Actor* member = play->partyMembers[i];
                member->colChkInfo.health += 10;  // Heal amount (adjust as needed)
                // Clamp to max health (assuming 30 is max for party members; adjust based on your actor's max)
                if (member->colChkInfo.health > 30) {
                    member->colChkInfo.health = 30;
                }
            }
        }
        
        // Heal the player separately (player health is in save context, not actor health)
        Player* player = GET_PLAYER(play);
        gSaveContext.save.info.playerData.health += 10;  // Heal 1 heart (4 quarters; adjust as needed)
        // Clamp to max health capacity
        if (gSaveContext.save.info.playerData.health > gSaveContext.save.info.playerData.healthCapacity) {
            gSaveContext.save.info.playerData.health = gSaveContext.save.info.playerData.healthCapacity;
        }
        
        // Reset cooldown and return to movement
        this->attackCooldown = 300.0f;  // Reset cooldown (adjust duration as needed)
        En_Tirkin_SetupMove(this);
    }
}

void En_Tirkin_CastFire(En_Tirkin* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    
    if (SkelAnime_Update(&this->skelanime)) {  // Animation finished
        // Spawn flame actor
        
        this->attackCooldown = 200.0f;  // Reset cooldown (adjust duration as needed)
         En_Tirkin_InitFlame(&this->actor, play);
        En_Tirkin_SetupMove(this);
    }
}

void En_Tirkin_Move(En_Tirkin* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    if ((!(this->actor.xzDistToPlayer > 150.0f) && !(this->actor.xzDistToPlayer < 50.0f)) ||
               (!(this->actor.yDistToPlayer > 500.0f) && !(this->actor.yDistToPlayer < 100.0f))) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot = this->actor.world.rot;
        this->actor.speed = 3.0f;
         if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
    // Slide along the wall
    s16 wallYaw = this->actor.wallYaw;
    this->actor.world.rot.y = wallYaw + 0x4000;  // angle perpendicular to wall
    this->actor.shape.rot.y = this->actor.world.rot.y;
    return;
}
    } else if ((!(this->actor.xzDistToPlayer > 500.0f) && !(this->actor.xzDistToPlayer < 150.0f)) ||
               (!(this->actor.yDistToPlayer > 500.0f) && !(this->actor.yDistToPlayer < 100.0f)))  {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot = this->actor.world.rot;
        this->actor.speed = 5.0f;
         if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
    // Slide along the wall
    s16 wallYaw = this->actor.wallYaw;
    this->actor.world.rot.y = wallYaw + 0x4000;  // angle perpendicular to wall
    this->actor.shape.rot.y = this->actor.world.rot.y;
    return;
}
    } else {
        this->actionFunc = En_Tirkin_Idle;
    }
    Actor* enemy = En_Tirkin_FindNearestEnemy(this, play, 300.0f);

    // If an enemy is within 300 units, start combat mode
    if (enemy != NULL ) {
        En_Tirkin_AttackEnemyMoveSetup(this);
        return;
    }
}



void En_Tirkin_Update(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;

    if ((this->damagedTimer != 0) && (this->actionFunc == En_Tirkin_Down)) {
        this->damagedTimer--;
    }
    if (this->actionFunc == En_Tirkin_AttackEnemyMoveThink) {
    if (this->attackCooldown != 0) {
        this->attackCooldown--;
        this->attackCooldownFire--;
    }
};

    EnTirkin_CheckDamage(this, play);
    this->actionFunc(this, play);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    Collider_UpdateCylinder(&this->actor, &this->collider_fire);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider_fire.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider_fire.base);
   Actor_UpdateBgCheckInfo(play, &this->actor, this->collider.dim.radius, this->collider.dim.height * 0.5f, 0.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
     Actor_MoveXZGravity(&this->actor);

    this->actionFunc(this, play);
}

void En_Tirkin_DrawFlame(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_tirkin.c", 999);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    /* simple coloring like Honotrap flame */
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 200, 0, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

    /* orient quad toward camera like other flame effects */
    Matrix_RotateY(BINANG_TO_RAD((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)) - this->actor.shape.rot.y + 0x8000)),
                   MTXMODE_APPLY);

    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx, "../z_en_tirkin.c", 1010);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_tirkin.c", 1014);
}

void Tirkin_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    En_Tirkin* this = (En_Tirkin*)thisx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
}

void En_Tirkin_Draw(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;
     Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelanime.skeleton, this->skelanime.jointTable, this->skelanime.dListCount,
                          NULL,  Tirkin_PostLimbDraw, this);
}
