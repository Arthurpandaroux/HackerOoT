/*
 * File: z_en_spider_telethia.c
 * Overlay: ovl_En_Spider_Telethia
 * Description: Custom Actor
 */

#include "z_en_spider_telethia.h"

#include "play_state.h"

#include "assets/objects/object_en_spider_telethia/object_en_spider_telethia.h"

#define FLAGS (0)

void En_Spider_Telethia_Init(Actor* thisx, PlayState* play);
void En_Spider_Telethia_Destroy(Actor* thisx, PlayState* play);
void En_Spider_Telethia_Update(Actor* thisx, PlayState* play);
void En_Spider_Telethia_Draw(Actor* thisx, PlayState* play);

void En_Spider_Telethia_DoNothing(En_Spider_Telethia* this, PlayState* play);

ActorProfile En_Spider_Telethia_Profile = {
    ACTOR_EN_SPIDER_TELETHIA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EN_SPIDER_TELETHIA,
    sizeof(En_Spider_Telethia),
    (ActorFunc)En_Spider_Telethia_Init,
    (ActorFunc)En_Spider_Telethia_Destroy,
    (ActorFunc)En_Spider_Telethia_Update,
    (ActorFunc)En_Spider_Telethia_Draw,
};

void En_Spider_Telethia_Init(Actor* thisx, PlayState* play) {
    En_Spider_Telethia* this = (En_Spider_Telethia*)thisx;

    this->actionFunc = En_Spider_Telethia_DoNothing;
}

void En_Spider_Telethia_Destroy(Actor* thisx, PlayState* play) {
    En_Spider_Telethia* this = (En_Spider_Telethia*)thisx;
}

void En_Spider_Telethia_Update(Actor* thisx, PlayState* play) {
    En_Spider_Telethia* this = (En_Spider_Telethia*)thisx;

    this->actionFunc(this, play);
}

void En_Spider_Telethia_Draw(Actor* thisx, PlayState* play) {
    En_Spider_Telethia* this = (En_Spider_Telethia*)thisx;
}

void En_Spider_Telethia_DoNothing(En_Spider_Telethia* this, PlayState* play) {

}
