#pragma once

#include "../resolve.hpp"

inline void* (*cm)() = nullptr;
inline void (*cam_set_bgcolor)(void*, NC4) = nullptr;
inline void (*cam_set_clearflags)(void*, int) = nullptr;
inline int (*cam_get_clearflags)(void*) = nullptr;

namespace sdk::unity {

inline bool cam_ok = false;

inline bool cam_resolve() {
    if (cam_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Camera");
    if (!c) return false;
    cm = sdk::resolve_m<void* (*)()>(c, "get_main", 0);
    cam_set_bgcolor = sdk::resolve_m<void (*)(void*, NC4)>(c, "set_backgroundColor", 1);
    cam_set_clearflags = sdk::resolve_m<void (*)(void*, int)>(c, "set_clearFlags", 1);
    cam_get_clearflags = sdk::resolve_m<int (*)(void*)>(c, "get_clearFlags", 0);
    cam_ok = cm != nullptr;
    return cam_ok;
}

inline void cam_reset() {
    cm = nullptr;
    cam_set_bgcolor = nullptr;
    cam_set_clearflags = nullptr;
    cam_get_clearflags = nullptr;
    cam_ok = false;
}

}
