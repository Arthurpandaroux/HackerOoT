/*
 * File: z_en_nopon.c
 * Overlay: ovl_En_Nopon
 * Description: Custom Actor
 */

#include "z_en_nopon.h"

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
#include "play_state.h"
#include "player.h"
#include "collision_check.h"

#include "assets/objects/object_en_nopon/object_en_nopon.h"


#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_FRIENDLY)

void En_Nopon_Init(Actor* thisx, PlayState* play);
void En_Nopon_Destroy(Actor* thisx, PlayState* play);
void En_Nopon_Update(Actor* thisx, PlayState* play);
void En_Nopon_Draw(Actor* thisx, PlayState* play);

void En_Nopon_SetupIdle(En_Nopon* this);
void En_Nopon_SetupMove(En_Nopon* this);
void En_Nopon_SetupSwim(En_Nopon* this);
void En_Nopon_SetupAttack(En_Nopon* this);
void En_Nopon_AttackEnemyMoveSetup(En_Nopon* this);
void En_Nopon_AttackNormalSetup(En_Nopon* this);
void En_Nopon_AttackEnemyMoveThinkSetup(En_Nopon* this);

void En_Nopon_Idle(En_Nopon* this, PlayState* play);
void En_Nopon_Move(En_Nopon* this, PlayState* play);
void En_Nopon_Swim(En_Nopon* this, PlayState* play);
void En_Nopon_Attack(En_Nopon* this, PlayState* play);
void EnNopon_Die(En_Nopon* this, PlayState* play);
void EnNopon_Damaged(En_Nopon* this, PlayState* play);
void En_Nopon_AttackEnemyMove(En_Nopon* this, PlayState* play);
void En_Nopon_AttackNormal(En_Nopon* this, PlayState* play);
void En_Nopon_AttackEnemyMoveThink(En_Nopon* this, PlayState* play);

ActorProfile En_Nopon_Profile = {
    ACTOR_EN_NOPON,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_EN_NOPON,
    sizeof(En_Nopon),
    (ActorFunc)En_Nopon_Init,
    (ActorFunc)En_Nopon_Destroy,
    (ActorFunc)En_Nopon_Update,
    (ActorFunc)En_Nopon_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
         COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
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
    { 20, 40, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
     {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x00000001, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 30, 30, 0, { 0, 0, 0 } },
};




static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

void EnNopon_SetupAction(En_Nopon* this, En_NoponActionFunc actionFunc) {
    this->actionFunc = actionFunc;
};

static InitChainEntry sInitChain[] = { ICHAIN_S8(naviEnemyId, NAVI_ENEMY_DEFAULT, ICHAIN_CONTINUE),
    ICHAIN_U8(attentionRangeType, ATTENTION_RANGE_5, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3000, ICHAIN_CONTINUE) 
};



s16 Attackx;
    s16 Attacky;
    s16 Attackz;

void En_Nopon_Init(Actor* thisx, PlayState* play) {
    En_Nopon* this = (En_Nopon*)thisx;
    Player* player = GET_PLAYER(play);
    this->actor.world.pos.x = (Math_SinS(this->actor.yawTowardsPlayer) * -30.0f) + player->actor.world.pos.x;
    this->actor.world.pos.y = player->actor.world.pos.y + 10.0f;
    this->actor.world.pos.z = (Math_CosS(this->actor.yawTowardsPlayer) * -30.0f) + player->actor.world.pos.z;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.shape.rot = this->actor.world.rot;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    SkelAnime_InitFlex(play, &this->skelanime, &nopon_skel, &gnopon_idle_animGnopon_idle_animAnim, this->jointTable, this->morphTable, NOPON_SKEL_NUM_LIMBS);
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 10.0f);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.colChkInfo.health = 30;
    En_Nopon_SetupIdle(this);
    this->actor.speed = 2.0f;
     this->actor.world.pos.y += 20.0f;
    Collider_InitCylinder(play, &this->collider_Attack1);
    Collider_SetCylinder(play, &this->collider_Attack1, &this->actor, &sCylinderInit2);
    this->collider_Attack1.elem.atDmgInfo.dmgFlags = DMG_SWORD;
    this->collider.elem.atDmgInfo.dmgFlags = DMG_SWORD;
    this->attackCooldown = 100.0f;
    for (int i = 0; i < ARRAY_COUNT(play->partyMembers); i++) {
        if (play->partyMembers[i] == NULL) {
            play->partyMembers[i] = &this->actor;
            play->partyMemberCount++;
            break;
        }
    }
        }




