#ifndef ARMA_SKEL_H
#define ARMA_SKEL_H

#include "ultra64.h"
#include "array_count.h"
#include "animation.h"
#include "object_arma.h"

extern u64 Arma_skel_arma_torso_ci4[];
extern u64 Arma_skel_arma_torso_pal_rgba16[];
extern u64 Arma_skel_arma_head_ci4[];
extern u64 Arma_skel_arma_head_pal_rgba16[];
extern u64 Arma_skel_rainbow_spear_rgba16[];
extern u64 Arma_skel_arma_leg_ci4[];
extern u64 Arma_skel_arma_leg_pal_rgba16[];
extern Vtx Arma_skel_Bone_mesh_layer_Opaque_vtx_0[32];
extern Gfx Arma_skel_Bone_mesh_layer_Opaque_tri_0[];
extern Vtx Arma_skel_Bone_001_mesh_layer_Opaque_vtx_0[79];
extern Gfx Arma_skel_Bone_001_mesh_layer_Opaque_tri_0[];
extern Vtx Arma_skel_Bone_002_mesh_layer_Opaque_vtx_0[138];
extern Gfx Arma_skel_Bone_002_mesh_layer_Opaque_tri_0[];
extern Vtx Arma_skel_Bone_002_mesh_layer_Opaque_vtx_1[44];
extern Gfx Arma_skel_Bone_002_mesh_layer_Opaque_tri_1[];
extern Vtx Arma_skel_Bone_004_mesh_layer_Opaque_vtx_0[9];
extern Gfx Arma_skel_Bone_004_mesh_layer_Opaque_tri_0[];
extern Vtx Arma_skel_Bone_005_mesh_layer_Opaque_vtx_0[16];
extern Gfx Arma_skel_Bone_005_mesh_layer_Opaque_tri_0[];
extern Vtx Arma_skel_Bone_007_mesh_layer_Opaque_vtx_0[6];
extern Gfx Arma_skel_Bone_007_mesh_layer_Opaque_tri_0[];
extern Vtx Arma_skel_Bone_008_mesh_layer_Opaque_vtx_0[18];
extern Gfx Arma_skel_Bone_008_mesh_layer_Opaque_tri_0[];
extern Gfx mat_Arma_skel_f3dlite_material_002_layerOpaque[];
extern Gfx mat_Arma_skel_f3dlite_material_layerOpaque[];
extern Gfx mat_Arma_skel_f3dlite_material_004_layerOpaque[];
extern Gfx mat_Arma_skel_f3dlite_material_003_layerOpaque[];
extern Gfx Arma_skel_Bone_mesh_layer_Opaque[];
extern Gfx Arma_skel_Bone_001_mesh_layer_Opaque[];
extern Gfx Arma_skel_Bone_002_mesh_layer_Opaque[];
extern Gfx Arma_skel_Bone_004_mesh_layer_Opaque[];
extern Gfx Arma_skel_Bone_005_mesh_layer_Opaque[];
extern Gfx Arma_skel_Bone_007_mesh_layer_Opaque[];
extern Gfx Arma_skel_Bone_008_mesh_layer_Opaque[];
extern FlexSkeletonHeader Arma_skel;
#define ARMA_SKEL_BONE_POS_LIMB 0
#define ARMA_SKEL_BONE_ROT_LIMB 1
#define ARMA_SKEL_BONE_001_LIMB 2
#define ARMA_SKEL_BONE_002_LIMB 3
#define ARMA_SKEL_BONE_004_LIMB 4
#define ARMA_SKEL_BONE_005_LIMB 5
#define ARMA_SKEL_BONE_007_LIMB 6
#define ARMA_SKEL_BONE_008_LIMB 7
#define ARMA_SKEL_NUM_LIMBS 8

#endif
