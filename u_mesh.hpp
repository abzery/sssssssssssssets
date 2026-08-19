#pragma once

#include "../resolve.hpp"

inline void (*mgo_ctor)(void*, Il2CppString*) = nullptr;
inline void (*mgo_ctor0)(void*) = nullptr;
inline void* (*mgo_get_transform)(void*) = nullptr;
inline void* (*mgo_add_component)(void*, void*) = nullptr;
inline void (*mgo_set_active)(void*, int) = nullptr;

inline void* (*mesh_ctor)(void*) = nullptr;
inline void (*mesh_set_vertices)(void*, void*) = nullptr;
inline void (*mesh_set_normals)(void*, void*) = nullptr;
inline void (*mesh_set_uv)(void*, void*) = nullptr;
inline void (*mesh_set_triangles)(void*, void*) = nullptr;
inline void (*mesh_set_index_format)(void*, int) = nullptr;
inline void (*mesh_recalc_bounds)(void*) = nullptr;
inline void (*mesh_recalc_normals)(void*) = nullptr;

inline void (*mfilter_set_shared_mesh)(void*, void*) = nullptr;

namespace sdk::unity {

inline bool mesh_ok = false;

inline bool mesh_resolve() {
    if (mesh_ok) return true;
    Il2CppClass* go = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "GameObject");
    Il2CppClass* mesh = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Mesh");
    Il2CppClass* mf = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "MeshFilter");
    if (!go || !mesh || !mf) return false;
    mgo_ctor = sdk::resolve_m<void (*)(void*, Il2CppString*)>(go, ".ctor", 1);
    mgo_ctor0 = sdk::resolve_m<void (*)(void*)>(go, ".ctor", 0);
    mgo_get_transform = sdk::resolve_m<void* (*)(void*)>(go, "get_transform", 0);
    mgo_add_component = sdk::resolve_m<void* (*)(void*, void*)>(go, "AddComponent", 1);
    mgo_set_active = sdk::resolve_m<void (*)(void*, int)>(go, "SetActive", 1);
    mesh_ctor = sdk::resolve_m<void* (*)(void*)>(mesh, ".ctor", 0);
    mesh_set_vertices = sdk::resolve_m<void (*)(void*, void*)>(mesh, "set_vertices", 1);
    mesh_set_normals = sdk::resolve_m<void (*)(void*, void*)>(mesh, "set_normals", 1);
    mesh_set_uv = sdk::resolve_m<void (*)(void*, void*)>(mesh, "set_uv", 1);
    mesh_set_triangles = sdk::resolve_m<void (*)(void*, void*)>(mesh, "set_triangles", 1);
    mesh_set_index_format = sdk::resolve_m<void (*)(void*, int)>(mesh, "set_indexFormat", 1);
    mesh_recalc_bounds = sdk::resolve_m<void (*)(void*)>(mesh, "RecalculateBounds", 0);
    mesh_recalc_normals = sdk::resolve_m<void (*)(void*)>(mesh, "RecalculateNormals", 0);
    mfilter_set_shared_mesh = sdk::resolve_m<void (*)(void*, void*)>(mf, "set_sharedMesh", 1);
    mesh_ok = mgo_ctor && mgo_get_transform && mgo_add_component && mesh_ctor &&
              mesh_set_vertices && mesh_set_normals && mesh_set_uv && mesh_set_triangles &&
              mesh_set_index_format && mesh_recalc_bounds && mfilter_set_shared_mesh;
    return mesh_ok;
}

inline void mesh_reset() {
    mgo_ctor = nullptr;
    mgo_ctor0 = nullptr;
    mgo_get_transform = nullptr;
    mgo_add_component = nullptr;
    mgo_set_active = nullptr;
    mesh_ctor = nullptr;
    mesh_set_vertices = nullptr;
    mesh_set_normals = nullptr;
    mesh_set_uv = nullptr;
    mesh_set_triangles = nullptr;
    mesh_set_index_format = nullptr;
    mesh_recalc_bounds = nullptr;
    mesh_recalc_normals = nullptr;
    mfilter_set_shared_mesh = nullptr;
    mesh_ok = false;
}

}
