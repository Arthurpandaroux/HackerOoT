#ifndef TEST_NOPON_SCENE_H
#define TEST_NOPON_SCENE_H

#include "ultra64.h"
#include "romfile.h"
#include "array_count.h"
#include "sequence.h"
#include "actor_profile.h"
#include "bgcheck.h"
#include "camera.h"
#include "cutscene.h"
#include "cutscene_commands.h"
#include "environment.h"
#include "z_math.h"
#include "object.h"
#include "ocarina.h"
#include "path.h"
#include "player.h"
#include "room.h"
#include "scene.h"
#include "event_manager.h"
#include "animated_materials.h"
#include "save.h"

// For older decomp versions
#ifndef SCENE_CMD_PLAYER_ENTRY_LIST
#define SCENE_CMD_PLAYER_ENTRY_LIST(length, playerEntryList) \
    { SCENE_CMD_ID_SPAWN_LIST, length, CMD_PTR(playerEntryList) }
#undef SCENE_CMD_SPAWN_LIST
#define SCENE_CMD_SPAWN_LIST(spawnList) \
    { SCENE_CMD_ID_ENTRANCE_LIST, 0, CMD_PTR(spawnList) }
#endif

#ifndef BLEND_RATE_AND_FOG_NEAR
#define BLEND_RATE_AND_FOG_NEAR(blendRate, fogNear) (s16)((((blendRate) / 4) << 10) | (fogNear))
#endif

// Animated Materials requires the segment number to be offset by 7
#ifndef MATERIAL_SEGMENT_NUM
#define MATERIAL_SEGMENT_NUM(n) ((n) - 7)
#endif

// The last entry also requires to be a negative number
#ifndef LAST_MATERIAL_SEGMENT_NUM
#define LAST_MATERIAL_SEGMENT_NUM(n) -MATERIAL_SEGMENT_NUM(n)
#endif

extern SceneCmd test_nopon_scene_header00[];
extern RomFile test_nopon_scene_roomList[];
extern u8 _test_nopon_room_0SegmentRomStart[];
extern u8 _test_nopon_room_0SegmentRomEnd[];
extern ActorEntry test_nopon_scene_header00_playerEntryList[];
extern Spawn test_nopon_scene_header00_entranceList[];
extern EnvLightSettings test_nopon_scene_header00_lightSettings[4];
#if ENABLE_ANIMATED_MATERIALS
extern AnimatedMaterial test_nopon_scene_header00_AnimMat[];
#endif
extern SurfaceType test_nopon_scene_polygonTypes[3];
extern Vec3s test_nopon_scene_vertices[28];
extern CollisionPoly test_nopon_scene_polygons[46];
extern CollisionHeader test_nopon_scene_collisionHeader;
extern SceneCmd test_nopon_room_0_header00[];
extern s16 test_nopon_room_0_header00_objectList[];
extern ActorEntry test_nopon_room_0_header00_actorList[];
extern Gfx test_nopon_room_0_shapeHeader_entry_0_opaque[];
extern u64 test_nopon_room_0_dl_grass_1_ci4[];
extern u64 test_nopon_room_0_dl_grass_1_x_grass_3_pal_rgba16[];
extern u64 test_nopon_room_0_dl_grass_3_ci4[];
extern u64 test_nopon_room_0_dl_wall_ci4[];
extern u64 test_nopon_room_0_dl_wall_pal_rgba16[];
extern u64 test_nopon_room_0_dl_sword_art_ci8[];
extern u64 test_nopon_room_0_dl_sword_art_pal_rgba16[];
extern Vtx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_vtx_cull[8];
extern Vtx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_vtx_0[36];
extern Gfx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_tri_0[];
extern Vtx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_vtx_1[20];
extern Gfx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_tri_1[];
extern Vtx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_vtx_2[4];
extern Gfx test_nopon_room_0_dl_Floor_mesh_layer_Opaque_tri_2[];
extern Gfx mat_test_nopon_room_0_dl_floor_mat_layerOpaque[];
extern Gfx mat_test_nopon_room_0_dl_f3dlite_material_layerOpaque[];
extern Gfx mat_test_nopon_room_0_dl_f3dlite_material_001_layerOpaque[];
extern Gfx test_nopon_room_0_dl_Floor_mesh_layer_Opaque[];
extern RoomShapeNormal test_nopon_room_0_shapeHeader;
extern RoomShapeDListsEntry test_nopon_room_0_shapeDListsEntry[1];

#endif
