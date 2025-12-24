/*
 * File: z_en_tirkin.c
 * Overlay: ovl_En_Tirkin
 * Description: Custom Actor
 */

#include "z_en_tirkin.h"

#include "play_state.h"

#include "assets/objects/object_en_tirkin/object_en_tirkin.h"

#define FLAGS (0)

void En_Tirkin_Init(Actor* thisx, PlayState* play);
void En_Tirkin_Destroy(Actor* thisx, PlayState* play);
void En_Tirkin_Update(Actor* thisx, PlayState* play);
void En_Tirkin_Draw(Actor* thisx, PlayState* play);

void En_Tirkin_DoNothing(En_Tirkin* this, PlayState* play);

ActorProfile En_Tirkin_Profile = {
    ACTOR_EN_TIRKIN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_TIRKIN,
    sizeof(En_Tirkin),
    (ActorFunc)En_Tirkin_Init,
    (ActorFunc)En_Tirkin_Destroy,
    (ActorFunc)En_Tirkin_Update,
    (ActorFunc)En_Tirkin_Draw,
};

void En_Tirkin_Init(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;

    this->actionFunc = En_Tirkin_DoNothing;
}

void En_Tirkin_Destroy(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;
}

void En_Tirkin_Update(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;

    this->actionFunc(this, play);
}

void En_Tirkin_Draw(Actor* thisx, PlayState* play) {
    En_Tirkin* this = (En_Tirkin*)thisx;
}

void En_Tirkin_DoNothing(En_Tirkin* this, PlayState* play) {

}
