/*
 * File: z_en_caterpillar.c
 * Overlay: ovl_En_Caterpillar
 * Description: Custom Actor
 */

#include "z_en_caterpillar.h"

#include "play_state.h"

#include "assets/objects/object_en_caterpillar/object_en_caterpillar.h"

#define FLAGS (0)

void En_Caterpillar_Init(Actor* thisx, PlayState* play);
void En_Caterpillar_Destroy(Actor* thisx, PlayState* play);
void En_Caterpillar_Update(Actor* thisx, PlayState* play);
void En_Caterpillar_Draw(Actor* thisx, PlayState* play);

void En_Caterpillar_DoNothing(En_Caterpillar* this, PlayState* play);

ActorProfile En_Caterpillar_Profile = {
    ACTOR_EN_CATERPILLAR,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_CATERPILLAR,
    sizeof(En_Caterpillar),
    (ActorFunc)En_Caterpillar_Init,
    (ActorFunc)En_Caterpillar_Destroy,
    (ActorFunc)En_Caterpillar_Update,
    (ActorFunc)En_Caterpillar_Draw,
};

void En_Caterpillar_Init(Actor* thisx, PlayState* play) {
    En_Caterpillar* this = (En_Caterpillar*)thisx;

    this->actionFunc = En_Caterpillar_DoNothing;
}

void En_Caterpillar_Destroy(Actor* thisx, PlayState* play) {
    En_Caterpillar* this = (En_Caterpillar*)thisx;
}

void En_Caterpillar_Update(Actor* thisx, PlayState* play) {
    En_Caterpillar* this = (En_Caterpillar*)thisx;

    this->actionFunc(this, play);
}

void En_Caterpillar_Draw(Actor* thisx, PlayState* play) {
    En_Caterpillar* this = (En_Caterpillar*)thisx;
}

void En_Caterpillar_DoNothing(En_Caterpillar* this, PlayState* play) {

}
