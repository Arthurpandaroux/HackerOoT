/*
 * File: z_en_ogre.c
 * Overlay: ovl_En_Ogre
 * Description: Custom Actor
 */

#include "z_en_ogre.h"

#include "play_state.h"

#include "assets/objects/object_en_ogre/object_en_ogre.h"

#define FLAGS (0)

void En_Ogre_Init(Actor* thisx, PlayState* play);
void En_Ogre_Destroy(Actor* thisx, PlayState* play);
void En_Ogre_Update(Actor* thisx, PlayState* play);
void En_Ogre_Draw(Actor* thisx, PlayState* play);

void En_Ogre_DoNothing(En_Ogre* this, PlayState* play);

ActorProfile En_Ogre_Profile = {
    ACTOR_EN_OGRE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_OGRE,
    sizeof(En_Ogre),
    (ActorFunc)En_Ogre_Init,
    (ActorFunc)En_Ogre_Destroy,
    (ActorFunc)En_Ogre_Update,
    (ActorFunc)En_Ogre_Draw,
};

void En_Ogre_Init(Actor* thisx, PlayState* play) {
    En_Ogre* this = (En_Ogre*)thisx;

    this->actionFunc = En_Ogre_DoNothing;
}

void En_Ogre_Destroy(Actor* thisx, PlayState* play) {
    En_Ogre* this = (En_Ogre*)thisx;
}

void En_Ogre_Update(Actor* thisx, PlayState* play) {
    En_Ogre* this = (En_Ogre*)thisx;

    this->actionFunc(this, play);
}

void En_Ogre_Draw(Actor* thisx, PlayState* play) {
    En_Ogre* this = (En_Ogre*)thisx;
}

void En_Ogre_DoNothing(En_Ogre* this, PlayState* play) {

}
