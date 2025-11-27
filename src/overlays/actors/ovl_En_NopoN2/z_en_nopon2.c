/*
 * File: z_en_nopon2.c
 * Overlay: ovl_En_NopoN2
 * Description: Custom Actor
 */

#include "z_en_nopon2.h"

#include "play_state.h"

#include "assets/objects/object_en_nopon2/object_en_nopon2.h"

#define FLAGS (0)

void En_NopoN2_Init(Actor* thisx, PlayState* play);
void En_NopoN2_Destroy(Actor* thisx, PlayState* play);
void En_NopoN2_Update(Actor* thisx, PlayState* play);
void En_NopoN2_Draw(Actor* thisx, PlayState* play);

void En_NopoN2_DoNothing(En_NopoN2* this, PlayState* play);

ActorProfile En_NopoN2_Profile = {
    ACTOR_EN_NOPON2,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_NOPON2,
    sizeof(En_NopoN2),
    (ActorFunc)En_NopoN2_Init,
    (ActorFunc)En_NopoN2_Destroy,
    (ActorFunc)En_NopoN2_Update,
    (ActorFunc)En_NopoN2_Draw,
};

void En_NopoN2_Init(Actor* thisx, PlayState* play) {
    En_NopoN2* this = (En_NopoN2*)thisx;

    this->actionFunc = En_NopoN2_DoNothing;
}

void En_NopoN2_Destroy(Actor* thisx, PlayState* play) {
    En_NopoN2* this = (En_NopoN2*)thisx;
}

void En_NopoN2_Update(Actor* thisx, PlayState* play) {
    En_NopoN2* this = (En_NopoN2*)thisx;

    this->actionFunc(this, play);
}

void En_NopoN2_Draw(Actor* thisx, PlayState* play) {
    En_NopoN2* this = (En_NopoN2*)thisx;
}

void En_NopoN2_DoNothing(En_NopoN2* this, PlayState* play) {

}
