#pragma once

#define OFF_PLAYER_MANAGER          180740496ULL
#define OFF_PM_LOCAL_PLAYER         0x70
#define OFF_PM_PLAYER_LIST          0x28
#define OFF_LIST_COUNT              0x20
#define OFF_LIST_BUFFER             0x18
#define OFF_LIST_ENTRY_BASE         0x30
#define OFF_LIST_ENTRY_STRIDE       0x18

#define OFF_PLAYER_HEALTH           0x7C
#define OFF_PLAYER_TEAM             0x79
#define OFF_PLAYER_MOVEMENT_CTRL    0x98
#define OFF_PLAYER_PHOTON_PTR       0x160
#define OFF_PLAYER_MAIN_CAMERA      0xE8
#define OFF_CAMERA_TRANSFORM        0x100
#define OFF_CAM_TRANSFORM_MATRIX    0x10
#define OFF_CAM_MATRIX_DATA         0xF0
#define PLAYER_HEIGHT               1.67f

#define OFF_PHOTON_NAME             0x20
#define OFF_PHOTON_PROPS_REG        0x38
#define OFF_PROPS_COUNT             0x20
#define OFF_PROPS_LIST              0x18
#define OFF_PROPS_KEY_BASE          0x28
#define OFF_PROPS_VAL_BASE          0x30
#define OFF_PROPS_VALUE_DATA        0x10

#define OFF_TD_POSITION             0x44
#define OFF_TRANSFORM_MATRIX        0x28
#define OFF_TRANSFORM_INDEX         0x30
#define OFF_MATRIX_LIST             0x18
#define OFF_MATRIX_INDICES          0x20
#define TRANSFORM_MATRIX_SIZE       48

#define OFF_PLAYER_VIEW_1           0x48
#define OFF_PLAYER_VIEW_2           0x50
#define OFF_PLAYER_OCCLUSION        0xB8
#define OFF_OCCLUSION_CURRENT       0x34
#define OFF_OCCLUSION_NEXT          0x38

#define OFF_PLAYER_CHAR_VIEW        0x48
#define OFF_CHAR_VIEW_BIPED_MAP     0x48
#define OFF_VIEW_BIPED_MAP          0x48

#define OFF_PLAYER_WEAPONRY         0x88
#define OFF_PLAYER_AIM              0x80
#define OFF_AIM_AIMING_DATA         0x90
#define OFF_AIMING_CUR_AIM_ANG      0x18
#define OFF_AIMING_CUR_EULER_ANG    0x24
#define OFF_PLAYER_CAMERA_HOLDER    0x28
#define OFF_WEAPONRY_CURRENT        0xA0
#define OFF_WC_WEAPON_PROPS         0xA8
#define OFF_WEAPON_LOD              0x88
#define OFF_LOD_SKINNED_ARRAY       0x38
#define OFF_LOD_MESH_ARRAY          0x48
#define OFF_UNITY_ARRAY_LENGTH      0x18
#define OFF_UNITY_ARRAY_DATA        0x20
#define OFF_PLAYER_ARMS_LOD         0xC8
#define OFF_ARMS_MESH_RENDERER      0x28
#define OFF_ARMS_GLOVES_RENDERER    0x30
#define OFF_PLAYER_CHAR_VIEW_TPS    0xD0
#define OFF_CHAR_VIEW_OCCLUSION     0x30
#define OFF_PLAYER_CHAR_VISIBLE     0xD8
#define OFF_PLAYER_CHAR_LOD         0x128
#define OFF_CHAR_LOD_MESH_RENDERER  0x30

#define OFF_BIPED_START             0x20
#define OFF_BIPED_STRIDE            8
#define BIPED_BONE_COUNT            22

#define BONE_HEAD                   0
#define BONE_NECK                   1
#define BONE_SPINE                  2
#define BONE_SPINE1                 3
#define BONE_SPINE2                 4
#define BONE_LEFT_SHOULDER          5
#define BONE_LEFT_UPPERARM          6
#define BONE_LEFT_FOREARM           7
#define BONE_LEFT_HAND              8
#define BONE_RIGHT_SHOULDER         9
#define BONE_RIGHT_UPPERARM         10
#define BONE_RIGHT_FOREARM          11
#define BONE_RIGHT_HAND             12
#define BONE_HIP                    13
#define BONE_LEFT_UPLEG             14
#define BONE_LEFT_LEG               15
#define BONE_LEFT_FOOT              16
#define BONE_LEFT_TOE               17
#define BONE_RIGHT_UPLEG            18
#define BONE_RIGHT_LEG              19
#define BONE_RIGHT_FOOT             20
#define BONE_RIGHT_TOE              21

#define OFF_MC_TRANSFORM_DATA       0xB0

#define OFF_UNITY_STRING_LENGTH     0x10
#define OFF_UNITY_STRING_CHARS      0x14
