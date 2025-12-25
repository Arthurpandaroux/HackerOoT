#ifndef TIRKIN_SKEL_H
#define TIRKIN_SKEL_H

#include "ultra64.h"
#include "array_count.h"
#include "animation.h"
#include "object_tirkin.h"

extern u64 Tirkin_skel_tirkin_skin_ci4[];
extern u64 Tirkin_skel_tirkin_skin_pal_rgba16[];
extern u64 Tirkin_skel_torso_tirkin_ci4[];
extern u64 Tirkin_skel_torso_tirkin_pal_rgba16[];
extern u64 Tirkin_skel_tirkin_mouth_ci4[];
extern u64 Tirkin_skel_tirkin_mouth_pal_rgba16[];
extern u64 Tirkin_skel_wings_tirkin_ci4[];
extern u64 Tirkin_skel_wings_tirkin_pal_rgba16[];
extern u64 Tirkin_skel_blue_hair_tirkin_ci4[];
extern u64 Tirkin_skel_blue_hair_tirkin_pal_rgba16[];
extern u64 Tirkin_skel_toes_and_hands_tirkin_ci4[];
extern u64 Tirkin_skel_toes_and_hands_tirkin_pal_rgba16[];
extern u64 Tirkin_skel_rainbow_spear_rgba16[];
extern Vtx Tirkin_skel_Bone_mesh_layer_Opaque_vtx_0[34];
extern Gfx Tirkin_skel_Bone_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_mesh_layer_Opaque_vtx_1[12];
extern Gfx Tirkin_skel_Bone_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_005_mesh_layer_Opaque_vtx_0[19];
extern Gfx Tirkin_skel_Bone_005_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_005_mesh_layer_Opaque_vtx_1[18];
extern Gfx Tirkin_skel_Bone_005_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_005_mesh_layer_Opaque_vtx_2[3];
extern Gfx Tirkin_skel_Bone_005_mesh_layer_Opaque_tri_2[];
extern Vtx Tirkin_skel_Bone_005_mesh_layer_Opaque_vtx_3[21];
extern Gfx Tirkin_skel_Bone_005_mesh_layer_Opaque_tri_3[];
extern Vtx Tirkin_skel_Bone_008_mesh_layer_Opaque_vtx_0[91];
extern Gfx Tirkin_skel_Bone_008_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_008_mesh_layer_Opaque_vtx_1[19];
extern Gfx Tirkin_skel_Bone_008_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_008_mesh_layer_Opaque_vtx_2[20];
extern Gfx Tirkin_skel_Bone_008_mesh_layer_Opaque_tri_2[];
extern Vtx Tirkin_skel_Bone_006_mesh_layer_Opaque_vtx_0[45];
extern Gfx Tirkin_skel_Bone_006_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_006_mesh_layer_Opaque_vtx_1[3];
extern Gfx Tirkin_skel_Bone_006_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_007_mesh_layer_Opaque_vtx_0[30];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_007_mesh_layer_Opaque_vtx_1[51];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_007_mesh_layer_Opaque_vtx_2[3];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque_tri_2[];
extern Vtx Tirkin_skel_Bone_007_mesh_layer_Opaque_vtx_3[21];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque_tri_3[];
extern Vtx Tirkin_skel_Bone_007_mesh_layer_Opaque_vtx_4[68];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque_tri_4[];
extern Vtx Tirkin_skel_Bone_007_mesh_layer_Opaque_vtx_5[41];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque_tri_5[];
extern Vtx Tirkin_skel_Bone_009_mesh_layer_Opaque_vtx_0[33];
extern Gfx Tirkin_skel_Bone_009_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_009_mesh_layer_Opaque_vtx_1[4];
extern Gfx Tirkin_skel_Bone_009_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_010_mesh_layer_Opaque_vtx_0[21];
extern Gfx Tirkin_skel_Bone_010_mesh_layer_Opaque_tri_0[];
extern Vtx Tirkin_skel_Bone_010_mesh_layer_Opaque_vtx_1[6];
extern Gfx Tirkin_skel_Bone_010_mesh_layer_Opaque_tri_1[];
extern Vtx Tirkin_skel_Bone_010_mesh_layer_Opaque_vtx_2[30];
extern Gfx Tirkin_skel_Bone_010_mesh_layer_Opaque_tri_2[];
extern Gfx mat_Tirkin_skel_f3dlite_material_003_layerOpaque[];
extern Gfx mat_Tirkin_skel_f3dlite_material_002_layerOpaque[];
extern Gfx mat_Tirkin_skel_f3dlite_material_layerOpaque[];
extern Gfx mat_Tirkin_skel_f3dlite_material_004_layerOpaque[];
extern Gfx mat_Tirkin_skel_f3dlite_material_001_layerOpaque[];
extern Gfx mat_Tirkin_skel_f3dlite_material_005_layerOpaque[];
extern Gfx mat_Tirkin_skel_f3dlite_material_007_layerOpaque[];
extern Gfx Tirkin_skel_Bone_mesh_layer_Opaque[];
extern Gfx Tirkin_skel_Bone_005_mesh_layer_Opaque[];
extern Gfx Tirkin_skel_Bone_008_mesh_layer_Opaque[];
extern Gfx Tirkin_skel_Bone_006_mesh_layer_Opaque[];
extern Gfx Tirkin_skel_Bone_007_mesh_layer_Opaque[];
extern Gfx Tirkin_skel_Bone_009_mesh_layer_Opaque[];
extern Gfx Tirkin_skel_Bone_010_mesh_layer_Opaque[];
extern FlexSkeletonHeader Tirkin_skel;
#define TIRKIN_SKEL_BONE_POS_LIMB 0
#define TIRKIN_SKEL_BONE_ROT_LIMB 1
#define TIRKIN_SKEL_BONE_005_LIMB 2
#define TIRKIN_SKEL_BONE_008_LIMB 3
#define TIRKIN_SKEL_BONE_006_LIMB 4
#define TIRKIN_SKEL_BONE_007_LIMB 5
#define TIRKIN_SKEL_BONE_009_LIMB 6
#define TIRKIN_SKEL_BONE_010_LIMB 7
#define TIRKIN_SKEL_NUM_LIMBS 8

#endif
