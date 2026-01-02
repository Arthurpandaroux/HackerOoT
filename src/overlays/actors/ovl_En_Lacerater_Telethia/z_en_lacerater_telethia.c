/*
 * File: z_en_lacerater_telethia.c
 * Overlay: ovl_En_Lacerater_Telethia
 * Description: Custom Actor
 */

#include "z_en_lacerater_telethia.h"

#include "play_state.h"

#include "assets/objects/object_en_lacerater_telethia/object_en_lacerater_telethia.h"

#define FLAGS (0)

void En_Lacerater_Telethia_Init(Actor* thisx, PlayState* play);
void En_Lacerater_Telethia_Destroy(Actor* thisx, PlayState* play);
void En_Lacerater_Telethia_Update(Actor* thisx, PlayState* play);
void En_Lacerater_Telethia_Draw(Actor* thisx, PlayState* play);

void En_Lacerater_Telethia_DoNothing(En_Lacerater_Telethia* this, PlayState* play);

ActorProfile En_Lacerater_Telethia_Profile = {
    ACTOR_EN_LACERATER_TELETHIA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_LACERATER_TELETHIA,
    sizeof(En_Lacerater_Telethia),
    (ActorFunc)En_Lacerater_Telethia_Init,
    (ActorFunc)En_Lacerater_Telethia_Destroy,
    (ActorFunc)En_Lacerater_Telethia_Update,
    (ActorFunc)En_Lacerater_Telethia_Draw,
};

void En_Lacerater_Telethia_Init(Actor* thisx, PlayState* play) {
    En_Lacerater_Telethia* this = (En_Lacerater_Telethia*)thisx;

    this->actionFunc = En_Lacerater_Telethia_DoNothing;
}

void En_Lacerater_Telethia_Destroy(Actor* thisx, PlayState* play) {
    En_Lacerater_Telethia* this = (En_Lacerater_Telethia*)thisx;
}

void En_Lacerater_Telethia_Update(Actor* thisx, PlayState* play) {
    En_Lacerater_Telethia* this = (En_Lacerater_Telethia*)thisx;

    this->actionFunc(this, play);
}

void En_Lacerater_Telethia_Draw(Actor* thisx, PlayState* play) {
    En_Lacerater_Telethia* this = (En_Lacerater_Telethia*)thisx;
}

void En_Lacerater_Telethia_DoNothing(En_Lacerater_Telethia* this, PlayState* play) {

}
