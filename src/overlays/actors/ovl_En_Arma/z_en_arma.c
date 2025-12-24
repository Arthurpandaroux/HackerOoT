/*
 * File: z_en_arma.c
 * Overlay: ovl_En_Arma
 * Description: Custom Actor
 */

#include "z_en_arma.h"

#include "play_state.h"

#include "assets/objects/object_en_arma/object_en_arma.h"

#define FLAGS (0)

void En_Arma_Init(Actor* thisx, PlayState* play);
void En_Arma_Destroy(Actor* thisx, PlayState* play);
void En_Arma_Update(Actor* thisx, PlayState* play);
void En_Arma_Draw(Actor* thisx, PlayState* play);

void En_Arma_DoNothing(En_Arma* this, PlayState* play);

ActorProfile En_Arma_Profile = {
    ACTOR_EN_ARMA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_ARMA,
    sizeof(En_Arma),
    (ActorFunc)En_Arma_Init,
    (ActorFunc)En_Arma_Destroy,
    (ActorFunc)En_Arma_Update,
    (ActorFunc)En_Arma_Draw,
};

void En_Arma_Init(Actor* thisx, PlayState* play) {
    En_Arma* this = (En_Arma*)thisx;

    this->actionFunc = En_Arma_DoNothing;
}

void En_Arma_Destroy(Actor* thisx, PlayState* play) {
    En_Arma* this = (En_Arma*)thisx;
}

void En_Arma_Update(Actor* thisx, PlayState* play) {
    En_Arma* this = (En_Arma*)thisx;

    this->actionFunc(this, play);
}

void En_Arma_Draw(Actor* thisx, PlayState* play) {
    En_Arma* this = (En_Arma*)thisx;
}

void En_Arma_DoNothing(En_Arma* this, PlayState* play) {

}