Actor* En_Nopon_FindNearestEnemy(En_Nopon* this, PlayState* play, f32 maxDist) {
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

void EnNopon_DamagedSetup(En_Nopon* this) {
    this->actionFunc = EnNopon_Damaged;
    Animation_PlayOnceSetSpeed(&this->skelanime, &nopon_damaged_animNopon_damaged_animAnim, 4.0f);
}
void EnNopon_CheckDamage(En_Nopon* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if ((this->collider.base.acFlags & AC_HIT)) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_ApplyDamage(&this->actor);
        if (this->actor.colChkInfo.health == 0) {
            EnNopon_DamagedSetup(this);
            Enemy_StartFinishingBlow(play, &this->actor);
        } else {
            EnNopon_DamagedSetup(this);
        }
    }
}

void EnNopon_Damaged(En_Nopon* this, PlayState* play) {
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->actor.speed = 0.0f;
    if (SkelAnime_Update(&this->skelanime)) {
    En_Nopon_SetupMove(this);
    };
}

void En_Nopon_Destroy(Actor* thisx, PlayState* play) {
    En_Nopon* this = (En_Nopon*)thisx;
    Collider_DestroyCylinder(play, &this->collider);
    Collider_DestroyCylinder(play, &this->collider_Attack1);
}



void En_Nopon_SetupIdle(En_Nopon* this) {
    this->actionFunc = En_Nopon_Idle;
     Animation_MorphToLoop(&this->skelanime, &gnopon_idle_animGnopon_idle_animAnim, 4.0f);
    this->actor.speed = 0.0f;
}

void En_Nopon_SetupMove(En_Nopon* this) {
     Animation_PlayLoopSetSpeed(&this->skelanime, &gnopon_walk_animGnopon_walkAnim, 1.1f);
     this->actionFunc = En_Nopon_Move;
     this->actor.speed = 2.0f;
}

void En_Nopon_Idle(En_Nopon* this, PlayState* play) {
     SkelAnime_Update(&this->skelanime);
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.shape.rot = this->actor.world.rot;
    this->actor.speed = 0.0f;
    if ((!(this->actor.xzDistToPlayer > 150.0f))  && !(this->actor.xzDistToPlayer < 50.0f)) {
         En_Nopon_SetupMove(this);
    }  else if (!(this->actor.xzDistToPlayer > 500.0f) && !(this->actor.xzDistToPlayer < 150.0f)) {
         En_Nopon_SetupMove(this);
    }
}


void En_Nopon_AttackEnemyMoveSetup(En_Nopon* this) {
    Animation_MorphToLoop(&this->skelanime, &gnopon_walk_animGnopon_walkAnim, 1.0f);
    this->actionFunc = En_Nopon_AttackEnemyMove;
    this->actor.speed = 4.0f;
}

void En_Nopon_AttackNormalSetup(En_Nopon* this) {
    Animation_PlayOnce(&this->skelanime, &nopon_attack_animNopon_thinking_animAnim);
    this->actionFunc = En_Nopon_AttackNormal;
    this->actor.speed = 4.0f;
}

void En_Nopon_AttackEnemyMove(En_Nopon* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    Actor* enemy = En_Nopon_FindNearestEnemy(this, play, 300.0f);
    Actor* enemy2 = En_Nopon_FindNearestEnemy(this, play, 100.0f);
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
        En_Nopon_SetupMove(this);
        return;
    }
    if (enemy2 != NULL) {
        En_Nopon_AttackEnemyMoveThinkSetup(this);
        return;
    }
}

