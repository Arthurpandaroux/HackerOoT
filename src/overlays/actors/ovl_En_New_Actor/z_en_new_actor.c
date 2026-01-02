/*
 * File: z_en_new_actor.c
 * Overlay: ovl_En_New_Actor
 * Description: Custom Actor
 */

#include "z_en_new_actor.h"

#include "play_state.h"

#include "assets/objects/object_en_new_actor/object_en_new_actor.h"

#define FLAGS (0)

void En_New_Actor_Init(Actor* thisx, PlayState* play);
void En_New_Actor_Destroy(Actor* thisx, PlayState* play);
void En_New_Actor_Update(Actor* thisx, PlayState* play);
void En_New_Actor_Draw(Actor* thisx, PlayState* play);

void En_New_Actor_DoNothing(En_New_Actor* this, PlayState* play);

ActorProfile En_New_Actor_Profile = {
    ACTOR_EN_NEW_ACTOR,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_NEW_ACTOR,
    sizeof(En_New_Actor),
    (ActorFunc)En_New_Actor_Init,
    (ActorFunc)En_New_Actor_Destroy,
    (ActorFunc)En_New_Actor_Update,
    (ActorFunc)En_New_Actor_Draw,
};

void En_New_Actor_Init(Actor* thisx, PlayState* play) {
    En_New_Actor* this = (En_New_Actor*)thisx;

    this->actionFunc = En_New_Actor_DoNothing;
}

void En_New_Actor_Destroy(Actor* thisx, PlayState* play) {
    En_New_Actor* this = (En_New_Actor*)thisx;
}

void En_New_Actor_Update(Actor* thisx, PlayState* play) {
    En_New_Actor* this = (En_New_Actor*)thisx;

    this->actionFunc(this, play);
}

void En_New_Actor_Draw(Actor* thisx, PlayState* play) {
    En_New_Actor* this = (En_New_Actor*)thisx;
}

void En_New_Actor_DoNothing(En_New_Actor* this, PlayState* play) {

}
