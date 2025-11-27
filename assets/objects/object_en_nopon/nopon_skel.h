#ifndef NOPON_SKEL_H
#define NOPON_SKEL_H

#include "ultra64.h"
#include "array_count.h"
#include "animation.h"
#include "object_en_nopon.h"

extern u64 nopon_skel_pc060000_arm_ci8[];
extern u64 nopon_skel_pc060000_arm_pal_rgba16[];
extern u64 nopon_skel_regular_mouth_ci8[];
extern u64 nopon_skel_regular_mouth_pal_rgba16[];
extern u64 nopon_skel_riki_eyes_ci8[];
extern u64 nopon_skel_riki_eyes_pal_rgba16[];
extern u64 nopon_skel_head_fur_ci8[];
extern u64 nopon_skel_head_fur_pal_rgba16[];
extern Vtx nopon_skel_Bone_001_mesh_layer_Opaque_vtx_0[44];
extern Gfx nopon_skel_Bone_001_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_002_mesh_layer_Opaque_vtx_0[22];
extern Gfx nopon_skel_Bone_002_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_004_mesh_layer_Opaque_vtx_0[15];
extern Gfx nopon_skel_Bone_004_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_007_mesh_layer_Opaque_vtx_0[26];
extern Gfx nopon_skel_Bone_007_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_008_mesh_layer_Opaque_vtx_0[20];
extern Gfx nopon_skel_Bone_008_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_009_mesh_layer_Opaque_vtx_0[16];
extern Gfx nopon_skel_Bone_009_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_005_mesh_layer_Opaque_vtx_0[100];
extern Gfx nopon_skel_Bone_005_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_005_mesh_layer_Opaque_vtx_1[8];
extern Gfx nopon_skel_Bone_005_mesh_layer_Opaque_tri_1[];
extern Vtx nopon_skel_Bone_005_mesh_layer_Opaque_vtx_2[21];
extern Gfx nopon_skel_Bone_005_mesh_layer_Opaque_tri_2[];
extern Vtx nopon_skel_Bone_005_mesh_layer_Opaque_vtx_3[5];
extern Gfx nopon_skel_Bone_005_mesh_layer_Opaque_tri_3[];
extern Vtx nopon_skel_Bone_010_mesh_layer_Opaque_vtx_0[12];
extern Gfx nopon_skel_Bone_010_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_012_mesh_layer_Opaque_vtx_0[16];
extern Gfx nopon_skel_Bone_012_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_013_mesh_layer_Opaque_vtx_0[31];
extern Gfx nopon_skel_Bone_013_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_011_mesh_layer_Opaque_vtx_0[8];
extern Gfx nopon_skel_Bone_011_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_014_mesh_layer_Opaque_vtx_0[16];
extern Gfx nopon_skel_Bone_014_mesh_layer_Opaque_tri_0[];
extern Vtx nopon_skel_Bone_015_mesh_layer_Opaque_vtx_0[31];
extern Gfx nopon_skel_Bone_015_mesh_layer_Opaque_tri_0[];
extern Gfx mat_nopon_skel_f3dlite_material_005_layerOpaque[];
extern Gfx mat_revert_nopon_skel_f3dlite_material_005_layerOpaque[];
extern Gfx mat_nopon_skel_f3dlite_material_007_layerOpaque[];
extern Gfx mat_revert_nopon_skel_f3dlite_material_007_layerOpaque[];
extern Gfx mat_nopon_skel_f3dlite_material_004_layerOpaque[];
extern Gfx mat_revert_nopon_skel_f3dlite_material_004_layerOpaque[];
extern Gfx mat_nopon_skel_f3dlite_material_006_layerOpaque[];
extern Gfx mat_revert_nopon_skel_f3dlite_material_006_layerOpaque[];
extern Gfx nopon_skel_Bone_001_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_002_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_004_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_007_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_008_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_009_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_005_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_010_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_012_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_013_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_011_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_014_mesh_layer_Opaque[];
extern Gfx nopon_skel_Bone_015_mesh_layer_Opaque[];
extern FlexSkeletonHeader nopon_skel;
#define NOPON_SKEL_BONE_POS_LIMB 0
#define NOPON_SKEL_BONE_ROT_LIMB 1
#define NOPON_SKEL_BONE_001_LIMB 2
#define NOPON_SKEL_BONE_002_LIMB 3
#define NOPON_SKEL_BONE_004_LIMB 4
#define NOPON_SKEL_BONE_007_LIMB 5
#define NOPON_SKEL_BONE_008_LIMB 6
#define NOPON_SKEL_BONE_009_LIMB 7
#define NOPON_SKEL_BONE_005_LIMB 8
#define NOPON_SKEL_BONE_010_LIMB 9
#define NOPON_SKEL_BONE_012_LIMB 10
#define NOPON_SKEL_BONE_013_LIMB 11
#define NOPON_SKEL_BONE_011_LIMB 12
#define NOPON_SKEL_BONE_014_LIMB 13
#define NOPON_SKEL_BONE_015_LIMB 14
#define NOPON_SKEL_NUM_LIMBS 15

#endif