void En_Nopon_AttackNormal(En_Nopon* this, PlayState* play) {
    Actor* enemy = En_Nopon_FindNearestEnemy(this, play, 200.0f);
    this->actor.speed = 3.5f;
    if (enemy != NULL && enemy->destroy == NULL) {
    enemy = NULL;
    };
    f32 distInFront = 20.0f;
Vec3f colliderPos;
if (enemy != NULL && enemy->destroy != NULL) {
    s16 targetYaw = Actor_WorldYawTowardActor(&this->actor, enemy);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    this->actor.speed = 3.5f;
    Math_Vec3f_Copy(&colliderPos, &this->actor.world.pos);
    Attackx += distInFront * Math_SinS(this->actor.shape.rot.y); 
    Attackz += distInFront * Math_CosS(this->actor.shape.rot.y);
    Attacky += 10.0f;
};
    if (SkelAnime_Update(&this->skelanime)) {
        En_Nopon_AttackEnemyMoveSetup(this);
        this->attackCooldown = 100.0f;
    };
    this->collider_Attack1.dim.pos.x = Attackx;
    this->collider_Attack1.dim.pos.y = Attacky;
    this->collider_Attack1.dim.pos.z = Attackz;
}

void En_Nopon_AttackEnemyMoveThinkSetup(En_Nopon* this) {
    this->actionFunc = En_Nopon_AttackEnemyMoveThink;
    Animation_MorphToLoop(&this->skelanime, &nopon_thinking_animNopon_thinkingAnim, 2.0f);
}

void En_Nopon_AttackEnemyMoveThink(En_Nopon* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    Actor* enemy = En_Nopon_FindNearestEnemy(this, play, 100.0f);
    if (enemy != NULL && enemy->destroy == NULL) {
    enemy = NULL;
    };
    if (enemy != NULL && enemy->destroy != NULL) {
        s16 targetYaw = Actor_WorldYawTowardActor(&this->actor, enemy);
    Math_SmoothStepToS(&this->actor.world.rot.y, targetYaw, 5, 1000, 1);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if (this->attackCooldown == 0) {
        En_Nopon_AttackNormalSetup(this);
    }   
    this->actor.speed = 0.4f;
    };

    // If an enemy is out of range, return to normal movement
    if (enemy == NULL) {
        En_Nopon_AttackEnemyMoveSetup(this);
        return;
    }
}


void En_Nopon_Move(En_Nopon* this, PlayState* play) {
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
        this->actionFunc = En_Nopon_Idle;
    }
    Actor* enemy = En_Nopon_FindNearestEnemy(this, play, 300.0f);

    // If an enemy is within 300 units, start combat mode
    if (enemy != NULL ) {
        En_Nopon_AttackEnemyMoveSetup(this);
        return;
    }
}

void En_Nopon_Update(Actor* thisx, PlayState* play) {
    En_Nopon* this = (En_Nopon*)thisx;
        if (this->damagedTimer != 0) {
        this->damagedTimer--;
    }
    if (this->actionFunc == En_Nopon_AttackEnemyMoveThink) {
    if (this->attackCooldown != 0) {
        this->attackCooldown--;
    }
};

    EnNopon_CheckDamage(this, play);
    this->actionFunc(this, play);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    Collider_UpdateCylinder(&this->actor, &this->collider_Attack1);
    if (this->actionFunc != En_Nopon_AttackNormal) {
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    if (this->actionFunc == En_Nopon_AttackNormal) {
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider_Attack1.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    };
   Actor_UpdateBgCheckInfo(play, &this->actor, this->collider.dim.radius, this->collider.dim.height * 0.5f, 0.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
     Actor_UpdateBgCheckInfo(play, &this->actor, this->collider_Attack1.dim.radius, this->collider_Attack1.dim.height * 0.5f, 0.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
     Actor_MoveXZGravity(&this->actor);
     this->collider_Attack1.dim.pos.x = Attackx;
this->collider_Attack1.dim.pos.y =  Attacky;
this->collider_Attack1.dim.pos.z = Attackz;

}


void Nopon_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    En_Nopon* this = (En_Nopon*)thisx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
}

void En_Nopon_Draw(Actor* thisx, PlayState* play) {
    En_Nopon* this = (En_Nopon*)thisx;
     Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelanime.skeleton, this->skelanime.jointTable, this->skelanime.dListCount,
                          NULL,  Nopon_PostLimbDraw, this);
}